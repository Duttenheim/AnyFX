//------------------------------------------------------------------------------
//  varbuffer.cc
//  (C) 2013 Gustav Sterbrant
//------------------------------------------------------------------------------
#include "varbuffer.h"
#include "parameter.h"
#include <algorithm>
#include "typechecker.h"
#include "shader.h"
#include "effect.h"

namespace AnyFX
{

//------------------------------------------------------------------------------
/**
*/
VarBuffer::VarBuffer() :
	alignedSize(0),
	size(0),
	hasAnnotation(false),
	group(0),
	binding(0)
{
	this->symbolType = Symbol::VarbufferType;
}

//------------------------------------------------------------------------------
/**
*/
VarBuffer::~VarBuffer()
{
	// empty
}

//------------------------------------------------------------------------------
/**
*/
void
VarBuffer::AddVariable(const Variable& var)
{
	this->variables.push_back(var);
}

//------------------------------------------------------------------------------
/**
*/
void
VarBuffer::TypeCheck(TypeChecker& typechecker)
{
	// add varblock, if failed we must have a redefinition
	if (!typechecker.AddSymbol(this)) return;

	// type check annotation
	if (this->hasAnnotation)
	{
		this->annotation.TypeCheck(typechecker);
	}

	// evaluate qualifiers
	for (unsigned i = 0; i < this->qualifiers.size(); i++)
	{
		const std::string& qualifier = this->qualifiers[i];
		if (qualifier == "shared") this->qualifierFlags |= Qualifiers::Shared;
		else
		{
			std::string message = AnyFX::Format("Unknown qualifier '%s', %s\n", qualifier.c_str(), this->ErrorSuffix().c_str());
			typechecker.Error(message);
		}
	}

	for (unsigned i = 0; i < this->qualifierExpressions.size(); i++)
	{
		const std::string& qualifier = this->qualifierExpressions[i].name;
		Expression* expr = this->qualifierExpressions[i].expr;
		if (qualifier == "group") this->group = expr->EvalUInt(typechecker);
		else
		{
			std::string message = AnyFX::Format("Unknown qualifier '%s', %s\n", qualifier.c_str(), this->ErrorSuffix().c_str());
			typechecker.Error(message);
		}
		delete expr;
	}

	// get the binding location and increment the global counter
	if (typechecker.GetHeader().GetType() == Header::GLSL)
	{
		this->binding = Shader::bindingIndices[1]++;
	}
	else if (typechecker.GetHeader().GetType() == Header::SPIRV)
	{
		this->binding = Shader::bindingIndices[this->group]++;
	}

    const Header& header = typechecker.GetHeader();
    if (header.GetType() != Header::GLSL && header.GetType() != Header::SPIRV)
    {
        std::string message = AnyFX::Format("Varbuffers are only supported in OpenGL, %s\n", this->ErrorSuffix().c_str());
        typechecker.Error(message);
    }
    else
    {
        if ((header.GetMajor() < 4 && header.GetMinor() < 3) && header.GetType() != Header::SPIRV)
        {
            std::string message = AnyFX::Format("Varbuffers are only supported in OpenGL 4.3+, %s\n", this->ErrorSuffix().c_str());
            typechecker.Error(message);
        }
    }

	unsigned offset = 0;
	unsigned i;
	for (i = 0; i < this->variables.size(); i++)
	{
        Variable var = this->variables[i];
        if (var.GetArrayType() == Variable::UnsizedArray && i < this->variables.size() - 1)
        {
            std::string message = AnyFX::Format("Varbuffers can only have its last member as an unsized array, %s\n", var.GetName().c_str(), this->ErrorSuffix().c_str());
            typechecker.Error(message);
        }

		var.TypeCheck(typechecker);
		this->size += var.GetByteSize() * var.GetArraySize();

		// handle offset later, now we know array size
		unsigned alignedSize = 0;
		unsigned stride = 0;
		unsigned elementStride = 0;
		unsigned alignment = 0;
		std::vector<unsigned> suboffsets;
		if (header.GetType() == Header::GLSL || header.GetType() == Header::SPIRV)
			alignment = Effect::GetAlignmentGLSL(var.GetDataType(), var.GetArraySize(), alignedSize, stride, elementStride, suboffsets, false, typechecker);

		// if we have a struct, we need to unroll it, and calculate the offsets
		const DataType& type = var.GetDataType();

		// avoid adding actual struct
		if (type.GetType() == DataType::UserType)
		{
			// unroll structures to generate variables with proper names, they should come in the same order as the suboffsets
			Structure* structure = dynamic_cast<Structure*>(typechecker.GetSymbol(type.GetName()));
			std::vector<Variable> subvars;
			structure->Unroll(var.GetName(), subvars, typechecker);

			// add suboffsets to this offset
			assert(subvars.size() == suboffsets.size());
			unsigned j;
			for (j = 0; j < suboffsets.size(); j++)
			{
				// append structure offset to base
				this->offsetsByName[subvars[j].GetName()] = offset + suboffsets[j];
			}
		}
		else
		{
			// add offset to list
			this->offsetsByName[var.GetName()] = offset;
		}

		// offset should be size of struct, round of
		offset += alignedSize;
		offset = Effect::AlignToPow(offset, alignment);
	}

	this->alignedSize = offset;
}

//------------------------------------------------------------------------------
/**
*/
std::string
VarBuffer::Format(const Header& header) const
{
	std::string formattedCode;

    // varbuffers of this type are only available in GLSL4+
	if (header.GetType() == Header::GLSL)
	{
		std::string layout = AnyFX::Format("layout(std430, binding=%d) buffer ", this->binding);
		formattedCode.append(layout);
	}
	else if (header.GetType() == Header::SPIRV)
	{
		std::string layout = AnyFX::Format("layout(std430, set=%d, binding=%d) buffer ", this->group, this->binding);
		formattedCode.append(layout);
	}
	
	formattedCode.append(this->GetName());
	formattedCode.append("\n{\n");

	unsigned i;
	for (i = 0; i < this->variables.size(); i++)
	{
		const Variable& var = this->variables[i];

		// format code and add to code
		formattedCode.append(var.Format(header, true));
	}
	
	// finalize and return
	formattedCode.append("};\n\n");
	return formattedCode;
}

//------------------------------------------------------------------------------
/**
*/
void
VarBuffer::Compile(BinWriter& writer)
{
	writer.WriteString(this->name);
	writer.WriteUInt(this->alignedSize);
	writer.WriteUInt(this->size);
	writer.WriteUInt(ToInteger(this->qualifierFlags));
	writer.WriteUInt(this->binding);
	writer.WriteUInt(this->group);

	// write if annotation is used
	writer.WriteBool(this->hasAnnotation);

	// compile annotation if
	if (this->hasAnnotation)
	{
		this->annotation.Compile(writer);
	}	

	// write offsets
	writer.WriteUInt(this->offsetsByName.size());
	std::map<std::string, unsigned>::const_iterator it = this->offsetsByName.begin();
	for (; it != this->offsetsByName.end(); it++)
	{
		writer.WriteString((*it).first);
		writer.WriteUInt((*it).second);
	}
}

//------------------------------------------------------------------------------
/**
	qsort hook for sorting parameters
*/
bool
VarbufferParamCompare(const Variable& v1, const Variable& v2)
{
	return v1.GetByteSize() * v1.GetArraySize() > v2.GetByteSize() * v1.GetArraySize();
}

//------------------------------------------------------------------------------
/**
	sort variables based on type size in decreasing order, meaning the first value has the biggest size
	(this only needs to be here because there seems to be some bug with uniform buffers (GLSL) on ATI cards)
	but it's always good practice to properly pack your data ;)
*/
void 
VarBuffer::SortVariables()
{
	std::sort(this->variables.begin(), this->variables.end(), VarbufferParamCompare);
}
} // namespace AnyFX

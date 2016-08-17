//------------------------------------------------------------------------------
//  varblock.cc
//  (C) 2013 Gustav Sterbrant
//------------------------------------------------------------------------------
#include "varblock.h"
#include "parameter.h"
#include <algorithm>
#include "typechecker.h"
#include "shader.h"
#include "structure.h"
#include "effect.h"

namespace AnyFX
{

//------------------------------------------------------------------------------
/**
*/
VarBlock::VarBlock() :
	hasAnnotation(false),
	shared(false),
	group(0),
	binding(0),
	push(false)
{
	this->symbolType = Symbol::VarblockType;
}

//------------------------------------------------------------------------------
/**
*/
VarBlock::~VarBlock()
{
	// empty
}

//------------------------------------------------------------------------------
/**
*/
void
VarBlock::AddVariable(const Variable& var)
{
	this->variables.push_back(var);
}

//------------------------------------------------------------------------------
/**
*/
void
VarBlock::TypeCheck(TypeChecker& typechecker)
{
	// add varblock, if failed we must have a redefinition
	if (!typechecker.AddSymbol(this)) return;
	const Header& header = typechecker.GetHeader();

	// type check annotation
	if (this->hasAnnotation)
	{
		this->annotation.TypeCheck(typechecker);
	}

	// evaluate qualifiers
	for (unsigned i = 0; i < this->qualifiers.size(); i++)
	{
		const std::string& qualifier = this->qualifiers[i];
		if (qualifier == "shared") this->shared = true;
		else if (qualifier == "push") this->push = true;
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
	if (header.GetType() == Header::GLSL)
	{
		this->binding = Shader::bindingIndices[0];
		Shader::bindingIndices[0]++;
	}
	else if (header.GetType() == Header::SPIRV && !this->push)
	{
		this->binding = Shader::bindingIndices[this->group];
		Shader::bindingIndices[this->group]++;
	}

	unsigned offset = 0;
	unsigned i;
	for (i = 0; i < this->variables.size(); i++)
	{
        Variable& var = this->variables[i];
		var.group = this->group;
        if (var.GetArrayType() == Variable::UnsizedArray)
        {
            std::string message = AnyFX::Format("Varblocks cannot contain variables of unsized type! (%s), %s\n", var.GetName().c_str(), this->ErrorSuffix().c_str());
            typechecker.Error(message);
        }

        // since TypeCheck might modify the variable, we must replace the old one. 
		var.TypeCheck(typechecker);

		// handle offset later, now we know array size
		unsigned alignedSize = 0;
		unsigned stride = 0;
		unsigned elementStride = 0;
		unsigned alignment = 0;
		std::vector<unsigned> suboffsets;
		if (header.GetType() == Header::GLSL || header.GetType() == Header::SPIRV)
			alignment = Effect::GetAlignmentGLSL(var.GetDataType(), var.GetArraySize(), alignedSize, stride, elementStride, suboffsets, true, typechecker);

		// if we have a struct, we need to unroll it, and calculate the offsets
		const DataType& type = var.GetDataType();

		// align offset with current alignment
		offset = Effect::AlignToPow(offset, alignment);

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
	}
	// aligned size must be the sum of all offsets
	this->alignedSize = offset;

	Header::Type type = header.GetType();
	int major = header.GetMajor();
	if (type == Header::GLSL)
	{
		if (major < 3)
		{
			std::string message = AnyFX::Format("Varblocks are only supported in GLSL versions 3+, %s\n", this->ErrorSuffix().c_str());
			typechecker.Error(message);
		}
	}
	else if (type == Header::HLSL)
	{
		if (major < 4)
		{
			std::string message = AnyFX::Format("Varblocks are only supported in HLSL versions 4+, %s\n", this->ErrorSuffix().c_str());
			typechecker.Error(message);
		}
	}
	else if (type == Header::SPIRV)
	{
		if (this->shared && this->push)
		{
			std::string message = AnyFX::Format("Varblocks can not both qualifiers 'shared' and 'push', %s\n", this->ErrorSuffix().c_str());
			typechecker.Error(message);
		}
	}
}

//------------------------------------------------------------------------------
/**
*/
std::string
VarBlock::Format(const Header& header) const
{
	std::string formattedCode;

    // only output if we have variables
    if (this->variables.empty()) return formattedCode;

    if (this->shared)
    {
        // varblocks of this type are only available in GLSL3-4, HLSL4-5 and SPIR-V
		if (header.GetType() == Header::GLSL)
		{
			std::string layout = AnyFX::Format("layout(shared, binding=%d) uniform ", this->binding);
			formattedCode.append(layout);
		}
		else if (header.GetType() == Header::SPIRV)
		{
			std::string layout = AnyFX::Format("layout(std140, set=%d, binding=%d) uniform ", this->group, this->binding);
			formattedCode.append(layout);
		}
        else if (header.GetType() == Header::HLSL) formattedCode.append("shared cbuffer ");
    }
	else
	{
		// varblocks of this type are only available in GLSL3-4 and HLSL4-5
		if (header.GetType() == Header::GLSL)
		{
			std::string layout = AnyFX::Format("layout(std140, binding=%d) uniform ", this->binding);
			formattedCode.append(layout);
		}
		else if (header.GetType() == Header::SPIRV)
		{
			if (this->push)
			{
				std::string layout = AnyFX::Format("layout(push_constant) uniform __PC__ ", this->group, this->binding);
				formattedCode.append(layout);
			}
			else
			{
				std::string layout = AnyFX::Format("layout(std140, set=%d, binding=%d) uniform ", this->group, this->binding);
				formattedCode.append(layout);
			}
			
		}
        else if (header.GetType() == Header::HLSL) formattedCode.append("cbuffer ");
    }
	
	// hmm, if this is a push constant range, setup as a single instance struct
	if (this->push)
	{
		std::vector<std::string> macros;
		formattedCode.append("\n{\n");

		unsigned i;
		for (i = 0; i < this->variables.size(); i++)
		{
			const Variable& var = this->variables[i];

			// format code and add to code
			formattedCode.append(var.Format(header, true));
		}

		// finalize and return
		formattedCode.append("} ");
		formattedCode.append(this->GetName());
		formattedCode.append(";\n\n");
	}
	else
	{
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
	}
	return formattedCode;
}

//------------------------------------------------------------------------------
/**
*/
void
VarBlock::Compile(BinWriter& writer)
{
	writer.WriteString(this->name);
	writer.WriteUInt(this->alignedSize);
	writer.WriteBool(this->shared);
	writer.WriteUInt(this->binding);
	writer.WriteUInt(this->group);
	writer.WriteBool(this->push);

	// write if annotation is used
	writer.WriteBool(this->hasAnnotation);

	// compile annotation if
	if (this->hasAnnotation)
	{
		this->annotation.Compile(writer);
	}	

	// write how many variables we have 
	writer.WriteInt(this->variables.size());

	// compile all variable blocks
	unsigned i;
	for (i = 0; i < this->variables.size(); i++)
	{
		this->variables[i].Compile(writer);
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
	sort hook for sorting parameters
*/
bool
VarblockParamCompare(const Variable& v1, const Variable& v2)
{
	return v1.GetByteSize() * v1.GetArraySize() < v2.GetByteSize() * v1.GetArraySize();
}

//------------------------------------------------------------------------------
/**
	sort variables based on type size in decreasing order, meaning the first value has the biggest size
	(this only needs to be here because there seems to be some bug with uniform buffers (GLSL) on ATI cards)
	but it's always good practice to properly pack your data ;)
*/
void 
VarBlock::SortVariables()
{
	std::sort(this->variables.begin(), this->variables.end(), VarblockParamCompare);
}
} // namespace AnyFX

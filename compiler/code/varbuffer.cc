//------------------------------------------------------------------------------
//  varbuffer.cc
//  (C) 2013 Gustav Sterbrant
//------------------------------------------------------------------------------
#include "varbuffer.h"
#include "parameter.h"
#include <algorithm>
#include "typechecker.h"

namespace AnyFX
{

//------------------------------------------------------------------------------
/**
*/
VarBuffer::VarBuffer() :
	hasAnnotation(false)
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
VarBuffer::AddVariable( const Variable& var )
{
	this->variables.push_back(var);
}

//------------------------------------------------------------------------------
/**
*/
void 
VarBuffer::TypeCheck( TypeChecker& typechecker )
{
	// add varblock, if failed we must have a redefinition
	if (!typechecker.AddSymbol(this)) return;

	// type check annotation
	if (this->hasAnnotation)
	{
		this->annotation.TypeCheck(typechecker);
	}

    const Header& header = typechecker.GetHeader();
    if (header.GetType() != Header::GLSL)
    {
        std::string message = AnyFX::Format("Varbuffers are only supported in OpenGL, %s\n", this->ErrorSuffix().c_str());
        typechecker.Error(message);
    }
    else
    {
        if (header.GetMajor() != 4 && header.GetMinor() != 3)
        {
            std::string message = AnyFX::Format("Varbuffers are only supported in OpenGL 4.3+, %s\n", this->ErrorSuffix().c_str());
            typechecker.Error(message);
        }
    }

	unsigned i;
	for (i = 0; i < this->variables.size(); i++)
	{
        Variable var = this->variables[i];
        if (var.GetArrayType() == Variable::UnsizedArray)
        {
            std::string message = AnyFX::Format("Varbuffers cannot contain variables of unsized type! (%s), %s\n", var.GetName().c_str(), this->ErrorSuffix().c_str());
            typechecker.Error(message);
        }

		var.TypeCheck(typechecker);
	}

	Header::Type type = header.GetType();
	int major = header.GetMajor();
	if (type == Header::GLSL)
	{
		if (major < 3)
		{
			std::string message = AnyFX::Format("Varbuffers are only supported in GLSL versions 3+, %s\n", this->ErrorSuffix().c_str());
			typechecker.Error(message);
		}
	}
	else if (type == Header::HLSL)
	{
		if (major < 4)
		{
			std::string message = AnyFX::Format("Varbuffers are only supported in HLSL versions 4+, %s\n", this->ErrorSuffix().c_str());
			typechecker.Error(message);
		}
	}
}

//------------------------------------------------------------------------------
/**
*/
std::string 
VarBuffer::Format( const Header& header ) const
{
	std::string formattedCode;

    if (this->isShared)
    {
        // varblocks of this type are only available in GLSL4+
        if (header.GetType() == Header::GLSL) formattedCode.append("layout(std430, shared) buffer ");
    }
    else
    {
        // varblocks of this type are only available in GLSL4+
        if (header.GetType() == Header::GLSL) formattedCode.append("layout(std430) buffer ");
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
VarBuffer::Compile( BinWriter& writer )
{
	writer.WriteString(this->name);
	writer.WriteBool(this->isShared);

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

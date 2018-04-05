//------------------------------------------------------------------------------
// variablebase.cc
// (C) 2016 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "variablebase.h"

// disable 'strtok': This function or variable may be unsafe.
#pragma warning (disable : 4996)
namespace AnyFX
{

//------------------------------------------------------------------------------
/**
*/
VariableBase::VariableBase() :
	parentBlock(NULL),
	isInVarblock(false),
	isArray(false),
	hasDefaultValue(false),
	sampler(NULL),
	byteSize(0),
	currentValue(NULL),
	arraySize(1),
	binding(0),
	set(0)
{
	// empty
}

//------------------------------------------------------------------------------
/**
*/
VariableBase::~VariableBase()
{
	if (this->currentValue) delete[] this->currentValue;
}

//------------------------------------------------------------------------------
/**
*/
void
VariableBase::OnLoaded()
{
	// setup signature
	std::string typeString = TypeToString(this->type);
	this->signature = typeString + ":" + this->name;

	this->byteSize = TypeToByteSize(this->type) * this->arraySize;
	this->currentValue = new char[this->byteSize];
	if (this->hasDefaultValue)
	{
		this->SetupDefaultValue(this->defaultValueString);
	}
	else
	{
		memset(this->currentValue, 0, this->byteSize);
	}
}

//------------------------------------------------------------------------------
/**
*/
void
VariableBase::SetupDefaultValue(const std::string& string)
{
	unsigned numValues = 0;
	if (string.length() == 0) return;
	std::string copy = string;
	char* data = &copy[0];
	char* str = strtok(data, ",");
	while (str)
	{
		switch (this->type)
		{
		case Float:
		case Float2:
		case Float3:
		case Float4:
		case Double:
		case Double2:
		case Double3:
		case Double4:
		case Matrix2x2:
		case Matrix2x3:
		case Matrix2x4:
		case Matrix3x2:
		case Matrix3x3:
		case Matrix3x4:
		case Matrix4x2:
		case Matrix4x3:
		case Matrix4x4:
		{
			float value = (float)atof(str);
			memcpy((void*)(this->currentValue + numValues * sizeof(float)), (void*)&value, sizeof(float));
			break;
		}
		case Integer:
		case Integer2:
		case Integer3:
		case Integer4:
		case UInteger:
		case UInteger2:
		case UInteger3:
		case UInteger4:
		{
			int value = atoi(str);
			memcpy((void*)(this->currentValue + numValues * sizeof(int)), (void*)&value, sizeof(int));
			break;
		}
		case Bool:
		case Bool2:
		case Bool3:
		case Bool4:
		{
			int value = atoi(str);
			memcpy((void*)(this->currentValue + numValues * sizeof(bool)), (void*)&value, sizeof(bool));
			break;
		}
		}
		str = strtok(NULL, ",");
		numValues++;
	}
}

} // namespace AnyFX
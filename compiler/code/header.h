#pragma once
//------------------------------------------------------------------------------
/**
    @class Header
    
    Containts settings neccessary to compile an AnyFX file.
    
    (C) 2013 Gustav Sterbrant
*/
//------------------------------------------------------------------------------
#include <string>
#include <vector>
#include <map>
#include "compileable.h"
namespace AnyFX
{

class Header : public Compileable
{
public:

	enum Type
	{
		HLSL,
		GLSL,
		SPIRV,
		Metal,
		PS,
		Wii,

		InvalidType,

		NumTypes
	};

	enum Flags
	{
		NoFlags = 0 << 0,
		NoSubroutines = 1 << 1,					// tell compiler to convert used subroutines into new shader programs instead
		PutGlobalVariablesInBlock = 1 << 2,		// tell compiler to put variables outside variable buffer blocks into a global block, named GlobalBlock
		OutputGeneratedShaders = 1 << 3,		// tell compiler to output each shader program to file

		NumFlags
	};

	/// constructor
	Header();
	/// destructor
	virtual ~Header();

	/// sets the effect profile
	void SetProfile(const std::string& profile);
	/// set the effect defines
	void SetFlags(const std::vector<std::string>& defines);
	/// get the effect type, is extracted from the type
	const Type& GetType() const;

	/// get compiler flags
	const int& GetFlags() const;
	/// get compiler value
	const std::string& GetValue(const std::string& str) const;

	/// gets the profile major number
	int GetMajor() const;
	/// gets the profile minor number
	int GetMinor() const;

	/// typecheck header
	void TypeCheck(TypeChecker& typechecker);
	/// compile header
	void Compile(BinWriter& writer);

private:
	std::string profile;
	std::map<std::string, std::string> values;
	int major;
	int minor;
	Type type;
	int flags;
}; 


//------------------------------------------------------------------------------
/**
*/
inline const Header::Type& 
Header::GetType() const
{
	return this->type;
}

//------------------------------------------------------------------------------
/**
*/
inline const int&
Header::GetFlags() const
{
	return this->flags;
}

//------------------------------------------------------------------------------
/**
*/
inline const std::string&
Header::GetValue(const std::string& str) const
{
	std::map<std::string, std::string>::const_iterator it = this->values.find(str);
	if (it != this->values.end()) return it->second;
	else						  return it->first;
}

//------------------------------------------------------------------------------
/**
*/
inline int
Header::GetMajor() const
{
	return this->major;
}

//------------------------------------------------------------------------------
/**
*/
inline int
Header::GetMinor() const
{
	return this->minor;
}

} // namespace AnyFX
//------------------------------------------------------------------------------

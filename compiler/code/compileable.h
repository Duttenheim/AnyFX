#pragma once
//------------------------------------------------------------------------------
/**
    @class AnyFX::Compileable
    
    Base class for every type of data structure which can be compiled.
    
    (C) 2013 Gustav Sterbrant
*/
//------------------------------------------------------------------------------
#include "binwriter.h"
#include "qualifierexpression.h"
#include "types.h"
#include <vector>
#include <string>
namespace AnyFX
{
class TypeChecker;
class Generator;
class Compileable
{
public:

	/// constructor
	Compileable();
	/// destructor
	virtual ~Compileable();

	/// destroy symbol
	virtual void Destroy();

	/// set line
	void SetLine(unsigned line);
	/// get line
	unsigned GetLine() const;
	/// set character position
	void SetPosition(unsigned pos);
	/// get character position
	unsigned GetPosition() const;
	/// set source file
	void SetFile(const std::string& string);
	/// get source file
	const std::string& GetFile() const;
	/// get if the compileable object has an error
	const bool HasError() const;
	/// get error
	const std::string& GetError() const;

	/// add qualifier to varblock
	void AddQualifier(const std::string& qualifier);
	/// get number of qualifiers
	const size_t GetNumQualifiers() const;
	/// get qualifier by index
	const std::string& GetQualifier(unsigned i) const;

	/// add expression qualifier to varblock
	void AddQualifierExpression(const QualifierExpression& qualifier);
	/// get number of expression qualifiers
	const size_t GetNumQualifierExpressions() const;
	/// get name of expression qualifier
	const QualifierExpression& GetQualifierExpression(unsigned i) const;

	/// formats an appropriate error suffix
	std::string ErrorSuffix() const;
	
	/// override to setup necessary information
	virtual void Setup();
	/// override to perform type checking
	virtual void TypeCheck(TypeChecker& typechecker);
	/// override to perform code generation
	virtual void Generate(Generator& generator);
	/// override to perform compilation
	virtual void Compile(BinWriter& writer);

protected:
	std::string error;
	std::string file;
	int line;
	int row;
	bool hasErrors;
	bool hasWarnings;

	Qualifiers qualifierFlags;
	std::vector<std::string> qualifiers;
	std::vector<QualifierExpression> qualifierExpressions;
}; 

//------------------------------------------------------------------------------
/**
*/
inline void
Compileable::SetLine(unsigned line)
{
	this->line = line;
}

//------------------------------------------------------------------------------
/**
*/
inline unsigned 
Compileable::GetLine() const
{
	return this->line;
}

//------------------------------------------------------------------------------
/**
*/
inline void
Compileable::SetPosition(unsigned pos)
{
	this->row = pos;
}

//------------------------------------------------------------------------------
/**
*/
inline unsigned 
Compileable::GetPosition() const
{
	return this->row;
}

//------------------------------------------------------------------------------
/**
*/
inline void
Compileable::SetFile(const std::string& string)
{
	this->file = string;
}

//------------------------------------------------------------------------------
/**
*/
inline const std::string& 
Compileable::GetFile() const
{
	return this->file;
}

//------------------------------------------------------------------------------
/**
*/
inline const bool 
Compileable::HasError() const
{
	return this->hasErrors;
}

//------------------------------------------------------------------------------
/**
*/
inline const std::string& 
Compileable::GetError() const
{
	return this->error;
}

//------------------------------------------------------------------------------
/**
*/
inline void
Compileable::AddQualifier(const std::string& qualifier)
{
	this->qualifiers.push_back(qualifier);
}

//------------------------------------------------------------------------------
/**
*/
inline const size_t
Compileable::GetNumQualifiers() const
{
	return this->qualifiers.size();
}

//------------------------------------------------------------------------------
/**
*/
inline const std::string&
Compileable::GetQualifier(unsigned i) const
{
	return this->qualifiers[i];
}

//------------------------------------------------------------------------------
/**
*/
inline void
Compileable::AddQualifierExpression(const QualifierExpression& qualifier)
{
	this->qualifierExpressions.push_back(qualifier);
}

//------------------------------------------------------------------------------
/**
*/
inline const size_t
Compileable::GetNumQualifierExpressions() const
{
	return this->qualifierExpressions.size();
}

//------------------------------------------------------------------------------
/**
*/
inline const QualifierExpression&
Compileable::GetQualifierExpression(unsigned i) const
{
	return this->qualifierExpressions[i];
}
} // namespace AnyFX
//------------------------------------------------------------------------------
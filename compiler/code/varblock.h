#pragma once
//------------------------------------------------------------------------------
/**
    @class AnyFX::VarBlock
    
    A variable block describes an updatable set of variables.
    
    (C) 2013 Gustav Sterbrant
*/
//------------------------------------------------------------------------------
#include <string>
#include <vector>
#include "variable.h"
#include "symbol.h"
#include "annotation.h"
namespace AnyFX
{
class VarBlock : public Symbol
{
public:
	/// constructor
	VarBlock();
	/// destructor
	virtual ~VarBlock();

	/// set annotation
	void SetAnnotation(const Annotation& annotation);

	/// add variable
	void AddVariable(const Variable& var);
	/// get variables
	const std::vector<Variable>& GetVariables() const;
	/// set if varblock is shared by several shaders
	void SetShared(bool b);
	/// get if varblock is shared
	bool IsShared() const;

	/// sorts variables in varblock
	void SortVariables();

	/// type checks var block
	void TypeCheck(TypeChecker& typechecker);
	/// compiles var block
	void Compile(BinWriter& writer);

	/// format variable to fit target language
	std::string Format(const Header& header) const;
private:
	std::vector<Variable> variables;
	bool isShared;

	bool hasAnnotation;
	Annotation annotation;
}; 

//------------------------------------------------------------------------------
/**
*/
inline void 
VarBlock::SetShared( bool b )
{
	this->isShared = b;
}

//------------------------------------------------------------------------------
/**
*/
inline bool 
VarBlock::IsShared() const
{
	return this->isShared;
}

//------------------------------------------------------------------------------
/**
*/
inline const std::vector<Variable>& 
VarBlock::GetVariables() const
{
	return this->variables;
}

} // namespace AnyFX
//------------------------------------------------------------------------------

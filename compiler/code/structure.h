#pragma once
//------------------------------------------------------------------------------
/**
    @class Structure
    
    Implements user-defined types.
    
    (C) 2013 Gustav Sterbrant
*/
//------------------------------------------------------------------------------
#include <string>
#include <vector>
#include "parameter.h"
#include "symbol.h"
#include "variable.h"
namespace AnyFX
{

class Structure : public Symbol
{
public:
	/// constructor
	Structure();
	/// destructor
	virtual ~Structure();

	/// adds a parameter
	void AddParameter(const Parameter& param);
	/// gets parameters
	const std::vector<Parameter>& GetParameters() const;

    /// calculates the bit size of struct
    unsigned CalculateSize() const;

	/// format structure to fit target language
	std::string Format(const Header& header) const;
	/// unroll struct into list of variables (converts parameters to variables)
	void Unroll(const std::string& name, std::vector<Variable>& vars, TypeChecker& typechecker);

	/// type checks structure
	void TypeCheck(TypeChecker& typechecker);

private:
    /// check if structure contains type
    bool IsRecursive(TypeChecker& typeChecker);

	std::vector<Parameter> parameters;
}; 

} // namespace AnyFX

//------------------------------------------------------------------------------

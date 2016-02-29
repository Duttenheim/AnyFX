#pragma once
//------------------------------------------------------------------------------
/**
	A qualifier expression is a qualifier with a number attached to it. For example (slot = 5) where slot is the qualifier and 5 is the expression used
	
	(C) 2016 Individual contributors, see AUTHORS file
*/
//------------------------------------------------------------------------------
#include <string>
namespace AnyFX
{
class Expression;
struct QualifierExpression
{
	/// constructor
	QualifierExpression();
	/// destructor
	virtual ~QualifierExpression();

	std::string name;
	Expression* expr;
};

} // namespace AnyFX
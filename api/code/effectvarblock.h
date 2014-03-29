#pragma once
//------------------------------------------------------------------------------
/**
    @class AnyFX::EffectVarblock
    
    The EffectVarblock couples several variables together, which is managed, updated and can be shared by several effects.
	Varblocks can be used to optimize variables updates such as View and Projection matrices.
	In other words, the EffectVarblock acts as a potentially reusable buffer.
    
    (C) 2013 Gustav Sterbrant
*/
//------------------------------------------------------------------------------

#ifdef AFX_API
#error "afxapi.h included before effectvarblock.h"
#endif

#include <string>
#include "annotable.h"
namespace AnyFX
{
class InternalEffectVarblock;
class EffectVarblock : public Annotable
{
public:
	/// constructor
	EffectVarblock();
	/// destructor
	virtual ~EffectVarblock();

	/// commits varblock
	void Commit();
	/// returns name of varblock
	const std::string& GetName() const;

	/// discard varblock
	void Discard();

private:
	friend class EffectVarblockStreamLoader;
	friend class InternalEffectProgram;

	InternalEffectVarblock* internalVarblock;
}; 

} // namespace AnyFX
//------------------------------------------------------------------------------
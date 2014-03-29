#pragma once
//------------------------------------------------------------------------------
/**
    @class AnyFX::InternalEffectVarblock
    
    EffectVarblock backend, inherit this class to provide an implementation
    
    (C) 2013 Gustav Sterbrant
*/
//------------------------------------------------------------------------------
#include <string>
#include <vector>
namespace AnyFX
{
class Effect;
class InternalEffectVariable;
class InternalEffectProgram;
class InternalEffectVarblock 
{
public:
	/// constructor
	InternalEffectVarblock();
	/// destructor
	virtual ~InternalEffectVarblock();

	bool isDirty;

private:
	/// creates signature which is used for shared varblocks
	void SetupSignature();
	/// returns name of varblock
	const std::string& GetName() const;
	/// returns signature
	const std::string& GetSignature() const;

	/// decrease reference count, delete if 0
	void Release();
	/// increase reference count
	void Retain();

protected:
	friend class EffectVarblock;
	friend class EffectStreamLoader;
	friend class InternalEffectVariable;
	friend class InternalEffectProgram;
	friend class EffectVarblockStreamLoader;	

	/// sets up varblock from program, override in subclass
	virtual void Setup(std::vector<InternalEffectProgram*> programs);
	/// sets up varblock from programs using a pre-existing varblock
	virtual void SetupSlave(std::vector<InternalEffectProgram*> programs, InternalEffectVarblock* master);

	/// binds varblocks prior to updating
	virtual void Apply();
	/// updates varblocks back-end buffer
	virtual void Commit();
	
	/// updates single variable
	void SetVariable(InternalEffectVariable* var, void* value);
	/// updates variable array
	void SetVariableArray(InternalEffectVariable* var, void* value, size_t size);
	/// activates variable, this makes the uniform location be the one found in the given program
	virtual void Activate(InternalEffectProgram* program);

	std::string name;
	std::string signature;
	std::vector<InternalEffectVariable*> variables;
	InternalEffectVarblock* masterBlock;
	bool isShared;
	bool isSlave;
	bool active;
	int refCount;

	static unsigned globalUniformBlockBinding;

	struct InternalVarblockData
	{
		char* data;
		unsigned size;
	}* dataBlock;

	/// returns a pointer to the internal varblock data storage
	virtual InternalVarblockData* GetData();
}; 

//------------------------------------------------------------------------------
/**
*/
inline const std::string& 
InternalEffectVarblock::GetName() const
{
	return this->name;
}

//------------------------------------------------------------------------------
/**
*/
inline const std::string& 
InternalEffectVarblock::GetSignature() const
{
	return this->signature;
}

//------------------------------------------------------------------------------
/**
*/
inline InternalEffectVarblock::InternalVarblockData* 
InternalEffectVarblock::GetData()
{
	return this->dataBlock;
}

//------------------------------------------------------------------------------
/**
*/
inline void 
InternalEffectVarblock::Release()
{
	this->refCount--;
	if (this->refCount == 0)
	{
		delete this;
	}
}

//------------------------------------------------------------------------------
/**
*/
inline void 
InternalEffectVarblock::Retain()
{
	this->refCount++;
}

} // namespace AnyFX
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//  annotable.cc
//  (C) 2013 Gustav Sterbrant
//------------------------------------------------------------------------------
#include "annotable.h"
#include <assert.h>

namespace AnyFX
{

//------------------------------------------------------------------------------
/**
*/
Annotable::Annotable()
{
	this->annotationMap.clear();
}

//------------------------------------------------------------------------------
/**
*/
Annotable::~Annotable()
{
	// clear all strings
    eastl::map<eastl::string, AnnotationVariant>::iterator it = this->annotationMap.begin();
	for (it = this->annotationMap.begin(); it != this->annotationMap.end(); it++)
	{
		if (it->second.type == String)
		{
			delete it->second.data.stringValue;
		}
	}
}

//------------------------------------------------------------------------------
/**
*/
bool
Annotable::HasAnnotation(const eastl::string& name) const
{
    return this->annotationMap.find(name) != this->annotationMap.end();
}

//------------------------------------------------------------------------------
/**
*/
bool
Annotable::Flag(const eastl::string& name) const
{
	if (this->HasAnnotation(name)) return this->GetAnnotationBool(name);
	return false;
}

//------------------------------------------------------------------------------
/**
*/
int 
Annotable::GetAnnotationInt(const eastl::string& name) const
{	
	eastl::map<eastl::string, AnnotationVariant>::const_iterator it = this->annotationMap.find(name);
	assert(it != this->annotationMap.end());
	return it->second.data.intValue;
}

//------------------------------------------------------------------------------
/**
*/
bool 
Annotable::GetAnnotationBool(const eastl::string& name) const
{
	eastl::map<eastl::string, AnnotationVariant>::const_iterator it = this->annotationMap.find(name);
	assert(it != this->annotationMap.end());
	return it->second.data.boolValue;
}

//------------------------------------------------------------------------------
/**
*/
double 
Annotable::GetAnnotationDouble(const eastl::string& name) const
{
	eastl::map<eastl::string, AnnotationVariant>::const_iterator it = this->annotationMap.find(name);
	assert(it != this->annotationMap.end());
	return it->second.data.doubleValue;
}

//------------------------------------------------------------------------------
/**
*/
float 
Annotable::GetAnnotationFloat(const eastl::string& name) const
{
	eastl::map<eastl::string, AnnotationVariant>::const_iterator it = this->annotationMap.find(name);
	assert(it != this->annotationMap.end());
	return it->second.data.floatValue;
}

//------------------------------------------------------------------------------
/**
*/
const eastl::string&
Annotable::GetAnnotationString(const eastl::string& name) const
{
	eastl::map<eastl::string, AnnotationVariant>::const_iterator it = this->annotationMap.find(name);
	assert(it != this->annotationMap.end());
	return *it->second.data.stringValue;
}
} // namespace AnyFX
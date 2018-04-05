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
    std::map<std::string, AnnotationVariant>::iterator it = this->annotationMap.begin();
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
Annotable::HasAnnotation(const std::string& name) const
{
    return this->annotationMap.find(name) != this->annotationMap.end();
}

//------------------------------------------------------------------------------
/**
*/
bool
Annotable::Flag(const std::string& name) const
{
	if (this->HasAnnotation(name)) return this->GetAnnotationBool(name);
	return false;
}

//------------------------------------------------------------------------------
/**
*/
int 
Annotable::GetAnnotationInt(const std::string& name) const
{	
	std::map<std::string, AnnotationVariant>::const_iterator it = this->annotationMap.find(name);
	assert(it != this->annotationMap.end());
	return it->second.data.intValue;
}

//------------------------------------------------------------------------------
/**
*/
bool 
Annotable::GetAnnotationBool(const std::string& name) const
{
	std::map<std::string, AnnotationVariant>::const_iterator it = this->annotationMap.find(name);
	assert(it != this->annotationMap.end());
	return it->second.data.boolValue;
}

//------------------------------------------------------------------------------
/**
*/
double 
Annotable::GetAnnotationDouble(const std::string& name) const
{
	std::map<std::string, AnnotationVariant>::const_iterator it = this->annotationMap.find(name);
	assert(it != this->annotationMap.end());
	return it->second.data.doubleValue;
}

//------------------------------------------------------------------------------
/**
*/
float 
Annotable::GetAnnotationFloat(const std::string& name) const
{
	std::map<std::string, AnnotationVariant>::const_iterator it = this->annotationMap.find(name);
	assert(it != this->annotationMap.end());
	return it->second.data.floatValue;
}

//------------------------------------------------------------------------------
/**
*/
const std::string&
Annotable::GetAnnotationString(const std::string& name) const
{
	std::map<std::string, AnnotationVariant>::const_iterator it = this->annotationMap.find(name);
	assert(it != this->annotationMap.end());
	return *it->second.data.stringValue;
}
} // namespace AnyFX
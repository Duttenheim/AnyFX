#pragma once
//------------------------------------------------------------------------------
/**
    @class AnyFX::Annotable
    
    Classes who inherit from annotable can have annotations.
    
    (C) 2013 Gustav Sterbrant
*/
//------------------------------------------------------------------------------
#include "vartypes.h"
#include <string>
#include <map>
#include <vector>
namespace AnyFX
{
class Annotable
{
public:
	/// constructor
	Annotable();
	/// destructor
	virtual ~Annotable();

    /// returns true if annotation exists
    bool HasAnnotation(const std::string& name) const;
	/// returns true if annotation is bool, exists and is true
	bool Flag(const std::string& name) const;

	/// get int value
    int GetAnnotationInt(const std::string& name) const;
	/// get bool value
    bool GetAnnotationBool(const std::string& name) const;
	/// get double value
    double GetAnnotationDouble(const std::string& name) const;
	/// get float value
    float GetAnnotationFloat(const std::string& name) const;
	/// get string value
    const std::string& GetAnnotationString(const std::string& name) const;

private:
	friend class EffectAnnotationStreamLoader;
	friend class AnnotationLoader;

	struct AnnotationVariant
	{
		union
		{
			int intValue;
			bool boolValue;
			double doubleValue;
			float floatValue;
            std::string* stringValue;
		} data;

		VariableType type;
	};

	std::vector<VariableType> annotationTypes;
    std::map<std::string, AnnotationVariant> annotationMap;
}; 
} // namespace AnyFX
//------------------------------------------------------------------------------
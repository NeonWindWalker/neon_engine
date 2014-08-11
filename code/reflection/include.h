#pragma once

#include "seed.h"
#include "type.h"
#include "reflection.h"
#include "values.h"
#include "constructable.h"

#include "library.h"
#include "ttype.h"
#include "treflection.h"
#include "makers.h"
#include "enum.h"
#include "class.h"
#include "field.h"
#include "method.h"
#include "property.h"
#include "array.h"
#include "hashArray.h"

namespace ReflectionLib
{
	class Object : public RefCountable, public IReflective
	{
	public:
		rl_abstract_class(ReflectionLib::Object)

		Object(){}
		Object(const Object& other){}
		void operator = (const Object& other){}
	};
}
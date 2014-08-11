#pragma once

#include "font.h"

namespace GUI
{

class Library
{
public:
	Font* addFont(ConstString name, ConstString partName, ConstBinaryData data);
	Font* getFont(ConstString font)const;

	void buildWidget(Reflection rl)const;
	boolean getLocalizedText(String& outStr, ConstString entryname)const;

	void loadTextLocalization(ConstBinaryData data);

protected:
	typedef HashArray< Pair<String, StrongPtr< Font> > > Fonts_t;
	Fonts_t fonts;

	typedef HashArray< Pair<String, String> > TextSet_t;
	TextSet_t texts;

	typedef HashMap< Pair<ReflectionLib::TypeId, String> > ReflectionScripts_t;
	ReflectionScripts_t reflectionScripts;
};

}
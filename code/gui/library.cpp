#include "library.h"
#include "widget.h"

namespace GUI
{

Font* Library::addFont(ConstString name, ConstString partName, ConstBinaryData data)
{
	boolean bIns;
	Fonts_t::Iterator it = fonts.insert(bIns, name);
	if(!it->val)
		it->val = new Font();
	it->val->loadPart(data, partName);
	return it->val;
}

Font* Library::getFont(ConstString font)const
{
	Fonts_t::ConstIterator it = fonts.find(font);
	if(it != fonts.end())
		return it->val;
	return 0;
}


void Library::buildWidget(Reflection rl)const
{
	//Reflection rl = rl.endcast();
	//reflectionScripts.find(rl.do type().name())
}

void Library::loadTextLocalization(ConstBinaryData data)
{
	texts.clear();
	Reflection rl = reflectionOf(texts);
	rl.deserialize(ConstString((const utf8*)data.first(), data.byteSize()));
}

boolean Library::getLocalizedText(String& outStr, ConstString entryname)const
{
	auto it = texts.find(entryname);
	if(it == texts.end())
		return false;
	outStr = it->val;
	return true;
}

}


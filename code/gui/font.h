#pragma once

#include "seed.h"

namespace GUI
{

class Font : public RefCountable
{
public:
	Font();

	struct CharInfo
	{
		uint16 u;
		uint16 v;
		uint8 du,dv;
		int8 dx,dy;
		int8 xa;
		uint8 channel;
		uint8 bit;
		uint8 image;
		uint8 fontHeight;
		uint width() const { return xa; }

		CharInfo()
		{
			u = v = 0;
			du = dv = dx = dy = xa = channel = bit = image = fontHeight = 0;
		}
	};

	struct ImageInfo
	{
		String name;
		Vector size, invSize;
		boolean isBitCompressed;
		boolean bitCompressedLenght;

		ImageInfo()
		{
			size.setZero();
			invSize.setZero();
			isBitCompressed = false;
			bitCompressedLenght = false;
		}
	};

	INLINE boolean valid()const
	{
		return !charInfoTable.empty();
	}

	INLINE uint characterHeight()const
	{
		return _characterHeight;
	}

	INLINE const CharInfo* charInfo(uint ch)const
	{
		CharInfoTable_t::ConstIterator it = charInfoTable.find(ch);
		if(it != charInfoTable.end())
			return &it->val;
		return 0;
	}

	INLINE int charactersKerning(uint ch0, uint ch1)
	{
		KerningTable_t::ConstIterator it = kerningTable.find(uint64(ch0) | (uint64(ch1) << 32));
		if(it != kerningTable.end())
			return it->val;
		return 0;
	}

	INLINE const ImageInfo& imageInfo(uint n)const
	{
		return _imageInfo[n];
	}

	INLINE uint stringLineLenght(ConstString str)
	{
		UnicodeReader ur(str);
		uint prevCh = 0;
		int lenght = 0;
		while(!ur.empty())
		{
			uint ch = ur.pop();
			const CharInfo* ci = charInfo(ch);
			if(ci)
				lenght += (ci->width() + charactersKerning(prevCh, ch)) * _characterHeight / ci->fontHeight;
			prevCh = ch;
		}
		Assert(lenght >= 0);
		return lenght;
	}

	INLINE Vector2i stringSize(ConstString str)
	{
		UnicodeReader ur(str);
		uint prevCh = 0;
		int lenght = 0;
		int height = characterHeight();
		int maxLenght = 0;
		while(!ur.empty())
		{
			if(ur.str[0] == '\n'){
				height += characterHeight();
				maximize(maxLenght, lenght);
				lenght = 0;
				ur.str = ur.str.afterLeftTrim(1);
				continue;
			}

			if(ur.str[0] == '\r'){
				maximize(maxLenght, lenght);
				lenght = 0;
				ur.str = ur.str.afterLeftTrim(1);
				continue;
			}

			uint ch = ur.pop();
			const CharInfo* ci = charInfo(ch);
			if(ci)
				lenght += (ci->width() + charactersKerning(prevCh, ch)) * _characterHeight / ci->fontHeight;
			prevCh = ch;
			maximize(maxLenght, lenght);
		}

		return Vector2i(maxLenght, height);
	}

	void loadPart(ConstBinaryData data, ConstString shortName, boolean yUp = true);

	uint _characterHeight;

protected:
	typedef HashTable< Pair<uint, CharInfo> > CharInfoTable_t;
	typedef HashTable< Pair<uint64, int> > KerningTable_t;
	CharInfoTable_t charInfoTable;
	KerningTable_t kerningTable;
	Array<ImageInfo> _imageInfo;
};

}
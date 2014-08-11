#include "font.h"
#include "widget.h"

namespace GUI
{

namespace BMF
{
#pragma pack(push,1)
	struct InfoBlock
	{
		uint8 blockTypeId;
		uint32 blockSize;
		uint16 fontSize;
		uint8 reserved:4;
		uint8 bold    :1;
		uint8 italic  :1;
		uint8 unicode :1;
		uint8 smooth  :1;
		uint8 charSet;
		uint16 stretchH;
		uint8 aa;
		uint8 paddingUp;
		uint8 paddingRight;
		uint8 paddingDown;
		uint8 paddingLeft;
		uint8 spacingHoriz;
		uint8 spacingVert;
		uint8 outline;
		char fontName[1];
	};

	struct CommonBlock
	{
		uint8 blockTypeId;
		uint32  blockSize;
		uint16 lineHeight;
		uint16 base;
		uint16 scaleW;
		uint16 scaleH;
		uint16 pages;
		uint8 packed;
		uint8 alphaChnl;
		uint8 redChnl;
		uint8 greenChnl;
		uint8 blueChnl;
	};

	struct PagesBlock
	{
		uint8 blockTypeId;
		uint32  blockSize;
		char pageNames[1];
	};

	struct CharsBlock
	{
		uint8 blockTypeId;
		uint32 blockSize;
		struct CharInfo
		{
			uint32  id;
			uint16  x;
			uint16  y;
			uint16  width;
			uint16  height;
			int16 xoffset;
			int16 yoffset;
			int16 xadvance;
			uint8  page;
			uint8  chnl;
		};
		CharInfo chars[1];
	};

	struct KerningPairsBlock
	{
		uint8 blockTypeId;
		uint32 blockSize;
		struct KerningPair
		{
			uint32 first;
			uint32 second;
			int16 amount;
		};
		KerningPair kerningPairs[1];
	};
#pragma pack(pop)
}

Font::Font()
{
	_characterHeight = 0;
}

void Font::loadPart(ConstBinaryData data, ConstString shortName, boolean yUp)
{
	const byte* d = (const byte*)data.first();

	if(data.byteSize() < 4 || d[0] != 'B' || d[1] != 'M' || d[2] != 'F')
		return;

	ImageInfo II;

	d += 4;
	const BMF::InfoBlock* infoBl = (const BMF::InfoBlock*)d;
	d += nativeValueFromStandardValue(infoBl->blockSize) + 5;
	const BMF::CommonBlock* commonBl = (const BMF::CommonBlock*)d;
	uint pages = nativeValueFromStandardValue(commonBl->pages);
	if(pages == 1)
	{
		II.isBitCompressed = false;
		II.bitCompressedLenght = 8;
	}
	else
	{
		II.isBitCompressed = true;
		II.bitCompressedLenght = 8 / pages;
	}
	d += nativeValueFromStandardValue(commonBl->blockSize) + 5;
	const BMF::PagesBlock* pageBl = (const BMF::PagesBlock*)d;
	d += nativeValueFromStandardValue(pageBl->blockSize) + 5;
	const BMF::CharsBlock* charsBl = (const BMF::CharsBlock*)d;
	const BMF::KerningPairsBlock* kerningBl = 0;
	if(d + nativeValueFromStandardValue(charsBl->blockSize) + 5 < data.end()){
		d += nativeValueFromStandardValue(charsBl->blockSize) + 5;
		kerningBl = (const BMF::KerningPairsBlock*)d;
	}

	uint fontHeight = nativeValueFromStandardValue(commonBl->lineHeight); 
	maximize(_characterHeight, fontHeight);
	II.name = shortName;
	II.size.x = nativeValueFromStandardValue(commonBl->scaleW);
	II.size.y = nativeValueFromStandardValue(commonBl->scaleH);
	II.invSize = Vector::getOne() / II.size;

	uint imageIndex;
	for(imageIndex = 0; imageIndex < _imageInfo.size(); ++imageIndex)
		if(_imageInfo[imageIndex].name ==  II.name)
			break;
	
	for(const BMF::CharsBlock::CharInfo* ch = charsBl->chars, *ch_e = ch + (nativeValueFromStandardValue(charsBl->blockSize)/sizeof(BMF::CharsBlock::CharInfo)); ch < ch_e; ch++)
	{
		CharInfo ci;
		if(yUp)
		{
			ci.u = nativeValueFromStandardValue(ch->x);
			ci.v = (uint16)((uint)nativeValueFromStandardValue(commonBl->scaleH) - (uint)nativeValueFromStandardValue(ch->y) - (uint)nativeValueFromStandardValue(ch->height));
			ci.du = (uint8)nativeValueFromStandardValue(ch->width);
			ci.dv = (uint8)nativeValueFromStandardValue(ch->height);
			ci.dx = (int8)nativeValueFromStandardValue(ch->xoffset);
			ci.dy = (int8)((int)nativeValueFromStandardValue(commonBl->lineHeight) - (int)nativeValueFromStandardValue(ch->height) - (int)nativeValueFromStandardValue(ch->yoffset));
			ci.xa = (int8)nativeValueFromStandardValue(ch->xadvance);
		}
		else
		{
			ci.u = nativeValueFromStandardValue(ch->x);
			ci.v = nativeValueFromStandardValue(ch->y);
			ci.du = (uint8)nativeValueFromStandardValue(ch->width);
			ci.dv = (uint8)nativeValueFromStandardValue(ch->height);
			ci.dx = (int8)nativeValueFromStandardValue(ch->xoffset);
			ci.dy = (int8)nativeValueFromStandardValue(ch->yoffset);
			ci.xa = (int8)nativeValueFromStandardValue(ch->xadvance);
		}
		switch(nativeValueFromStandardValue(ch->chnl))
		{
		case 1:
			ci.channel = 2;
			break;
		case 2:
			ci.channel = 1;
			break;
		case 4:
			ci.channel = 0;
			break;
		case 8:
			ci.channel = 3;
			break;
		}
		ci.bit = nativeValueFromStandardValue(ch->page) * II.bitCompressedLenght;
		ci.image = imageIndex;
		ci.fontHeight = fontHeight;
		
		boolean bIns;
		charInfoTable.insert(bIns, ch->id)->val = ci;
		//Assert(bIns);
	}

	if(kerningBl)
	{
		for(const BMF::KerningPairsBlock::KerningPair* k = kerningBl->kerningPairs, *k_e = k + (nativeValueFromStandardValue(kerningBl->blockSize)/sizeof(BMF::KerningPairsBlock::KerningPair)); k < k_e; k++)
		{
			uint64 key = uint64(nativeValueFromStandardValue(k->first)) | (uint64(nativeValueFromStandardValue(k->second)) << 32);
			boolean bIns;
			kerningTable.insert(bIns, key)->val = nativeValueFromStandardValue(k->amount);
			//Assert(bIns);
		}
	}

	if(imageIndex == _imageInfo.size())
		_imageInfo.push(II);
}

}
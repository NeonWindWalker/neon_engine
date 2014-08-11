#include "text.h"

namespace GUI
{

void Text::onUpdate(Scalar dt)
{
	if(localizationEntry.size()){
		Library* lib = getLibrary();
		if(lib)
			if(!lib->getLocalizedText(text, localizationEntry))
				text.clear();
	}
	Widget::onUpdate(dt);
}

void Text::onDraw(const ParentDrawInfo& pdi, IDrawer& render)
{
	Library* lib = getLibrary();
	if(!lib)
		return;

	Bound absChildBound;
	Bound b = childBound();
	b.min = b.min * pdi.scale + pdi.offset;
	b.max = b.max * pdi.scale + pdi.offset;
	if(!pdi.bound.intersection(absChildBound, b))
		return;

	Font* fnt = lib->getFont(font);
	if(!fnt)
		return;
	int settedTex = -1;

	UnicodeReader ur(text);
	Vector locsize = childBound().size();
	Vector locpos(Scalar(0), locsize.y - (fnt->characterHeight() + Scalar(0.5)) * fontScale);
	while(!ur.empty())
	{
		uint firstChar = ur.getFirst();
		if(firstChar == '\n'){
			locpos.y -= fnt->characterHeight() * fontScale;
			locpos.x = 0;
			ur.str = ur.str.afterLeftTrim(1);
			continue;
		}

		if(firstChar == '\r'){
			locpos.x = 0;
			ur.str = ur.str.afterLeftTrim(1);
			continue;
		}

		if(isSpace(firstChar)){
			const Font::CharInfo* ci = fnt->charInfo(firstChar);
			if(ci)
				locpos.x += fontScale * ci->width() * fnt->characterHeight() / ci->fontHeight;
			ur.str = ur.str.afterLeftTrim(1);
			continue;
		}

		const utf8* wordFirst = ur.str.first();
		const utf8* wordEnd = ur.str.first();
		while(!ur.empty())
		{
			ConstString prev = ur.str;
			if(isSpace(ur.pop())){
				ur.str = prev;
				break;
			}
			wordEnd = ur.str.first();
		}

		if(wrapping)
		{
			uint wordLenght = fnt->stringLineLenght(ConstString(wordFirst, wordEnd));
			if(locpos.x + (Scalar(wordLenght) - Scalar(0.5)) * fontScale > locsize.x && Scalar(wordLenght) * fontScale <= locsize.x){
				locpos.y -= fnt->characterHeight() * fontScale;
				locpos.x = 0;
			}
		}
		
		UnicodeReader u(ConstString(wordFirst, wordEnd));
		uint prevChar = 0;
		while(!u.empty())
		{
			uint ch = u.pop();
			const Font::CharInfo* ci = fnt->charInfo(ch);
			if(!ci)
				continue;
			Scalar scale = fontScale * fnt->characterHeight() / ci->fontHeight;
			const Font::ImageInfo& imgInfo = fnt->imageInfo(ci->image);
			locpos.x += Scalar(fnt->charactersKerning(prevChar, ch)) * scale;

			Vector p = position + locpos + Vector((Scalar)ci->dx - Scalar(0.5), (Scalar)ci->dy - Scalar(0.5)) * scale;
			Vector s((ci->du + 1) * scale, (ci->dv + 1) * scale);

			Bound v;
			v.min = p * pdi.scale + pdi.offset;
			v.max = (p + s) * pdi.scale + pdi.offset;
			Bound t(Scalar(ci->u) - Scalar(0.5), Scalar(ci->v) - Scalar(0.5), Scalar(ci->u + ci->du) + Scalar(0.5), Scalar(ci->v + ci->dv) + Scalar(0.5));

			t.min *= imgInfo.invSize;
			t.max *= imgInfo.invSize;

			Bound clipedV, clipedT;
			if(clip(clipedV, clipedT, absChildBound, v, t))
			{
				if(ci->image != settedTex){
					render.setTexture(imgInfo.name);
					settedTex = ci->image;
				}
				Vector s = clipedV.size();
				clipedV.min.x = floor(clipedV.min.x / scale) * scale + Scalar(0.5) * scale;
				clipedV.min.y = floor(clipedV.min.y / scale) * scale + Scalar(0.5) * scale;
				s.x = floor(Scalar(s.x / scale) + Scalar(0.5)) * scale;
				s.y = floor(Scalar(s.y / scale) + Scalar(0.5)) * scale;
				clipedV.max = clipedV.min + s;

				if((shadowShift.x || shadowShift.y) && shadowColor.w)
				{
					Scalar shiftX = fontScale * fnt->characterHeight();
					Bound v_Sh;
					v_Sh.min = v.min + shadowShift * shiftX * pdi.scale;
					v_Sh.max = v.max + shadowShift * shiftX * pdi.scale;

					Bound clipedV_Sh, clipedT_Sh;
					if(clip(clipedV_Sh, clipedT_Sh, absChildBound, v_Sh, t))
					{
						render.drawFontQuad(clipedV_Sh.min, clipedV_Sh.max, clipedT_Sh.min, clipedT_Sh.max, shadowColor, ci->channel, ci->bit, imgInfo.bitCompressedLenght);
					}
				}

				render.drawFontQuad(clipedV.min, clipedV.max, clipedT.min, clipedT.max, color, ci->channel, ci->bit, imgInfo.bitCompressedLenght);
			}
			
			locpos.x += Scalar(ci->width()) * scale;
			prevChar = ch;
		}
	}
	
}

void Text::updateLayout_FromParent(const Bound& parentBound)
{
	Widget::updateLayout_FromParent(parentBound);
	
	if((autoSizeX | autoSizeY | autoFontScaleByX | autoFontScaleByY) == 0 || wrapping)
		return;
	
	Library* lib = getLibrary();
	if(!lib)
		return;

	Font* fnt = lib->getFont(font);
	if(!fnt)
		return;

	screen->lenghts.tmpSet(parentBound.size());
	Vector maxSize(screen->lenghts.lenghts[maxSizeXType].x * maxSizeX, screen->lenghts.lenghts[maxSizeXType].y * maxSizeY);
	Vector marginLow, marginHi;
	margin.getLowAndHi(marginLow, marginHi, parentBound.size(), screen->lenghts);
	maxSize -= marginLow + marginHi;

	Vector2i stringSize = fnt->stringSize(text);
	stringSize.x += 2;
	stringSize.y += 2;
	
	Scalar maxFontScaleByX = -1;
	if(autoFontScaleByX)
	{
		if(autoSizeX && maxSizeXType)
			maxFontScaleByX = maxSize.x / Scalar(stringSize.x);
		else
			maxFontScaleByX = size.x / Scalar(stringSize.x);
	}

	Scalar maxFontScaleByY = -1;
	if(autoFontScaleByY)
	{
		uint h = autoFontScaleByYConsiderateNewLines ? stringSize.y : fnt->characterHeight();
		if(autoSizeY && maxSizeYType)
			maxFontScaleByY = maxSize.y / Scalar(h);
		else
			maxFontScaleByY = size.y / Scalar(h);
	}

	if(maxFontScaleByX > 0)
	{
		if(maxFontScaleByY > 0)
			fontScale = min(maxFontScaleByX, maxFontScaleByY);
		else
			fontScale = maxFontScaleByX;
	}
	else if(maxFontScaleByY > 0)
	{
		fontScale = maxFontScaleByY;
	}
	
	if(autoSizeX)
	{
		if(maxSizeXType)
			size.x = min(stringSize.x * fontScale, maxSize.x);
		else
			size.x = stringSize.x * fontScale;
	}

	if(autoSizeY)
	{
		if(maxSizeYType)
			size.y = min(stringSize.y * fontScale, maxSize.y);
		else
			size.y = stringSize.y * fontScale;
	}

	Widget::updateLayout_FromParent(parentBound);
}

}
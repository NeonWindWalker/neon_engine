#include "atlas.h"

namespace ImageLib
{

void AtlasCoordinatesGenerator::push( int w, int h )
{
	Frame frame = {w, h, 0};
	frames(frame)->count++;
}

inline uint next_pow2( uint k )
{
	k--;
	for (int i = 1; i < sizeof(uint) * 8; i <<= 1)
		k |= k >> i;
	return k + 1;
}

void AtlasCoordinatesGenerator::build()
{
	if(frames.empty())
		return;

	uint maxFrameSize = 1;
	uint square = 0; 
	for (uint f = 0; f < frames.size(); f++)
	{
		frames[f].w += padding;
		frames[f].h += padding;
		maximize(maxFrameSize, (uint)frames[f].w);
		maximize(maxFrameSize, (uint)frames[f].h);
		square += frames[f].w * frames[f].h;
	}
	maximize(maxFrameSize, (uint)sqrt((double)square * 0.5));
	maxFrameSize = Base::saturate(maxFrameSize);

	Array<Frame> framesSaved;
	foreach(e,  frames)
		framesSaved.push(*e);

	for(_atlasHeight = maxFrameSize; ; _atlasHeight *= 2)
		for(_atlasWidth = _atlasHeight; _atlasWidth <= _atlasHeight * 2; _atlasWidth *= 2)
		{
			for(uint i=0; i < places.size(); ++i)
				delete places[i].val;
			places.clear();

			frames.clear();
			foreach(e,  framesSaved)
				frames.push(*e);

			_atlasesCount = 0;
			testBuild();

			if(_atlasesCount == 1)
				return;
		}
}

void AtlasCoordinatesGenerator::testBuild()
{
	while (!frames.empty())
	{
		_atlasesCount ++;
		int lineY = 0;

		typedef SortedArray< Pair<int, int> > LossesMap;
		LossesMap curLoses, prevLoses;

		while (!frames.empty() && lineY < (int)_atlasHeight)
		{
			int lineHeight = max(frames[0].w, frames[0].h);

			if (lineY + lineHeight > (int)_atlasHeight)
			{
				lineHeight = min(frames[0].w, frames[0].h);
				if (lineY + lineHeight > (int)_atlasHeight)
				{
					lineHeight = _atlasHeight - lineY;
				}
			}

			int lineX = 0;
			while (lineX < (int)_atlasWidth)
			{
				Frame* bestFrame = NULL;
				int bestLoss = 1 << 30;
				int bestCount = 0;
				bool bestTwiddle = false;
				int bestExtraHeight = 0;

				for (uint f = 0; f < frames.size(); f++)
				{
					Frame& frame = frames[f];
					int lineSize = _atlasWidth - lineX;

					if (frame.h <= lineSize)
					{
						int extraHeight = 0;
						LossesMap::Iterator left = prevLoses.lessequal(lineX);
						LossesMap::Iterator right = prevLoses.greaterequal(lineX + frame.h - 1);

						if (left != prevLoses.begin())
						{
							if (right != prevLoses.end()) right = prevLoses.end() - 1;
							extraHeight = min(left->val, right->val);
							while (left != right)
							{
								--right;
								if (right->val < extraHeight)
									extraHeight = right->val;
							}
						}

						int effectiveHeight = lineHeight + extraHeight;
						if (frame.w <= effectiveHeight)
						{
							int count = min(effectiveHeight / frame.w, frame.count);
							int loss = effectiveHeight - count * frame.w;
							if (loss < bestLoss)
							{
								bestFrame = &frame;
								bestLoss = loss;
								bestCount = count;
								bestTwiddle = true;
								bestExtraHeight = extraHeight;
							}
						}
					}

					if (frame.w <= lineSize)
					{
						int extraHeight = 0;
						LossesMap::Iterator left = prevLoses.lessequal(lineX);
						LossesMap::Iterator right = prevLoses.greaterequal(lineX + frame.w - 1);

						if (left != prevLoses.begin())
						{
							if (right!= prevLoses.end()) right = prevLoses.end() - 1;
							extraHeight = min(left->val, right->val);
							while (left != right)
							{
								--right;
								if (right->val < extraHeight)
									extraHeight = right->val;
							}
						}

						int effectiveHeight = lineHeight + extraHeight;

						if (frame.h <= effectiveHeight)
						{
							int count = min(effectiveHeight / frame.h, frame.count);
							int loss = effectiveHeight - count * frame.h;
							if (loss <= bestLoss)
							{
								bestFrame = &frame;
								bestLoss = loss;
								bestCount = count;
								bestTwiddle = false;
								bestExtraHeight = extraHeight;
							}
						}
					}

					if (bestLoss == 0) break;
				}

				if (bestFrame == NULL) break;

				int side = bestTwiddle ? bestFrame->w : bestFrame->h;
				for (int i = 0; i < bestCount; i++)
				{
					Frame frame;
                    frame.w = bestFrame->w;
                    frame.h = bestFrame->h;
					Place place;
                    place.x = lineX;
                    place.y = lineY - bestExtraHeight + side * i;
                    place.twiddle = bestTwiddle;
                    place.atlasId = _atlasesCount - 1;
					if (padding)
					{
						frame.h -= padding, frame.w -= padding;
						place.x += padding / 2;
						place.y += padding / 2;
					}

					//places(frame).item->value.insert(place, 0);
					boolean bNew;
					FramePlaces::Iterator it = places.insert(bNew,frame);
					if(bNew)
						it->val = new Array<Place>();
					it->val->insert(0, place);
					bestFrame->count--;
				}

				int offsetX = bestTwiddle ? bestFrame->h : bestFrame->w;

				curLoses(lineX)->val = bestLoss;
				curLoses(lineX + offsetX - 1)->val = bestLoss;

				lineX += offsetX;

				if (bestFrame->count == 0)
					frames.erase(bestFrame);
			}

			if (!curLoses.empty()) 
				curLoses(curLoses.last()->key + 1)->val = lineHeight;

			//prevLoses = curLoses;
			//curLoses.swap(prevLoses);
			//curLoses.resize(0);
			move(prevLoses, curLoses);
			curLoses.clear();

			lineY += lineHeight;
		}
	}
}

AtlasCoordinatesGenerator::Place AtlasCoordinatesGenerator::pop(int w, int h)
{
	Frame frame = {w, h};
	FramePlaces::Iterator record = places.find(frame);
	Assert(record != places.end());
	Place place;
	record->val->pop(place);
	if (record->val->empty()){
		delete record->val;
		places.erase(record);
	}
		
	return place;
}

void AtlasCoordinatesGenerator::clear()
{
	frames.clear();
	for(uint i=0; i < places.size(); ++i)
		delete places[i].val;
	places.clear();
}

}

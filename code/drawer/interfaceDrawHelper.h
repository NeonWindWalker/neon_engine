#pragma once

#include "batch.h"
#include "resources.h"

namespace DrawerLib
{
	
/* InterfaceDrawHelper - временный контейнер батчей для интерфейса и линий */
	
class InterfaceDrawHelper
{
protected:
	TempAllocator allocators[2];
	TempAllocator* allocator;
	TempAllocator* allocatorOld;
	
	Range<InterfaceQuadVertexFormat> quadsData;
	DrawerLib::IVertexBuffer* quadsVB; 
	uint quadsVBOffset;
	Range<InterfaceFontVertexFormat> fontsData;
	DrawerLib::IVertexBuffer* fontsVB; 
	uint fontsVBOffset;
	
	static const uint batchQuadsCount = 256;
	
	PointsBatch* pointsBatch;
	SimplePrimitiveVertexFormat* pointsCurrentDataPtr;
	static const uint maxPointsCount = 256;
	
	LinesBatch* linesBatch;
	SimplePrimitiveVertexFormat* linesCurrentDataPtr;
	static const uint maxLinesCount = 256;
	
	TrianglesBatch* trianglesBatch;
	SimplePrimitiveVertexFormat* trianglesCurrentDataPtr;
	static const uint maxTrianglesCount = 256;
	
	Array<BatchBase*> batches;
	Array<BatchBase*> primitiveBatches;
	
public:
	ITempDataBuffer* dataBuffer;
	DrawTaskParams* drawTaskParams;

	static ITexture* createSimpleTextTexture(IDevice& dev);
	
	InterfaceDrawHelper();
	void submit(DrawTask& drawTask);
	
	void quads(const ConstRange< InterfaceQuadVertexFormat> data, ITexture& texture, ITexture* alphaTexture = 0);
	void fontQuad(const ConstRange< InterfaceFontVertexFormat> data, ITexture& texture, EFontGlyphsMode mode);
	void breakQuadsBatch(DrawTask& drawTask);
	void points(const ConstRange< SimplePrimitiveVertexFormat> data);
	void lines(const ConstRange< SimplePrimitiveVertexFormat> data);
	void triangles(const ConstRange< SimplePrimitiveVertexFormat> data);
	
	
	void quad(
		const Vector2f& vA, const Vector2f& tA, const Vector4& cA,
		const Vector2f& vB, const Vector2f& tB, const Vector4& cB,
		const Vector2f& vC, const Vector2f& tC, const Vector4& cC,
		const Vector2f& vD, const Vector2f& tD, const Vector4& cD,
		ITexture& texture, ITexture* alphaTexture = 0)
	{
		InterfaceQuadVertexFormat v[4];
		v[0].position = vA;
		v[0].texcoord = tA;
		v[0].color = cA;
		v[1].position = vB;
		v[1].texcoord = tB;
		v[1].color = cB;
		v[2].position = vC;
		v[2].texcoord = tC;
		v[2].color = cC;
		v[3].position = vD;
		v[3].texcoord = tD;
		v[3].color = cD;
		quads(makeConstRange(v,4), texture, alphaTexture);
	}

	void fontQuad(
		const Vector2f& vA, const Vector2f& tA, const Vector4& cA,
		const Vector2f& vB, const Vector2f& tB, const Vector4& cB,
		const Vector2f& vC, const Vector2f& tC, const Vector4& cC,
		const Vector2f& vD, const Vector2f& tD, const Vector4& cD,
		ITexture& texture, uint byteNumber, uint bitNumber, uint bitLenght)
	{
		InterfaceFontVertexFormat v[4];
		v[0].position = vA;
		v[0].texcoord = tA;
		v[0].color = cA;
		v[0].byteNumber = byteNumber;
		v[0].bitNumber = bitNumber;
		v[0].bitLenght = bitLenght;
		v[1].position = vB;
		v[1].texcoord = tB;
		v[1].color = cB;
		v[1].byteNumber = byteNumber;
		v[1].bitNumber = bitNumber;
		v[1].bitLenght = bitLenght;
		v[2].position = vC;
		v[2].texcoord = tC;
		v[2].color = cC;
		v[2].byteNumber = byteNumber;
		v[2].bitNumber = bitNumber;
		v[2].bitLenght = bitLenght;
		v[3].position = vD;
		v[3].texcoord = tD;
		v[3].color = cD;
		v[3].byteNumber = byteNumber;
		v[3].bitNumber = bitNumber;
		v[3].bitLenght = bitLenght;
		fontQuad(makeConstRange(v,4), texture, bitLenght == 8 ? EFontGlyphsMode_SingleChannel : (bitLenght == 1 ? EFontGlyphsMode_BitCompressedMultisampled : EFontGlyphsMode_BitCompressed) );
	}

	INLINE void quad(
		const Vector2f& vA, const Vector2f& tA,
		const Vector2f& vB, const Vector2f& tB,
		const Vector2f& vC, const Vector2f& tC,
		const Vector2f& vD, const Vector2f& tD, 
		const Vector4f& color,
		ITexture& texture, ITexture* alphaTexture = 0)
	{
		quad(vA, tA, color, vB, tB, color, vC, tC, color, vD, tD, color, texture, alphaTexture);
	}

	INLINE void fontQuad(
		const Vector2f& vA, const Vector2f& tA,
		const Vector2f& vB, const Vector2f& tB,
		const Vector2f& vC, const Vector2f& tC,
		const Vector2f& vD, const Vector2f& tD, 
		const Vector4f& color,
		ITexture& texture, uint byteNumber, uint bitNumber, uint bitLenght)
	{
		fontQuad(vA, tA, color, vB, tB, color, vC, tC, color, vD, tD, color, texture, byteNumber, bitNumber, bitLenght);
	}
	
	INLINE void quad(
		const Vector2f& vMin, const Vector2f& vMax,
		const Vector2f& tMin, const Vector2f& tMax,
		const Vector4f& color,
		ITexture& texture, ITexture* alphaTexture = 0)
	{
		quad(
			vMin, tMin,
			Vector2f(vMax.x, vMin.y), Vector2f(tMax.x, tMin.y),
			Vector2f(vMin.x, vMax.y), Vector2f(tMin.x, tMax.y),
			vMax, tMax,
			color, texture, alphaTexture);
	}

	INLINE void fontQuad(
		const Vector2f& vMin, const Vector2f& vMax,
		const Vector2f& tMin, const Vector2f& tMax,
		const Vector4f& color,
		ITexture& texture, uint byteNumber, uint bitNumber, uint bitLenght)
	{
		fontQuad(
			vMin, tMin,
			Vector2f(vMax.x, vMin.y), Vector2f(tMax.x, tMin.y),
			Vector2f(vMin.x, vMax.y), Vector2f(tMin.x, tMax.y),
			vMax, tMax,
			color, texture, byteNumber, bitNumber, bitLenght);
	}
	
	INLINE void quad(
		const Vector2f& vMin, const Vector2f& vMax,
		const Vector4f& color,
		ITexture& texture, ITexture* alphaTexture = 0)
	{
		quad(
			vMin, Vector2f(0.0f, 0.0f),
			Vector2f(vMax.x, vMin.y), Vector2f(1.0f, 0.0f),
			Vector2f(vMin.x, vMax.y), Vector2f(0.0f, 1.0f),
			vMax, Vector2f(1.0f, 1.0f),
			color, texture, alphaTexture);
	}

	INLINE void fontQuad(
		const Vector2f& vMin, const Vector2f& vMax,
		const Vector4f& color,
		ITexture& texture, uint byteNumber, uint bitNumber, uint bitLenght)
	{
		fontQuad(
			vMin, Vector2f(0.0f, 0.0f),
			Vector2f(vMax.x, vMin.y), Vector2f(1.0f, 0.0f),
			Vector2f(vMin.x, vMax.y), Vector2f(0.0f, 1.0f),
			vMax, Vector2f(1.0f, 1.0f),
			color, texture, byteNumber, bitNumber, bitLenght);
	}
	
	void initSimpleTextTexture(ITexture* texture);
	void text(ITexture& simpleTextTexture, const Vector2f& pos, ConstString str, const Vector4& color, boolean yUp = true);
	
	INLINE void text(ITexture& simpleTextTexture, const Vector2f& pos, ConstString str, boolean yUp = true)
	{
		text(simpleTextTexture, pos, str, Vector4(1,1,1,1), yUp);
	}
	
	
	INLINE void triangle(const Vector2_t& a, const ColorRGBA ca, const Vector2_t& b, const ColorRGBA cb, const Vector2_t& c, const ColorRGBA cc)
	{
		SimplePrimitiveVertexFormat v[3];
		v[0].position = a;
		v[0].color = ca;
		v[1].position = b;
		v[1].color = cb;
		v[2].position = c;
		v[2].color = cc;
		triangles(ConstRange< SimplePrimitiveVertexFormat>(v, 3));
	}
	
	INLINE void line(const Vector2_t& a, const ColorRGBA ca, const Vector2_t& b, const ColorRGBA cb)
	{
		SimplePrimitiveVertexFormat v[2];
		v[0].position = a;
		v[0].color = ca;
		v[1].position = b;
		v[1].color = cb;
		lines(ConstRange< SimplePrimitiveVertexFormat>(v, 2));
	}

	void matrix(const Matrix34& mat);
	void lineBox(const Matrix34& mat, const ColorRGBA c);
	void box(const Matrix34& mat, const ColorRGBA c);
};


	
}

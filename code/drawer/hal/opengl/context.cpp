#include "context.h"
#include "resources.h"

#if defined(DRAWER_API_OPENGL) || defined(DRAWER_API_OPENGL_ES2)

namespace DrawerLib
{ 

Context::Context(HALDevice& dev) : basedevice(dev), _glFBO(0), _FBOActive(false)
{
	_renderTargetSize.set(0,0);
	_fullViewport = true;
	_viewportSize.setZero();
	_viewportPosition.setZero();
	defaultFilterTrilinear = false;
	defaultFilterAnisotropy = 0;
	clearCache();
}

Context::~Context()
{
#ifdef DRAWER_API_OPENGL
	if(_glFBO)
		glDeleteFramebuffersEXT(1, &_glFBO);
#else
	if(_glFBO)
		glDeleteFramebuffers(1, &_glFBO);
#endif
}


void Context::setRenderTargetWindowMode(FrameWindow& wnd)
{
#ifdef DRAWER_API_OPENGL
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
#else
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
#endif
    
    
#ifdef __APPLE__
    _FBOActive = false;
	_renderTargetSize = wnd.size();
    glBindFramebuffer(GL_FRAMEBUFFER, wnd.getFBO());
#else
    _FBOActive = false;
	_renderTargetSize = wnd.size();
	wnd.makeCurrentContext();
#endif
}

void Context::setRenderTargetCompositMode()
{
	basedevice.makeCurrentContext();

#ifdef DRAWER_API_OPENGL
	if(!_glFBO && glGenFramebuffersEXT)
		glGenFramebuffersEXT(1, &_glFBO);
#else
	if(!_glFBO)
		glGenFramebuffers(1, &_glFBO);
#endif

	if(!_glFBO)
		return;

#ifdef DRAWER_API_OPENGL
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, _glFBO);
#else
	glBindFramebuffer(GL_FRAMEBUFFER, _glFBO);
#endif

	_FBOActive = true;
}


void Context::setRenderTargetColor(uint n, TextureBase& tex, uint mip)
{
	if(!_FBOActive || !tex.renderable())
		return;
	_renderTargetSize.x = tex.width();
	_renderTargetSize.y = tex.height();
#ifdef DRAWER_API_OPENGL
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT + n, GL_TEXTURE_2D, tex.glTex(), mip);
#else
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + n, GL_TEXTURE_2D, tex.glTex(), mip);
#endif
}

void Context::setRenderTargetDepth(TextureBase& tex, uint mip)
{
	if(!_FBOActive || !tex.depthRenderable())
		return;
	_renderTargetSize.x = tex.width();
	_renderTargetSize.y = tex.height();
#ifdef DRAWER_API_OPENGL
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_TEXTURE_2D, tex.glTex(), mip);
#else
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, tex.glTex(), mip);
#endif
}

void Context::setRenderTargetDepth(RenderBufferDepthStensil& buf)
{
	if(!_FBOActive)
		return;
	_renderTargetSize.x = buf.width();
	_renderTargetSize.y = buf.height();
#ifdef DRAWER_API_OPENGL
	glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, buf.glRBO());
#else
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, buf.glRBO());
#endif
}

void Context::viewportFull()
{
	 glViewport(0,0, _renderTargetSize.x, _renderTargetSize.y);
	 _fullViewport = true;
	 _viewportPosition.setZero();
	 _viewportSize = _renderTargetSize;
}

void Context::viewport(uint x, uint y, uint width, uint height)
{
	 glViewport(x, y, width, height);
	 _viewportPosition.set(x, y);
	 _viewportSize.set(width, height);
	 _fullViewport = _viewportSize == _renderTargetSize && x == 0 && y == 0;
}

void Context::begin()
{
	glFrontFace(GL_CCW);
	clearCache();
}

void Context::clearBuffers(boolean clearDepth, Vector4 * color)
{
	if(!_fullViewport)
	{
		glEnable(GL_SCISSOR_TEST);
		glScissor(_viewportPosition.x, _viewportPosition.y, _viewportSize.x, _viewportSize.y);
	}

	if(clearDepth)
	{
		if(color)
		{
			glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
			glDepthMask(GL_TRUE);
			glClearColor(color->x, color->y, color->z, color->w);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}
		else
		{
			glDepthMask(GL_TRUE);
			glClear( GL_DEPTH_BUFFER_BIT);
		}
	}
	else
	{
		if(color)
		{
			glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
			glClearColor(color->x, color->y, color->z, color->w);
			glClear(GL_COLOR_BUFFER_BIT);
		}
		else
		{
		}
	}

	if(!_fullViewport)
	{
		glDisable(GL_SCISSOR_TEST);
	}
}

void Context::end()
{
	if(_FBOActive)
	{
#ifdef DRAWER_API_OPENGL
		for(uint i=0; i < 4; ++i){
			glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT + i, GL_RENDERBUFFER_EXT, 0);
			glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT + i, GL_TEXTURE_2D, 0, 0);
		}
		glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, 0);
		glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_TEXTURE_2D, 0, 0);
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
#else
		for(uint i=0; i < 4; ++i){
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_RENDERBUFFER, 0);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, 0, 0);
		}
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, 0, 0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
#endif
		_FBOActive = false;
	}

	basedevice.makeCurrentContext();
}

void Context::colorNo()
{
	glColorMask(GL_FALSE,GL_FALSE,GL_FALSE,GL_FALSE);
	glDisable(GL_BLEND);
}
void Context::blendNo()
{
	glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);
	glDisable(GL_BLEND);
}
void Context::blendAdd()
{
	glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);
}
void Context::blendAddXAlpha()
{
	glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
}
void Context::blendAlpha()
{
	glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}
void Context::blendXAlpha()
{
	glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
}

void Context::depthRange(float low, float hi)
{
#ifdef DRAWER_API_OPENGL
	glDepthRange(low, hi);
#else
	glDepthRangef(low, hi);
#endif

}

void Context::depthNo()
{
	glDepthMask(GL_FALSE);
	glDisable(GL_DEPTH_TEST);
}
void Context::depthLessEqual(boolean write)
{
	glEnable(GL_DEPTH_TEST);
	glDepthMask(write ? GL_TRUE : GL_FALSE);
	glDepthFunc(GL_LEQUAL);

}
void Context::depthGreaterEqual(boolean write)
{
	glEnable(GL_DEPTH_TEST);
	glDepthMask(write);
	glDepthFunc(GL_GEQUAL);
}


void Context::program(ShaderProgram& shader)
{
	if(_shaderProgram == &shader)
		return;

	shaderChangesCount ++;
	
#ifdef DRAWER_API_OPENGL
	glUseProgramObjectARB(shader.glProgram());
#else
	glUseProgram(shader.glProgram());
#endif
	
	_shaderProgram = &shader;

#ifdef DEBUG
	String errStr;
	if(isOpenGLError(errStr))
		logError << errStr;
#endif
}

void Context::setTextureFiltring(boolean trilinear, uint anisotropy)
{
	defaultFilterTrilinear = trilinear;
	defaultFilterAnisotropy = anisotropy;
}

void Context::texture(TextureNameIndexAndIndex index, TextureBase& tex, ETextureSetMode mode)
{
	if(index.deviceIndex < 0 || index.shaderIndex < 0)
		return;

#ifdef DRAWER_API_OPENGL
	glUniform1iARB(index.shaderIndex, index.deviceIndex);
#else
	glUniform1i(index.shaderIndex, index.deviceIndex);
#endif

	if(_bindedTextures[index.deviceIndex] == &tex && _bindedTextureModes[index.deviceIndex] == mode)
		return;

	textureChangesCount ++;
	_bindedTextures[index.deviceIndex] = &tex;
	_bindedTextureModes[index.deviceIndex] = mode;

#ifdef DRAWER_API_OPENGL
	glActiveTextureARB(GL_TEXTURE0 + index.deviceIndex);
#else
	glActiveTexture(GL_TEXTURE0 + index.deviceIndex);
#endif

	glBindTexture(GL_TEXTURE_2D, tex.glTex());

	switch(mode)
	{
	case ETextureSetMode_Default:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, tex.mips() > 1 ? (defaultFilterTrilinear ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR_MIPMAP_NEAREST) : GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		break;
	case ETextureSetMode_Particles:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, tex.mips() > 1 ? GL_LINEAR_MIPMAP_NEAREST : GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		break;
	case ETextureSetMode_DefaultGUI:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, tex.mips() > 1 ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		break;
	case ETextureSetMode_GUIFont:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, tex.mips() > 1 ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		break;
	case ETextureSetMode_GUIHardFont:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		break;
	case ETextureSetMode_ClampTablic:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, tex.mips() > 1 ? (defaultFilterTrilinear ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR_MIPMAP_NEAREST) : GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		break;
    default:
        Assert(false);
	}
	
#ifdef DEBUG
	String errStr;
	if(isOpenGLError(errStr))
		logError << errStr;
#endif
}

void Context::textureDisableFrom(uint n)
{
	
}

void Context::texture(uint n, TextureCube& tex)
{
	
}

void Context::draw(IVertexBuffer* ivb, EVertexFormat vbformat, uint vbByteOffset, uint vbSize, IIndexBuffer* iib, uint ibOffset, uint trisCount, boolean strip, boolean frontFace, boolean twosided)
{
	if(!ivb || !iib)
		return;

	drawsCount ++;

	VertexBuffer* vb = static_cast<VertexBuffer*>(ivb);
	IndexBuffer* ib = static_cast<IndexBuffer*>(iib);

	if(!vb->_glVBO || !ib->_glVBO)
		return;

	if(twosided)
        glDisable(GL_CULL_FACE);
    else
		glEnable(GL_CULL_FACE);
	
	glCullFace(frontFace ? GL_BACK : GL_FRONT);

#ifdef DRAWER_API_OPENGL
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, vb->_glVBO);
	glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, ib->_glVBO);
#else
	glBindBuffer(GL_ARRAY_BUFFER, vb->_glVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib->_glVBO);
#endif

	trianglesCount += trisCount;

	switch(vbformat)
	{
	case EVertexFormat_Mesh:
		for(uint i=0; i<5; ++i)
			my_glEnableVertexAttribArray(i);

		my_glVertexAttribPointer(0, 3, GL_FLOAT, 			false,	sizeof(MeshVertexFormat), vboStrideHack(vbByteOffset + MeshVertexFormat::position_offset));
		my_glVertexAttribPointer(1, 2, GL_SHORT,			true,	sizeof(MeshVertexFormat), vboStrideHack(vbByteOffset + MeshVertexFormat::texcoord_offset));
		my_glVertexAttribPointer(2, 4, GL_UNSIGNED_BYTE,	true,	sizeof(MeshVertexFormat), vboStrideHack(vbByteOffset + MeshVertexFormat::tangentAndUnocclusion_offset));
		my_glVertexAttribPointer(3, 4, GL_UNSIGNED_BYTE,	true,	sizeof(MeshVertexFormat), vboStrideHack(vbByteOffset + MeshVertexFormat::binormalAndOpacity_offset));
		my_glVertexAttribPointer(4, 4, GL_UNSIGNED_BYTE,	true,	sizeof(MeshVertexFormat), vboStrideHack(vbByteOffset + MeshVertexFormat::normalAndOpacity2_offset));

		if(strip)
			glDrawElements(GL_TRIANGLE_STRIP, trisCount + 2, GL_UNSIGNED_SHORT, vboStrideHack(ibOffset * 2));
		else
			glDrawElements(GL_TRIANGLES, trisCount * 3, GL_UNSIGNED_SHORT, vboStrideHack(ibOffset * 2));

		for(uint i=0; i<5; ++i)
			my_glDisableVertexAttribArray(i);
		break;
		
	case EVertexFormat_SkeletalMesh:
		for(uint i=0; i<7; ++i)
			my_glEnableVertexAttribArray(i);

		my_glVertexAttribPointer(0, 3, GL_FLOAT, 			false,	sizeof(SkeletalMeshVertexFormat), vboStrideHack(vbByteOffset + MeshVertexFormat::position_offset));
		my_glVertexAttribPointer(1, 2, GL_SHORT,			true,	sizeof(SkeletalMeshVertexFormat), vboStrideHack(vbByteOffset + MeshVertexFormat::texcoord_offset));
		my_glVertexAttribPointer(2, 4, GL_UNSIGNED_BYTE,	true,	sizeof(SkeletalMeshVertexFormat), vboStrideHack(vbByteOffset + MeshVertexFormat::tangentAndUnocclusion_offset));
		my_glVertexAttribPointer(3, 4, GL_UNSIGNED_BYTE,	true,	sizeof(SkeletalMeshVertexFormat), vboStrideHack(vbByteOffset + MeshVertexFormat::binormalAndOpacity_offset));
		my_glVertexAttribPointer(4, 4, GL_UNSIGNED_BYTE,	true,	sizeof(SkeletalMeshVertexFormat), vboStrideHack(vbByteOffset + MeshVertexFormat::normalAndOpacity2_offset));
		my_glVertexAttribPointer(5, 4, GL_UNSIGNED_BYTE,	false,	sizeof(SkeletalMeshVertexFormat), vboStrideHack(vbByteOffset + SkeletalMeshVertexFormat::boneIndexes_offset));
		my_glVertexAttribPointer(6, 4, GL_UNSIGNED_BYTE,	true,	sizeof(SkeletalMeshVertexFormat), vboStrideHack(vbByteOffset + SkeletalMeshVertexFormat::boneWeights_offset));

		if(strip)
			glDrawElements(GL_TRIANGLE_STRIP, trisCount + 2, GL_UNSIGNED_SHORT, vboStrideHack(ibOffset * 2));
		else
			glDrawElements(GL_TRIANGLES, trisCount * 3, GL_UNSIGNED_SHORT, vboStrideHack(ibOffset * 2));

		for(uint i=0; i<7; ++i)
			my_glDisableVertexAttribArray(i);
		break;
		
	case EVertexFormat_LightProbedMesh:
		for(uint i=0; i<7; ++i)
			my_glEnableVertexAttribArray(i);

		my_glVertexAttribPointer(0, 3, GL_FLOAT, 			false,	sizeof(LightProbedMeshVertexFormat), vboStrideHack(vbByteOffset + MeshVertexFormat::position_offset));
		my_glVertexAttribPointer(1, 2, GL_SHORT,			true,	sizeof(LightProbedMeshVertexFormat), vboStrideHack(vbByteOffset + MeshVertexFormat::texcoord_offset));
		my_glVertexAttribPointer(2, 4, GL_UNSIGNED_BYTE,	true,	sizeof(LightProbedMeshVertexFormat), vboStrideHack(vbByteOffset + MeshVertexFormat::tangentAndUnocclusion_offset));
		my_glVertexAttribPointer(3, 4, GL_UNSIGNED_BYTE,	true,	sizeof(LightProbedMeshVertexFormat), vboStrideHack(vbByteOffset + MeshVertexFormat::binormalAndOpacity_offset));
		my_glVertexAttribPointer(4, 4, GL_UNSIGNED_BYTE,	true,	sizeof(LightProbedMeshVertexFormat), vboStrideHack(vbByteOffset + MeshVertexFormat::normalAndOpacity2_offset));
		my_glVertexAttribPointer(5, 4, GL_UNSIGNED_BYTE,	false,	sizeof(LightProbedMeshVertexFormat), vboStrideHack(vbByteOffset + LightProbedMeshVertexFormat::lightProbeIndexes_offset));
		my_glVertexAttribPointer(6, 4, GL_UNSIGNED_BYTE,	true,	sizeof(LightProbedMeshVertexFormat), vboStrideHack(vbByteOffset + LightProbedMeshVertexFormat::lightProbeWeights_offset));

		if(strip)
			glDrawElements(GL_TRIANGLE_STRIP, trisCount + 2, GL_UNSIGNED_SHORT, vboStrideHack(ibOffset * 2));
		else
			glDrawElements(GL_TRIANGLES, trisCount * 3, GL_UNSIGNED_SHORT, vboStrideHack(ibOffset * 2));

		for(uint i=0; i<7; ++i)
			my_glDisableVertexAttribArray(i);
		break;
		
	case EVertexFormat_BuildinLightingMesh:
		for(uint i=0; i<8; ++i)
			my_glEnableVertexAttribArray(i);

		my_glVertexAttribPointer(0, 3, GL_FLOAT, 			false,	sizeof(BuildinLightingMeshVertexFormat), vboStrideHack(vbByteOffset + MeshVertexFormat::position_offset));
		my_glVertexAttribPointer(1, 2, GL_SHORT,			true,	sizeof(BuildinLightingMeshVertexFormat), vboStrideHack(vbByteOffset + MeshVertexFormat::texcoord_offset));
		my_glVertexAttribPointer(2, 4, GL_UNSIGNED_BYTE,	true,	sizeof(BuildinLightingMeshVertexFormat), vboStrideHack(vbByteOffset + MeshVertexFormat::tangentAndUnocclusion_offset));
		my_glVertexAttribPointer(3, 4, GL_UNSIGNED_BYTE,	true,	sizeof(BuildinLightingMeshVertexFormat), vboStrideHack(vbByteOffset + MeshVertexFormat::binormalAndOpacity_offset));
		my_glVertexAttribPointer(4, 4, GL_UNSIGNED_BYTE,	true,	sizeof(BuildinLightingMeshVertexFormat), vboStrideHack(vbByteOffset + MeshVertexFormat::normalAndOpacity2_offset));
		my_glVertexAttribPointer(5, 4, GL_SHORT,			true,	sizeof(BuildinLightingMeshVertexFormat), vboStrideHack(vbByteOffset + BuildinLightingMeshVertexFormat::lightVectorXLightIntensivityAndLightIntensivity_offset));
		my_glVertexAttribPointer(6, 4, GL_UNSIGNED_BYTE,	true,	sizeof(BuildinLightingMeshVertexFormat), vboStrideHack(vbByteOffset + BuildinLightingMeshVertexFormat::lightIntensivity_offset));
		my_glVertexAttribPointer(7, 4, GL_UNSIGNED_BYTE,	true,	sizeof(BuildinLightingMeshVertexFormat), vboStrideHack(vbByteOffset + BuildinLightingMeshVertexFormat::ambientIntensivity_offset));

		if(strip)
			glDrawElements(GL_TRIANGLE_STRIP, trisCount + 2, GL_UNSIGNED_SHORT, vboStrideHack(ibOffset * 2));
		else
			glDrawElements(GL_TRIANGLES, trisCount * 3, GL_UNSIGNED_SHORT, vboStrideHack(ibOffset * 2));

		for(uint i=0; i<8; ++i)
			my_glDisableVertexAttribArray(i);
		break;
		
	case EVertexFormat_HeightMap:
		for(uint i=0; i<5; ++i)
			my_glEnableVertexAttribArray(i);

		my_glVertexAttribPointer(0, 2, GL_SHORT, 			false,	sizeof(HeightMapVertexFormat), vboStrideHack(vbByteOffset + HeightMapVertexFormat::indexes_offset));
		my_glVertexAttribPointer(1, 1, GL_FLOAT, 			false,	sizeof(HeightMapVertexFormat), vboStrideHack(vbByteOffset + HeightMapVertexFormat::height_offset));
		my_glVertexAttribPointer(2, 2, GL_SHORT,			true,	sizeof(HeightMapVertexFormat), vboStrideHack(vbByteOffset + HeightMapVertexFormat::derivata_offset));
		my_glVertexAttribPointer(3, 4, GL_UNSIGNED_BYTE,	true,	sizeof(HeightMapVertexFormat), vboStrideHack(vbByteOffset + HeightMapVertexFormat::layers_offset));
		my_glVertexAttribPointer(4, 4, GL_UNSIGNED_BYTE,	true,	sizeof(HeightMapVertexFormat), vboStrideHack(vbByteOffset + HeightMapVertexFormat::layers_offset + 4));
		
		if(strip)
			glDrawElements(GL_TRIANGLE_STRIP, trisCount + 2, GL_UNSIGNED_SHORT, vboStrideHack(ibOffset * 2));
		else
			glDrawElements(GL_TRIANGLES, trisCount * 3, GL_UNSIGNED_SHORT, vboStrideHack(ibOffset * 2));

		for(uint i=0; i<5; ++i)
			my_glDisableVertexAttribArray(i);
		break;

	case EVertexFormat_Particle:
		for(uint i=0; i<6; ++i)
			my_glEnableVertexAttribArray(i);

		my_glVertexAttribPointer(0, 3, GL_FLOAT, 			false,	sizeof(ParticleVertexFormat), vboStrideHack(vbByteOffset + ParticleVertexFormat::position_offset));
		my_glVertexAttribPointer(1, 1, GL_FLOAT, 			false,	sizeof(ParticleVertexFormat), vboStrideHack(vbByteOffset + ParticleVertexFormat::depth_offset));
		my_glVertexAttribPointer(2, 4, GL_FLOAT,			false,	sizeof(ParticleVertexFormat), vboStrideHack(vbByteOffset + ParticleVertexFormat::color_offset));
		my_glVertexAttribPointer(3, 4, GL_SHORT,			true,	sizeof(ParticleVertexFormat), vboStrideHack(vbByteOffset + ParticleVertexFormat::texCoords_offset));
		my_glVertexAttribPointer(4, 1, GL_FLOAT,			false,	sizeof(ParticleVertexFormat), vboStrideHack(vbByteOffset + ParticleVertexFormat::hue_offset));
		my_glVertexAttribPointer(5, 4, GL_UNSIGNED_BYTE,	true,	sizeof(ParticleVertexFormat), vboStrideHack(vbByteOffset + ParticleVertexFormat::params_offset));

		if(strip)
			glDrawElements(GL_TRIANGLE_STRIP, trisCount + 2, GL_UNSIGNED_SHORT, vboStrideHack(ibOffset * 2));
		else
			glDrawElements(GL_TRIANGLES, trisCount * 3, GL_UNSIGNED_SHORT, vboStrideHack(ibOffset * 2));

		for(uint i=0; i<6; ++i)
			my_glDisableVertexAttribArray(i);
		break;

	case EVertexFormat_InterfaceQuad:
		for(uint i=0; i<3; ++i)
			my_glEnableVertexAttribArray(i);

		my_glVertexAttribPointer(0, 3, GL_FLOAT, 			false, sizeof(InterfaceQuadVertexFormat), vboStrideHack(vbByteOffset + InterfaceQuadVertexFormat::position_offset));
		my_glVertexAttribPointer(1, 2, GL_FLOAT,			false, sizeof(InterfaceQuadVertexFormat), vboStrideHack(vbByteOffset + InterfaceQuadVertexFormat::texcoord_offset));
		my_glVertexAttribPointer(2, 4, GL_FLOAT,			false, sizeof(InterfaceQuadVertexFormat), vboStrideHack(vbByteOffset + InterfaceQuadVertexFormat::color_offset));
		
		if(strip)
			glDrawElements(GL_TRIANGLE_STRIP, trisCount + 2, GL_UNSIGNED_SHORT, vboStrideHack(ibOffset * 2));
		else
			glDrawElements(GL_TRIANGLES, trisCount * 3, GL_UNSIGNED_SHORT, vboStrideHack(ibOffset * 2));

		for(uint i=0; i<3; ++i)
			my_glDisableVertexAttribArray(i);
		break;

	case EVertexFormat_InterfaceFont:
		for(uint i=0; i<4; ++i)
			my_glEnableVertexAttribArray(i);

		my_glVertexAttribPointer(0, 3, GL_FLOAT, 			false, sizeof(InterfaceFontVertexFormat), vboStrideHack(vbByteOffset + InterfaceFontVertexFormat::position_offset));
		my_glVertexAttribPointer(1, 2, GL_FLOAT,			false, sizeof(InterfaceFontVertexFormat), vboStrideHack(vbByteOffset + InterfaceFontVertexFormat::texcoord_offset));
		my_glVertexAttribPointer(2, 4, GL_FLOAT,			false, sizeof(InterfaceFontVertexFormat), vboStrideHack(vbByteOffset + InterfaceFontVertexFormat::color_offset));
		my_glVertexAttribPointer(3, 4, GL_UNSIGNED_BYTE,	false, sizeof(InterfaceFontVertexFormat), vboStrideHack(vbByteOffset + InterfaceFontVertexFormat::params_offset));

		if(strip)
			glDrawElements(GL_TRIANGLE_STRIP, trisCount + 2, GL_UNSIGNED_SHORT, vboStrideHack(ibOffset * 2));
		else
			glDrawElements(GL_TRIANGLES, trisCount * 3, GL_UNSIGNED_SHORT, vboStrideHack(ibOffset * 2));

		for(uint i=0; i<4; ++i)
			my_glDisableVertexAttribArray(i);
		break;
		
	case EVertexFormat_SimplePrimitive:
		for(uint i=0; i<2; ++i)
			my_glEnableVertexAttribArray(i);

		my_glVertexAttribPointer(0, 3, GL_FLOAT, 			false, sizeof(SimplePrimitiveVertexFormat), vboStrideHack(vbByteOffset + SimplePrimitiveVertexFormat::position_offset));
		my_glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE,	true, sizeof(SimplePrimitiveVertexFormat), vboStrideHack(vbByteOffset + SimplePrimitiveVertexFormat::color_offset));
		
		if(strip)
			glDrawElements(GL_TRIANGLE_STRIP, trisCount + 2, GL_UNSIGNED_SHORT, vboStrideHack(ibOffset * 2));
		else
			glDrawElements(GL_TRIANGLES, trisCount * 3, GL_UNSIGNED_SHORT, vboStrideHack(ibOffset * 2));

		for(uint i=0; i<2; ++i)
			my_glDisableVertexAttribArray(i);
		break;

	default:
		trianglesCount -= trisCount;
	}

#ifdef DRAWER_API_OPENGL
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
	glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, 0);
#else
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
#endif
}

void Context::drawQuads(IVertexBuffer* ivb, EVertexFormat vbformat, uint vbByteOffset, uint count)
{
	if(!ivb)
		return;
	
	uint s = formatSize(vbformat);
	uint maxQuads = basedevice.getQuadIndexBuffer()->count() / 6;
	while(count)
	{
		uint q = min(maxQuads, count);
		draw(ivb, vbformat, vbByteOffset, q * 4 * formatSize(vbformat), basedevice.getQuadIndexBuffer(), 0, q * 2, false, true, true);
		count -= q;
		vbByteOffset += q * 4 * s;
	}
}

void Context::drawSimplePoints(IVertexBuffer* ivb, uint vbByteOffset, uint count)
{
	if(!ivb)
		return;

	VertexBuffer* vb = static_cast<VertexBuffer*>(ivb);

	if(!vb->_glVBO)
		return;

#ifdef DRAWER_API_OPENGL
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, vb->_glVBO);
#else
	glBindBuffer(GL_ARRAY_BUFFER, vb->_glVBO);
#endif
	
	for(uint i=0; i<2; ++i)
		my_glEnableVertexAttribArray(i);

	my_glVertexAttribPointer(0, 3, GL_FLOAT, 			false, sizeof(SimplePrimitiveVertexFormat), vboStrideHack(vbByteOffset + SimplePrimitiveVertexFormat::position_offset));
	my_glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE,	true, sizeof(SimplePrimitiveVertexFormat), vboStrideHack(vbByteOffset + SimplePrimitiveVertexFormat::color_offset));

	glDrawArrays(GL_POINTS, 0, count);

	for(uint i=0; i<2; ++i)
		my_glDisableVertexAttribArray(i);
	
#ifdef DRAWER_API_OPENGL
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
#else
	glBindBuffer(GL_ARRAY_BUFFER, 0);
#endif
}

void Context::drawSimpleLines(IVertexBuffer* ivb, uint vbByteOffset, uint count)
{
	if(!ivb)
		return;

	VertexBuffer* vb = static_cast<VertexBuffer*>(ivb);

	if(!vb->_glVBO)
		return;

#ifdef DRAWER_API_OPENGL
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, vb->_glVBO);
#else
	glBindBuffer(GL_ARRAY_BUFFER, vb->_glVBO);
#endif
	
	for(uint i=0; i<2; ++i)
		my_glEnableVertexAttribArray(i);

	my_glVertexAttribPointer(0, 3, GL_FLOAT, 			false, sizeof(SimplePrimitiveVertexFormat), vboStrideHack(vbByteOffset + SimplePrimitiveVertexFormat::position_offset));
	my_glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE,	true, sizeof(SimplePrimitiveVertexFormat), vboStrideHack(vbByteOffset + SimplePrimitiveVertexFormat::color_offset));

	glDrawArrays(GL_LINES, 0, count * 2);

	for(uint i=0; i<2; ++i)
		my_glDisableVertexAttribArray(i);
	
#ifdef DRAWER_API_OPENGL
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
#else
	glBindBuffer(GL_ARRAY_BUFFER, 0);
#endif
}

void Context::drawSimpleTriangles(IVertexBuffer* ivb, uint vbByteOffset, uint count)
{
	if(!ivb)
		return;

	VertexBuffer* vb = static_cast<VertexBuffer*>(ivb);

	if(!vb->_glVBO)
		return;
	
	glDisable(GL_CULL_FACE);

#ifdef DRAWER_API_OPENGL
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, vb->_glVBO);
#else
	glBindBuffer(GL_ARRAY_BUFFER, vb->_glVBO);
#endif
	
	for(uint i=0; i<2; ++i)
		my_glEnableVertexAttribArray(i);

	my_glVertexAttribPointer(0, 3, GL_FLOAT, 			false, sizeof(SimplePrimitiveVertexFormat), vboStrideHack(vbByteOffset + SimplePrimitiveVertexFormat::position_offset));
	my_glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE,	true, sizeof(SimplePrimitiveVertexFormat), vboStrideHack(vbByteOffset + SimplePrimitiveVertexFormat::color_offset));

	glDrawArrays(GL_TRIANGLES, 0, count * 3);

	for(uint i=0; i<2; ++i)
		my_glDisableVertexAttribArray(i);
	
#ifdef DRAWER_API_OPENGL
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
#else
	glBindBuffer(GL_ARRAY_BUFFER, 0);
#endif
}


void Context::copyColorData(StrongPtr<ImageLib::BaseImage>& outImg)
{
	if(!_renderTargetSize.x || !_renderTargetSize.y){
		outImg = 0;
		return;
	}

	if(outImg && (outImg->width() != _renderTargetSize.x || outImg->height() != _renderTargetSize.y || outImg->colortype() != EColor_RGBA))
		outImg = 0;

	Image<ColorRGBA>* img = 0;
	if(outImg)
	{
		img = outImg->cast<ColorRGBA>();
	}
	else
	{
		img = new Image<ColorRGBA>(_renderTargetSize.x, _renderTargetSize.y);
		outImg = img;
	}

	if(!img){
		outImg = 0;
		return;
	}

	glReadPixels(0, 0, _renderTargetSize.x, _renderTargetSize.y, GL_RGBA, GL_UNSIGNED_BYTE, img->data());
}

boolean Context::copyColorData(GLuint pbo, EColor colorType)
{
#if defined(DRAWER_API_OPENGL) || defined(DRAWER_API_OPENGLES3)
	GLenum format, type;
	switch(colorType)
	{
	case EColor_RGBA:
		format = GL_BGRA_EXT;
		type = GL_UNSIGNED_BYTE;
		break;
	case EColor_RGB:
		format = GL_RGB;
		type = GL_UNSIGNED_BYTE;
		break;
	case EColor_RGBAF:
		format = GL_RGBA;
		type = GL_FLOAT;
		break;
	case EColor_RGBF:
		format = GL_RGB;
		type = GL_FLOAT;
		break;
	case EColor_RF:
		format = GL_R;
		type = GL_FLOAT;
		break;
	default:
		return false;
	}
	//glReadBuffer(GL_BACK);

#ifdef DRAWER_API_OPENGL
	glBindBufferARB(GL_PIXEL_PACK_BUFFER_ARB, pbo);
#else
	glBindBuffer(GL_PIXEL_PACK_BUFFER, pbo);
#endif

	glReadPixels(_viewportPosition.x, _viewportPosition.y, _viewportSize.x, _viewportSize.y, format, type, 0);

#ifdef DRAWER_API_OPENGL
	glBindBufferARB(GL_PIXEL_PACK_BUFFER_ARB, 0);
#else
	glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);
#endif

	return true;

#else
	return false;
#endif
	
}

}

#endif



#include "shader.h"

#if defined(DRAWER_API_OPENGL) || defined(DRAWER_API_OPENGL_ES2)

namespace DrawerLib
{ 

VertexShader::VertexShader(HALDevice& device) : lel(this),  dev(device), _glShader(0)
{}

PixelShader::PixelShader(HALDevice& device) : lel(this), dev(device), _glShader(0)
{}

ShaderProgram::ShaderProgram(HALDevice& device) : lel(this), dev(device), _glProgram(0)
{}

VertexShader::~VertexShader()
{
	if(_glShader){
#ifdef DRAWER_API_OPENGL
		glDeleteObjectARB(_glShader);
#else
		glDeleteShader(_glShader);
#endif
		_glShader = 0;
	}
}

PixelShader::~PixelShader()
{
	if(_glShader){
#ifdef DRAWER_API_OPENGL
		glDeleteObjectARB(_glShader);
#else
		glDeleteShader(_glShader);
#endif
		_glShader = 0;
	}
}

ShaderProgram::~ShaderProgram()
{
	if(_glProgram){
#ifdef DRAWER_API_OPENGL
		glDeleteObjectARB(_glProgram);
#else
		glDeleteProgram(_glProgram);
#endif
		_glProgram = 0;
	}
}


boolean VertexShader::init(ConstString code)
{
	if(_glShader){
#ifdef DRAWER_API_OPENGL
		glDeleteObjectARB(_glShader);
#else
		glDeleteShader(_glShader);
#endif
		_glShader = 0;
	}
	
#ifdef DRAWER_API_OPENGL
	_glShader = glCreateShaderObjectARB(GL_VERTEX_SHADER);
#else
	_glShader = glCreateShader(GL_VERTEX_SHADER);
#endif

	const GLchar* ptr = (const GLchar*)code.first();
	GLint length = code.size();

#ifdef DRAWER_API_OPENGL
	glShaderSourceARB(_glShader, 1, &ptr, &length);
	glCompileShaderARB(_glShader);
#else
	glShaderSource(_glShader, 1, &ptr, &length);
	glCompileShader(_glShader);
#endif
	
#ifdef DRAWER_API_OPENGL
	if(glGetShaderiv && glGetShaderInfoLog)
#endif
	{
		GLint result;
		glGetShaderiv(_glShader, GL_COMPILE_STATUS, &result);
		if(result == GL_FALSE)
		{
			/* get the shader info log */
			glGetShaderiv(_glShader, GL_INFO_LOG_LENGTH, &length);
			char *log = (char*)alloca(length);
			glGetShaderInfoLog(_glShader, length, &result, log);
			
			logError <<  "Compile Vertex Shader Error: " << ConstString((utf8*)log, length);

#ifdef DRAWER_API_OPENGL
			glDeleteObjectARB(_glShader);
#else
			glDeleteShader(_glShader);
#endif
			_glShader = 0;

			logError << code;

			//FILE* file = fopen("vs.txt", "wt");
			//fwrite(code.first(), code.byteSize(), 1, file);
			//fclose(file);

			return false;
		}
	}
	
	String errStr;
	if(isOpenGLError(errStr))
	{
		logError << errStr;
		return false;
	}
	return true;
}

boolean PixelShader::init(ConstString code)
{
	if(_glShader){
#ifdef DRAWER_API_OPENGL
		glDeleteObjectARB(_glShader);
#else
		glDeleteShader(_glShader);
#endif
		_glShader = 0;
	}
	
#ifdef DRAWER_API_OPENGL
	_glShader = glCreateShaderObjectARB(GL_FRAGMENT_SHADER);
#else
	_glShader = glCreateShader(GL_FRAGMENT_SHADER);
#endif

	const GLchar* ptr = (const GLchar*)code.first();
	GLint length = code.size();

#ifdef DRAWER_API_OPENGL
	glShaderSourceARB(_glShader, 1, &ptr, &length);
	glCompileShaderARB(_glShader);
#else
	glShaderSource(_glShader, 1, &ptr, &length);
	glCompileShader(_glShader);
#endif
	
#ifdef DRAWER_API_OPENGL
	if(glGetShaderiv && glGetShaderInfoLog)
#endif
	{
		GLint result;
		glGetShaderiv(_glShader, GL_COMPILE_STATUS, &result);
		if(result == GL_FALSE)
		{
			/* get the shader info log */
			glGetShaderiv(_glShader, GL_INFO_LOG_LENGTH, &length);
			char *log = (char*)alloca(length);
			glGetShaderInfoLog(_glShader, length, &result, log);
			
			logError <<  "Compile Pixel Shader Error: " << ConstString((utf8*)log, length);

#ifdef DRAWER_API_OPENGL
			glDeleteObjectARB(_glShader);
#else
			glDeleteShader(_glShader);
#endif
			_glShader = 0;

			//FILE* file = fopen("ps.txt", "wt");
			//fwrite(code.first(), code.byteSize(), 1, file);
			//fclose(file);

			return false;
		}
	}
	
	String errStr;
	if(isOpenGLError(errStr))
	{
		logError << errStr;
		return false;
	}
	
	return true;
}

boolean ShaderProgram::init(VertexShader& vertexShader, PixelShader& pixelShader, ConstRange< ConstString> vertParams)
{
	if(_glProgram){
#ifdef DRAWER_API_OPENGL
		glDeleteObjectARB(_glProgram);
#else
		glDeleteProgram(_glProgram);
#endif
		_glProgram = 0;
	}
	
#ifdef DRAWER_API_OPENGL
	_glProgram = glCreateProgramObjectARB();

	glAttachObjectARB(_glProgram, vertexShader.glShader());
	glAttachObjectARB(_glProgram, pixelShader.glShader());
	
	for(uint i=0; i < vertParams.size(); ++i)
		glBindAttribLocationARB(_glProgram, i, String(vertParams[i]).c_str());
	
	glLinkProgramARB(_glProgram);

	glValidateProgramARB(_glProgram);
#else
	_glProgram = glCreateProgram();

	glAttachShader(_glProgram, vertexShader.glShader());
	glAttachShader(_glProgram, pixelShader.glShader());

	for(uint i=0; i < vertParams.size(); ++i)
		glBindAttribLocation(_glProgram, i, String(vertParams[i]).c_str());

	glLinkProgram(_glProgram);

	glValidateProgram(_glProgram);
#endif

	String errStr;
	if(isOpenGLError(errStr))
	{
		logError << errStr;
		return false;
	}
	return true;
}

int ShaderProgram::getParameterIndex(ConstString name)
{
#ifdef DRAWER_API_OPENGL
	return glGetUniformLocationARB(_glProgram, String(name).c_str());
#else
	return glGetUniformLocation(_glProgram, String(name).c_str());
#endif
}


TextureNameIndexAndIndex ShaderProgram::getTextureIndex(ConstString name)
{
	TextureNameIndexAndIndex ret;
#ifdef DRAWER_API_OPENGL
	ret.shaderIndex = glGetUniformLocationARB(_glProgram, String(name).c_str());
#else
	ret.shaderIndex = glGetUniformLocation(_glProgram, String(name).c_str());
#endif

	if(ret.shaderIndex < 0)
		return ret;
	int* p = textureIndexes.find(ret.shaderIndex);
	if(p != textureIndexes.end()){
		ret.deviceIndex = p - textureIndexes.first();
		return ret;
	}
	textureIndexes.push(ret.shaderIndex);
	ret.deviceIndex = textureIndexes.size() - 1;
	return ret;
}

}

#endif

#include "vector.h"
#include "quaternion.h"
#include "matrix.h"
#include "string.h"

namespace Base
{

void print(String& s, int8 value)
{
	s.clear(5);
	s.forceSetSize(sprintf((char*)s.first(), "%i", int(value)));
}

void print(String& s, uint8 value)
{
	s.clear(5);
	s.forceSetSize(sprintf((char*)s.first(), "%u", uint(value)));
}

void print(String& s, int16 value)
{
	s.clear(7);
	s.forceSetSize(sprintf((char*)s.first(), "%i", int(value)));
}

void print(String& s, uint16 value)
{
	s.clear(7);
	s.forceSetSize(sprintf((char*)s.first(), "%u", value));
}

void print(String& s, int32 value)
{
	s.clear(12);
	s.forceSetSize(sprintf((char*)s.first(), "%i", value));
}

void print(String& s, uint32 value)
{
	s.clear(12);
	s.forceSetSize(sprintf((char*)s.first(), "%u", value));
}

void print(String& s, int64 value)
{
	s.clear(21);
	long long int v = value;
	s.forceSetSize(sprintf((char*)s.first(), "%lld", v));
}

void print(String& s, uint64 value)
{
	s.clear(21);
	long long unsigned int v = value;
	s.forceSetSize(sprintf((char*)s.first(), "%llu", v));
}

void print(String& s, float value)
{
	s.clear(16);
	s.forceSetSize(sprintf((char*)s.first(), "%.8g", value));
}

void print(String& s, double value)
{
	s.clear(32);
	s.forceSetSize(sprintf((char*)s.first(), "%.16g", value));
}

String printTabs(uint n)
{
	String str;
	str.reserve(n);
	for(uint i=0; i<n; ++i)
		str.fpush('\t');
	return str;
}

boolean _parse_to_nts(char* buff, uint buffsize, const ConstString& str)
{
	if(str.size() > buffsize - 1)
		return false;
	for(uint i = 0; i < str.size(); ++i)
		buff[i] = (char)str[i];
	buff[str.size()] = 0;
	return true;
}

boolean parse(int8& dest, const ConstString& str)
{
	char buff[5];
	if(!_parse_to_nts(buff, 5, str))
		return false;

	uint dst = 0;
	int scu = sscanf(buff, "%u", &dst);
	dest = uint8(dst);
	return 1 == scu;
}

boolean parse(uint8& dest, const ConstString& str)
{
	char buff[5];
	if(!_parse_to_nts(buff, 5, str))
		return false;

	uint dst = 0;
	int scu = sscanf(buff, "%i", &dst);
	dest = int8(dst);
	return 1 == scu;
}

boolean parse(int16& dest, const ConstString& str)
{
	char buff[7];
	if(!_parse_to_nts(buff, 7, str))
		return false;

	int dst = 0;
	int scu = sscanf(buff, "%i", &dst);
	dest = int16(dst);
	return 1 == scu;
}

boolean parse(uint16& dest, const ConstString& str)
{
	char buff[7];
	if(!_parse_to_nts(buff, 7, str))
		return false;

	uint dst = 0;
	int scu = sscanf(buff, "%u", &dst);
	dest = uint16(dst);
	return 1 == scu;
}

boolean parse(int32& dest, const ConstString& str)
{
	char buff[12];
	if(!_parse_to_nts(buff, 12, str))
		return false;

	int dst = 0;
	int scu = sscanf(buff, "%i", &dst);
	dest = int32(dst);
	return 1 == scu;
}

boolean parse(uint32& dest, const ConstString& str)
{
	char buff[12];
	if(!_parse_to_nts(buff, 12, str))
		return false;

	uint dst = 0;
	int scu = sscanf(buff, "%u", &dst);
	dest = uint32(dst);
	return 1 == scu;
}

boolean parse(int64& dest, const ConstString& str)
{
	char buff[21];
	if(!_parse_to_nts(buff, 21, str))
		return false;

	long long int dst = 0;
	int scu = sscanf(buff, "%lld", &dst);
	dest = int64(dst);
	return 1 == scu;
}

boolean parse(uint64& dest, const ConstString& str)
{
	char buff[21];
	if(!_parse_to_nts(buff, 21, str))
		return false;

	long long unsigned int dst = 0;
	int scu = sscanf(buff, "%llu", &dst);
	dest = uint64(dst);
	return 1 == scu;
}

boolean parse(float& dest, const ConstString& str)
{
	char buff[16];
	if(!_parse_to_nts(buff, 16, str))
		return false;

	float dst = 0;
	int scu = sscanf(buff, "%f", &dst);
	dest = float(dst);
	return 1 == scu;
}

boolean parse(double& dest, const ConstString& str)
{
	char buff[32];
	if(!_parse_to_nts(buff, 32, str))
		return false;

	double dst = 0;
	int scu = sscanf(buff, "%lf", &dst);
	dest = double(dst);
	return 1 == scu;
}



boolean parse(Matrix22f& v, const ConstString& str)
{
	return sscanf(String(str).c_str(), "%f%f%f%f", 
		&v.x.x, &v.x.y,
		&v.y.x, &v.y.y) == 4;
}


boolean parse(Matrix23f& v, const ConstString& str)
{
	return sscanf(String(str).c_str(), "%f%f%f%f%f%f", 
		&v.x.x, &v.x.y,
		&v.y.x, &v.y.y,
		&v.p.x, &v.p.y) == 6;
}


boolean parse(Matrix33f& v, const ConstString& str)
{
	return sscanf(String(str).c_str(), "%f%f%f%f%f%f%f%f%f", 
		&v.x.x, &v.x.y, &v.x.z,
		&v.y.x, &v.y.y, &v.y.z,
		&v.z.x, &v.z.y, &v.z.z) == 9;
}


boolean parse(Matrix34f& v, const ConstString& str)
{
	return sscanf(String(str).c_str(), "%f%f%f%f%f%f%f%f%f%f%f%f", 
		&v.x.x, &v.x.y, &v.x.z,
		&v.y.x, &v.y.y, &v.y.z,
		&v.z.x, &v.z.y, &v.z.z,
		&v.p.x, &v.p.y, &v.p.z) == 12;
}


boolean parse(Matrix44f& v, const ConstString& str)
{
	return sscanf(String(str).c_str(), "%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f", 
		&v.x.x, &v.x.y, &v.x.z, &v.x.w,
		&v.y.x, &v.y.y, &v.y.z, &v.y.w,
		&v.z.x, &v.z.y, &v.z.z, &v.z.w,
		&v.w.x, &v.w.y, &v.w.z, &v.w.w) == 16;
}

boolean parse(Quaternionf& v, const ConstString& str)
{
	return sscanf(String(str).c_str(), "%f%f%f%f", &v.x, &v.y, &v.z, &v.w) == 4;
}

boolean parse(Vector4f& v, const ConstString& str)
{
	return sscanf(String(str).c_str(), "%f%f%f%f", &v.x, &v.y, &v.z, &v.w) == 4;
}

boolean parse(Vector3f& v, const ConstString& str)
{
	return sscanf(String(str).c_str(), "%f%f%f", &v.x, &v.y, &v.z) == 3;
}

boolean parse(Vector2f& v, const ConstString& str)
{
	return sscanf(String(str).c_str(), "%f%f", &v.x, &v.y) == 2;
}




boolean parse(Matrix22d& v, const ConstString& str)
{
	return sscanf(String(str).c_str(), "%lf%lf%lf%lf",
		&v.x.x, &v.x.y,
		&v.y.x, &v.y.y) == 4;
}


boolean parse(Matrix23d& v, const ConstString& str)
{
	return sscanf(String(str).c_str(), "%lf%lf%lf%lf%lf%lf",
		&v.x.x, &v.x.y,
		&v.y.x, &v.y.y,
		&v.p.x, &v.p.y) == 6;
}


boolean parse(Matrix33d& v, const ConstString& str)
{
	return sscanf(String(str).c_str(), "%lf%lf%lf%lf%lf%lf%lf%lf%lf",
		&v.x.x, &v.x.y, &v.x.z,
		&v.y.x, &v.y.y, &v.y.z,
		&v.z.x, &v.z.y, &v.z.z) == 9;
}


boolean parse(Matrix34d& v, const ConstString& str)
{
	return sscanf(String(str).c_str(), "%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf",
		&v.x.x, &v.x.y, &v.x.z,
		&v.y.x, &v.y.y, &v.y.z,
		&v.z.x, &v.z.y, &v.z.z,
		&v.p.x, &v.p.y, &v.p.z) == 12;
}


boolean parse(Matrix44d& v, const ConstString& str)
{
	return sscanf(String(str).c_str(), "%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf",
		&v.x.x, &v.x.y, &v.x.z, &v.x.w,
		&v.y.x, &v.y.y, &v.y.z, &v.y.w,
		&v.z.x, &v.z.y, &v.z.z, &v.z.w,
		&v.w.x, &v.w.y, &v.w.z, &v.w.w) == 16;
}

boolean parse(Quaterniond& v, const ConstString& str)
{
	return sscanf(String(str).c_str(), "%lf%lf%lf%lf", &v.x, &v.y, &v.z, &v.w) == 4;
}

boolean parse(Vector4d& v, const ConstString& str)
{
	return sscanf(String(str).c_str(), "%lf%lf%lf%lf", &v.x, &v.y, &v.z, &v.w) == 4;
}

boolean parse(Vector3d& v, const ConstString& str)
{
	return sscanf(String(str).c_str(), "%lf%lf%lf", &v.x, &v.y, &v.z) == 3;
}

boolean parse(Vector2d& v, const ConstString& str)
{
	return sscanf(String(str).c_str(), "%lf%lf", &v.x, &v.y) == 2;
}



boolean parse(Vector4i& v, const ConstString& str)
{
	return sscanf(String(str).c_str(), "%d%d%d%d", &v.x, &v.y, &v.z, &v.w) == 4;
}

boolean parse(Vector3i& v, const ConstString& str)
{
	return sscanf(String(str).c_str(), "%d%d%d", &v.x, &v.y, &v.z) == 3;
}

boolean parse(Vector2i& v, const ConstString& str)
{
	return sscanf(String(str).c_str(), "%d%d", &v.x, &v.y) == 2;
}





void print(String& s, const Matrix22f& m)
{
	String tmp;
	for(uint j=0; j < 2; ++j)
		for(uint i=0; i < 2; ++i){
			print(tmp, m[j][i]);
			s << tmp;
			if(i*j != 1)
				s.push(utf8(' '));
		}
}


void print(String& s, const Matrix23f& m)
{
	String tmp;
	for(uint j=0; j < 3; ++j)
		for(uint i=0; i < 2; ++i){
			print(tmp, m[j][i]);
			s << tmp;
			if(i*j != 2)
				s.push(utf8(' '));
		}
}


void print(String& s, const Matrix33f& m)
{
	String tmp;
	for(uint j=0; j < 3; ++j)
		for(uint i=0; i < 3; ++i){
			print(tmp, m[j][i]);
			s << tmp;
			if(i*j != 4)
				s.push(utf8(' '));
		}
}


void print(String& s, const Matrix34f& m)
{
	String tmp;
	for(uint j=0; j < 4; ++j)
		for(uint i=0; i < 3; ++i){
			print(tmp, m[j][i]);
			s << tmp;
			if(i*j != 6)
				s.push(utf8(' '));
		}
}


void print(String& s, const Matrix44f& m)
{
	String tmp;
	for(uint j=0; j < 4; ++j)
		for(uint i=0; i < 4; ++i){
			print(tmp, m[j][i]);
			s << tmp;
			if(i*j != 9)
				s.push(utf8(' '));
		}
	s.fpop();// pop last char(' ')
}


void print(String& s, const Quaternionf& v)
{
	String tmp;
	for(uint i=0; i < 4; ++i){
		print(tmp, v[i]);
		s << tmp;
		if(i != 3)
			s.push(utf8(' '));
	}
}

void print(String& s, const Vector4f& v)
{
	String tmp;
	for(uint i=0; i < 4; ++i){
		print(tmp, v[i]);
		s << tmp;
		if(i != 3)
			s.push(utf8(' '));
	}
}


void print(String& s, const Vector3f& v)
{
	String tmp;
	for(uint i=0; i < 3; ++i){
		print(tmp, v[i]);
		s << tmp;
		if(i != 2)
			s.push(utf8(' '));
	}
}


void print(String& s, const Vector2f& v)
{
	String tmp;
	for(uint i=0; i < 2; ++i){
		print(tmp, v[i]);
		s << tmp;
		if(i != 1)
			s.push(utf8(' '));
	}
}




void print(String& s, const Matrix22d& m)
{
	String tmp;
	for(uint j = 0; j < 2; ++j)
	for(uint i = 0; i < 2; ++i){
		print(tmp, m[j][i]);
		s << tmp;
		if(i*j != 1)
			s.push(utf8(' '));
	}
}


void print(String& s, const Matrix23d& m)
{
	String tmp;
	for(uint j = 0; j < 3; ++j)
	for(uint i = 0; i < 2; ++i){
		print(tmp, m[j][i]);
		s << tmp;
		if(i*j != 2)
			s.push(utf8(' '));
	}
}


void print(String& s, const Matrix33d& m)
{
	String tmp;
	for(uint j = 0; j < 3; ++j)
	for(uint i = 0; i < 3; ++i){
		print(tmp, m[j][i]);
		s << tmp;
		if(i*j != 4)
			s.push(utf8(' '));
	}
}


void print(String& s, const Matrix34d& m)
{
	String tmp;
	for(uint j = 0; j < 4; ++j)
	for(uint i = 0; i < 3; ++i){
		print(tmp, m[j][i]);
		s << tmp;
		if(i*j != 6)
			s.push(utf8(' '));
	}
}


void print(String& s, const Matrix44d& m)
{
	String tmp;
	for(uint j = 0; j < 4; ++j)
	for(uint i = 0; i < 4; ++i){
		print(tmp, m[j][i]);
		s << tmp;
		if(i*j != 9)
			s.push(utf8(' '));
	}
	s.fpop();// pop last char(' ')
}


void print(String& s, const Quaterniond& v)
{
	String tmp;
	for(uint i = 0; i < 4; ++i){
		print(tmp, v[i]);
		s << tmp;
		if(i != 3)
			s.push(utf8(' '));
	}
}

void print(String& s, const Vector4d& v)
{
	String tmp;
	for(uint i = 0; i < 4; ++i){
		print(tmp, v[i]);
		s << tmp;
		if(i != 3)
			s.push(utf8(' '));
	}
}


void print(String& s, const Vector3d& v)
{
	String tmp;
	for(uint i = 0; i < 3; ++i){
		print(tmp, v[i]);
		s << tmp;
		if(i != 2)
			s.push(utf8(' '));
	}
}


void print(String& s, const Vector2d& v)
{
	String tmp;
	for(uint i = 0; i < 2; ++i){
		print(tmp, v[i]);
		s << tmp;
		if(i != 1)
			s.push(utf8(' '));
	}
}


void print(String& s, const Vector2i& v)
{
	String tmp;
	for(uint i=0; i < 2; ++i){
		print(tmp, v[i]);
		s << tmp;
		if(i != 1)
			s.push(utf8(' '));
	}
}

void print(String& s, const Vector3i& v)
{
	String tmp;
	for(uint i = 0; i < 3; ++i){
		print(tmp, v[i]);
		s << tmp;
		if(i != 1)
			s.push(utf8(' '));
	}
}

void print(String& s, const Vector4i& v)
{
	String tmp;
	for(uint i = 0; i < 4; ++i){
		print(tmp, v[i]);
		s << tmp;
		if(i != 1)
			s.push(utf8(' '));
	}
}

boolean normalizePath(String& dest, const ConstString& basePath, const ConstString& path)
{
	if(!path.size() || path[0] != '.'){
		dest = path;
		return true;
	}

	if(path.size() <= 2)
		return false;

	// ./
	if(isSlesh(path[1]))
	{
		dest.clear(basePath.size() + 1 + path.size() - 2);
		dest.fpush(basePath);
		dest.fpush('/');
		dest.fpush(path.afterLeftTrim(2));
		return true;
	}
	// ../
	else if(path[1] != '.' && isSlesh(path[2]))
	{
		ConstString p = path;
		ConstString b = basePath;
		do 
		{
			p = p.afterLeftTrim(3);
			b = ConstString(b.first(), b.rfind(utf8(0), SleshEqualComparator()));
			if(b.empty())
				return false;
		} while (p.size() >= 3 && p[0] == '.' && p[1] == '.' && isSlesh(p[2]));

		dest.clear(b.size() + 1 + p.size());
		dest.fpush(b);
		dest.fpush('/');
		dest.fpush(p);
		return true;
	}
	return false;
}


}

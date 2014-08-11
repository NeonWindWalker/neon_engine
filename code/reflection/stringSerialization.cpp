#include "include.h"

namespace ReflectionLib 
{

String collapseStringPath(const Array<String>& arr, const ConstString sybol)
{
	String s;
	for(uint i=0; i < arr.size(); ++i){
		s << arr[i];
		if(i + 1 < arr.size())
			s << sybol;
	}
	return s;
}

const utf8* parseString(ConstString& str, String& fixedStr, boolean& needUseFixedStr, ConstString source)
{
	const utf8* c = source.first();
	needUseFixedStr = false;
	while(c != source.end())
	{
		if(*c == '"'){
			str = ConstString(source.first(), c);
			return c + 1;
		}

		if(*c != '\\'){
			if(needUseFixedStr)
				fixedStr.push(*c);
			++c;
			continue;
		}

		++ c;
		while(c == source.end())
			return c;

		if(!needUseFixedStr){
			fixedStr.set(source.first(), c - 1);
			needUseFixedStr = true;
		}

		switch(*c)
		{
		case 'n':
			fixedStr.push('\n');
			break;
		case 'r':
			fixedStr.push('\r');
			break;
		case 't':
			fixedStr.push('\t');
			break;
		case '"':
			fixedStr.push('\"');
			break;
		default :
			fixedStr.push(*c);
			break;
		}
		++c;
	}
	str = ConstString(source.first(), c);
	return c;
}


enum ETokenType
{
	ETokenType_Null = 0,
	ETokenType_Clear,
	ETokenType_Insert,
	ETokenType_Erase,
	ETokenType_Name,
	ETokenType_Index,
	ETokenType_Construct,
	ETokenType_Destruct,
	ETokenType_Value,
	ETokenType_Reference,
	ETokenType_Begin,
	ETokenType_End,
	ETokenType_ArgsBegin,
	ETokenType_ArgsEnd,
	ETokenType_Comma
};

struct Token
{
	ETokenType type;
	ConstString str;

	Token() : type(ETokenType_Null) {}
	Token(ETokenType t, ConstString s) : type(t), str(s) {}
};

const utf8* findSpace(const utf8* frist, const utf8* end)
{
	const utf8* c;
	for(c = frist; c != end && !isSpace(*c); ++c);
	return c;
}

ConstString readToken(Token& tk, ConstString string, StringDeserializerErrorMsg& errorMsg)
{
	for(const utf8* c = string.first(); c != string.end(); ++c)
	{
		if(isSpace(*c))
			continue;

		if(isDigit(*c) || *c == '-')
		{
			const utf8* e = findSpace(c, string.end());
			tk.type = ETokenType_Value;
			tk.str = ConstString(c, e);
			return ConstString(e, string.end());
		}

		if(isLetter(*c) || *c == '_')
		{
			const utf8* e = findSpace(c, string.end());
			tk.type = ETokenType_Name;
			tk.str = ConstString(c, e);
			return ConstString(e, string.end());
		}

		if(*c == '$')
		{
			const utf8* e = findSpace(c, string.end());
			tk.type = ETokenType_Index;
			tk.str = ConstString(c + 1, e);
			return ConstString(e, string.end());
		}

		if(*c == '{')
		{
			tk.type = ETokenType_Begin;
			tk.str = ConstString(c, 1);
			return ConstString(c + 1, string.end());
		}

		if(*c == '}')
		{
			tk.type = ETokenType_End;
			tk.str = ConstString(c, 1);
			return ConstString(c + 1, string.end());
		}

		if(*c == '\"')
		{
			const utf8* e = c + 1;
			while(e != string.end() && *e != '\"')
				e++;

			if(e == string.end()){
				errorMsg.pos = c;
				errorMsg.msg = _CS("unexpected end, no closing \"");
				return ConstString(string.end(), string.end());
			}

			tk.type = ETokenType_Value;
			tk.str = ConstString(c + 1, e);
			return ConstString(e + 1, string.end());
		}

		if(*c == '!')
		{
			tk.type = ETokenType_Insert;
			tk.str = ConstString(c, 1);
			return ConstString(c + 1, string.end());
		}

		if(*c == '@')
		{
			const utf8* e = findSpace(c, string.end());
			tk.type = ETokenType_Construct;
			tk.str = ConstString(c + 1, e);
			return ConstString(e, string.end());
		}

		if(*c == '#')
		{
			const utf8* e = c + 1;
			while(e != string.end() && *e != '\n')
				e++;
			if(e == string.end()){
				errorMsg.pos = c;
				errorMsg.msg = _CS("unexpected end, no new line after #");
				return ConstString(string.end(), string.end());
			}

			uint lines=0;
			if(!parse(lines, ConstString(c + 1, e)) || lines == 0){
				errorMsg.pos = c;
				errorMsg.msg = _CS("cant parse lines count after #");
				return ConstString(string.end(), string.end());
			}

			c = ++e;
			for(; e != string.end() && lines; ++e)
				if(*e == '\n')
					-- lines;

			if(e == string.end() && lines){
				errorMsg.pos = c;
				errorMsg.msg = _CS("unexpected end, cant read all lines after #");
				return ConstString(string.end(), string.end());
			}

			tk.type = ETokenType_Value;
			tk.str = ConstString(c, e);
			return ConstString(e, string.end());
		}

		if(*c == '(')
		{
			tk.type = ETokenType_ArgsBegin;
			tk.str = ConstString(c, 1);
			return ConstString(c + 1, string.end());
		}

		if(*c == ')')
		{
			tk.type = ETokenType_ArgsEnd;
			tk.str = ConstString(c, 1);
			return ConstString(c + 1, string.end());
		}

		if(*c == ',')
		{
			tk.type = ETokenType_Comma;
			tk.str = ConstString(c, 1);
			return ConstString(c + 1, string.end());
		}

		if(*c == '%')
		{
			const utf8* e = findSpace(c, string.end());
			tk.type = ETokenType_Reference;
			tk.str = ConstString(c + 1, e);
			return ConstString(e, string.end());
		}

		if(*c == '~' )
		{
			if(c + 1 < string.end() && *(c+1) == '~'){
				tk.type = ETokenType_Clear;
				tk.str = ConstString(c + 2, uint(0));
				return ConstString(c + 2, string.end());
			}
			tk.type = ETokenType_Erase;
			tk.str = ConstString(c, 1);
			return ConstString(c + 1, string.end());
		}

		if(*c == '^')
		{
			const utf8* e = findSpace(c, string.end());
			tk.type = ETokenType_Destruct;
			tk.str = ConstString(c + 1, e);
			return ConstString(e, string.end());
		}

		if(*c == '/')
		{
			while(!isNewLine(*c++))
			{
				if(c == string.end()){
					tk.type = ETokenType_Null;
					tk.str = ConstString(string.first(), string.first());
					return ConstString(string.end(), string.end());
				}
			}
			continue;
		}

		errorMsg.pos = c;
		errorMsg.msg = _CS("unknow character");
		return ConstString(c, string.end());
	}

	tk.type = ETokenType_Null;
	tk.str = ConstString(string.first(), string.first());
	return ConstString(string.end(), string.end());
}


ConstString deserializeTraverse(ETokenType stopOperator, Reflection father, ConstString string, StringDeserializerErrorMsg& errorMsg);

ConstString readArguments(Array<Reflection>& vals, Array<void*>& valPtrs, ConstString string, const Semantic& sem, StringDeserializerErrorMsg& errorMsg)
{
	if(sem.empty())
	{
		Token tk;
		string = readToken(tk, string, errorMsg);
		if(tk.type != ETokenType_ArgsEnd && !errorMsg.isError()){
			errorMsg.pos = string.first();
			errorMsg.msg = _CS("cant find )");
			return ConstString(string.end(), string.end());
		}
		return string;
	}

	vals.resize(sem.size());
	valPtrs.resize(sem.size());
	for(uint nVal = 0; nVal < sem.size(); ++nVal)
	{
		Reflection r = sem[nVal].construct(Base::gSystemAllocator, 0);
		vals[nVal] = r;
		valPtrs[nVal] = r.objectPtr();
		string = deserializeTraverse(nVal + 1 != sem.size() ? ETokenType_Comma : ETokenType_ArgsEnd, r, string, errorMsg);
		if(errorMsg.isError()){
			for(int n = nVal; n >= 0; --n)
				vals[n].destruct();
			return string;
		}
	}

	return string;
}

ConstString deserializeTraverse(ETokenType stopOperator, Reflection father, ConstString string, StringDeserializerErrorMsg& errorMsg)
{
	if(!father.valid()){
		errorMsg.pos = string.first();
		errorMsg.msg << _CS("invalid reflection father");
		return string;
	}

	Token token;
	string = readToken(token, string, errorMsg);
	if(errorMsg.isError())
		return string;

	while(token.type)
	{
		if(token.type == stopOperator)
			return ConstString(token.str.end(), string.end());


		//Insert/Erase
		boolean needInsert = false;
		boolean needErase = false;

		if(token.type == ETokenType_Insert)
		{
			needInsert = true;
			string = readToken(token, string, errorMsg);
			if(!token.type || errorMsg.isError())
				return string;
		}

		if(token.type == ETokenType_Erase)
		{
			needErase = true;
			string = readToken(token, string, errorMsg);
			if(!token.type || errorMsg.isError())
				return string;
		}


		// Main Block
		Reflection refl = father;
		uint index = -1;
		ConstString name;

		if(token.type == ETokenType_Name)
		{
			name = token.str;
			if((needInsert | needErase) == 0)
			{
				refl = father.reflection(name);
			}
			else if(needInsert)
			{
				father.insert(name);
				refl = father.reflection(name);
			}
			else if(needErase)
			{
				father.erase(name);
				string = readToken(token, string, errorMsg);
				if(!token.type || errorMsg.isError())
					return string;
				continue;
			}

			string = readToken(token, string, errorMsg);
			if(!token.type || errorMsg.isError())
				return string;
		}

		if(token.type == ETokenType_Index)
		{
			if(token.str.empty())
				index = father.size();
			else{
				if(!parse(index, token.str)){
					errorMsg.pos = token.str.first();
					return string;
				}
			}

			if((needInsert | needErase) == 0)
			{
				refl = father.reflection(index);
			}
			else if(needInsert)
			{
				father.insert(index);
				refl = father.reflection(index);
			}
			else if(needErase)
			{
				father.erase(index);
				string = readToken(token, string, errorMsg);
				if(!token.type || errorMsg.isError())
					return string;
				continue;
			}

			string = readToken(token, string, errorMsg);
			if(!token.type || errorMsg.isError())
				return string;
		}

	

		//Construct / Destruct
		if(token.type == ETokenType_Construct)
		{
			if(name.empty() && index == (uint)-1){
				errorMsg.pos = string.first();
				errorMsg.msg = _CS("no name or index to construction");
				return string;
			}

			Type type = Type::getType(token.str);
			if(!type.valid()){
				errorMsg.pos = string.first();
				errorMsg.msg = _CS("cant find type type \"");
				errorMsg.msg << token.str << _CS("\"");
				return string;
			}

			string = readToken(token, string, errorMsg);
			if(errorMsg.isError())
				return string;

			if(token.type != ETokenType_ArgsBegin)
			{
				if(name.empty())
					refl = father.construct(index, type);
				else
					refl = father.construct(name, type);

				if(!refl.valid()){
					errorMsg.pos = string.first();
					errorMsg.msg = _CS("cant construct type \"");
					errorMsg.msg << type.name() << _CS("\"");
					return string;
				}
			}
			else
			{
				const Semantic* sem;
				if(name.empty())
					sem = refl.localConstructorSemantic(index);
				else
					sem = refl.localConstructorSemantic(name);

				if(!sem){
					errorMsg.pos = string.first();
					errorMsg.msg = _CS("invalid semantic");
					return string;
				}

				Array<Reflection> vals;
				Array<void*> valPtrs;
				string = readArguments(vals, valPtrs, string, *sem, errorMsg);
				if(errorMsg.isError())
					return string;

				if(name.empty())
					refl = father.construct(index, type, EChildType_Normal, valPtrs.first());
				else
					refl = father.construct(name, type, EChildType_Normal, valPtrs.first());

				for(uint nVal = 0; nVal < vals.size(); ++nVal)
					vals[nVal].destruct();

				if(!refl.valid()){
					errorMsg.pos = string.first();
					errorMsg.msg = _CS("cant construct type \"");
					errorMsg.msg << type.name() << _CS("\"");
					return string;
				}

				string = readToken(token, string, errorMsg);
				if(errorMsg.isError())
					return string;
			}

			if(!token.type)
				return string;
		}

		if(token.type == ETokenType_Destruct)
		{
			refl.destruct();

			string = readToken(token, string, errorMsg);
			if(!token.type || errorMsg.isError())
				return string;
		}


		// Last Block, assigning or call or intend

		if(token.type == ETokenType_Value)
		{
			refl.set(token.str);
			string = readToken(token, string, errorMsg);
			if(!token.type || errorMsg.isError())
				return string;
		}
		else if(token.type == ETokenType_Begin)
		{
			string = deserializeTraverse(ETokenType_End, refl, string, errorMsg);
			string = readToken(token, string, errorMsg);
			if(!token.type || errorMsg.isError())
				return string;
		}
		else if(token.type == ETokenType_ArgsBegin)
		{
			const MethodSemantic* sem = refl.methodSemantic();
			if(!sem || sem->out.size()){
				errorMsg.pos = string.first();
				errorMsg.msg = _CS("invalid method semantic");
				return string;
			}

			Array<Reflection> vals;
			Array<void*> valPtrs;
			string = readArguments(vals, valPtrs, string, sem->in, errorMsg);
			if(errorMsg.isError())
				return string;

			refl.methodCall(0, valPtrs.first());

			for(uint nVal = 0; nVal < vals.size(); ++nVal)
				vals[nVal].destruct();

			string = readToken(token, string, errorMsg);
			if(!token.type || errorMsg.isError())
				return string;
		}
		else if(token.type == ETokenType_Reference)
		{
			ConstString refname = token.str;
			string = readToken(token, string, errorMsg);
			if(!token.type || errorMsg.isError())
				return string;

			if(token.type != ETokenType_Value){
				errorMsg.pos = string.first();
				errorMsg.msg = _CS("no reference path");
				return string;
			}

			Reflection r = father.reflectionByPath(token.str);
			refl.setref(refname, r);

			string = readToken(token, string, errorMsg);
			if(!token.type || errorMsg.isError())
				return string;
		}
		else if(token.type == ETokenType_Clear)
		{
			refl.clear();
			string = readToken(token, string, errorMsg);
			if(!token.type || errorMsg.isError())
				return string;
		}
		else
		{
			errorMsg.pos = string.first();
			errorMsg.msg = _CS("no assigning calling or intend");
			return string;
		}
	}
	return string;
}

boolean Reflection::deserialize(ConstString string)
{
	if(string[0] == 0xef)
		string = string.afterLeftTrim(1);
	if(string[0] == 0xbb)
		string = string.afterLeftTrim(1);
	if(string[0] == 0xbf)
		string = string.afterLeftTrim(1);

	StringDeserializerErrorMsg msg;
	deserializeTraverse(ETokenType_Null, *this, string, msg);
	if(msg.isError())
		logWarning << _CS("char: ") << msg.pos << _CS(" ") << msg.msg;
	return !msg.isError();
}


void serializeTraverse(String& outStr, Reflection father, uint tabs)
{
	EMethaType fatherMethaType = father.typeMethaType();
	switch(fatherMethaType)
	{
	case EMethaType_Value:
	case EMethaType_EnumValue: 
		{
			String s;
			if(father.get(s))
			{
				if(isValidValue(s))
					outStr << s << _CS(" ");
				else
					outStr << _CS("\"") << s << _CS("\"");
			}
		}
		break;
	case EMethaType_Method: 
		break;

	case EMethaType_Array:
	case EMethaType_PolyArray: 
	case EMethaType_Class: 
	case EMethaType_Map: 
	case EMethaType_PolyMap: 
		{
			for(uint n=0, size = father.size(); n < size; ++n)
			{
				Reflection r = father.reflection(n);
				if(!r.valid() || r.typeMethaType() == EMethaType_Method)
					continue;

				r = r.endcast();

				EChildMode childMode = father.type().childMode(n);

				outStr.printTabs(tabs);

				if(isInsertEraseAllowed(fatherMethaType))
					outStr << _CS("! ");

				if(isNamed(fatherMethaType))
					outStr << father.name(n);
				else
					outStr << _CS("$") << n;

				outStr << _CS(" ");

				if(childMode == EChildType_Polymorphic)
					 outStr << _CS("@") << r.typeName() << _CS(" ");

				EMethaType methaType = r.typeMethaType();
				if(isValue(methaType))
				{
					serializeTraverse(outStr, r, tabs + 1);
				}
				else if(isContainer(methaType))
				{
					outStr << _CS("\n");
					outStr.printTabs(tabs) << _CS("{") << _CS("\n");
					serializeTraverse(outStr, r, tabs + 1);
					outStr.printTabs(tabs) << _CS("}");
				}

				outStr << _CS("\n");
			}
		}
		break;

	case EMethaType_RefArray: 
		break;
	case EMethaType_RefMap: 
		break;
	}
}


boolean Reflection::serialize(String& str)
{
	serializeTraverse(str, *this, 0);
	return true;
}

template<class t_Type>
INLINE boolean assignValue(Reflection dst, Reflection src)
{
	t_Type v;
	if(src.get(v))
		return dst.set(v);
	return false;
}


boolean Reflection::assign(Reflection src)
{
	if(!valid()){
		logWarning << "dest invalid";
		return false;
	}
	if(!src.valid()){
		logWarning << "src invalid";
		return false;
	}

	uint64 dstVer, srcVer;
	boolean versioned = src.getVersion(srcVer) && getVersion(dstVer);
	if(versioned && srcVer == dstVer)
		return true;

	EMethaType srcType = src.typeMethaType(), destType = typeMethaType();
	if(srcType != destType){
		logWarning << _CS("type mismatch");
		//return false;
	}

	//FIXME ������ ����� return
	if(versioned)
		setVersion(srcVer);

	switch(srcType)
	{
	case EMethaType_Value:
		{
			switch(src.typeId().number())
			{
			case PredefinedTypeID<int8>::value : return assignValue<int8>(*this, src);
			case PredefinedTypeID<uint8>::value : return assignValue<uint8>(*this, src);
			case PredefinedTypeID<int16>::value : return assignValue<int16>(*this, src);
			case PredefinedTypeID<uint16>::value : return assignValue<uint16>(*this, src);
			case PredefinedTypeID<int32>::value : return assignValue<int32>(*this, src);
			case PredefinedTypeID<uint32>::value : return assignValue<uint32>(*this, src);
			case PredefinedTypeID<int64>::value : return assignValue<int64>(*this, src);
			case PredefinedTypeID<uint64>::value : return assignValue<uint64>(*this, src);
			case PredefinedTypeID<float>::value : return assignValue<float>(*this, src);
			case PredefinedTypeID<double>::value : return assignValue<double>(*this, src);
			case PredefinedTypeID<String>::value : return assignValue<String>(*this, src);
			}
			return false;
		}
	case EMethaType_EnumValue:
		{
			uint v;
			if(src.get(v))
				set(v);
			return true;
		}
	case EMethaType_Class:
		{
			if(src.type() == type())
			{
				uint srcSize = src.size();
				for(uint i=0; i < srcSize; ++i)
				{
					Reflection s = src.reflection(i);
					Reflection d = reflection(i);
					if(src.mode(i) != EChildType_Reference)
					{
						Type sType = s.type();
						if(!d.valid() || d.type() != sType)
						{
							if(s.valid())
							{
								TTempAllocator<gcMaxArgumntsCount * 16> alc;
								void* args[gcMaxArgumntsCount];
								s.constuctorValues(args, alc);
								d = construct(i, sType, EChildType_Normal, args);
								d.assign(s);
							}
							else
								destruct(i);
						}
						else
							d.assign(s);
					}else
						setref(i, s);
				}
			}
			else
			{
				uint srcSize = src.size();
				for(uint i=0; i < srcSize; ++i)
				{
					Reflection s = src.reflection(i);
					String name = src.name(i);
					Reflection d = reflection(name);
					if(src.mode(i) != EChildType_Reference)
					{
						Type sType = s.type();
						if(!d.valid() || d.type() != sType)
						{
							if(s.valid())
							{
								TTempAllocator<gcMaxArgumntsCount * 16> alc;
								void* args[gcMaxArgumntsCount];
								s.constuctorValues(args, alc);
								d = construct(name, sType, EChildType_Normal, args);
								d.assign(s);
							}
							else
								destruct(name);
						}
						else
							d.assign(s);
					}else
						setref(name, s);
				}
			}
			return true;
		}
	case EMethaType_Array:
		{
			uint s = src.size();
			resize(s);
			for(uint i=0; i < s; ++i)
				reflection(i).assign(src.reflection(i));
			return true;
		}
		
	case EMethaType_RefArray:
		{
			uint s = src.size();
			resize(s);
			for(uint i=0; i < s; ++i)
				setref(i, src.reflection(i));
			return true;
		}

	case EMethaType_PolyArray:
		{
			uint s = src.size();
			resize(s);
			for(uint i=0; i < s; ++i)
			{
				Reflection d = reflection(i);
				Reflection s = src.reflection(i);
				Type t = s.type();
				if(t != d.type()){
					TTempAllocator<gcMaxArgumntsCount * 16> alc;
					void* args[gcMaxArgumntsCount];
					s.constuctorValues(args, alc);
					d = construct(i, t, EChildType_Normal, args);
				}
				d.assign(s);
			}
			return true;
		}
		
	case EMethaType_Map:
	case EMethaType_RefMap:
	case EMethaType_PolyMap:
		{
			uint srcSize = src.size();
			uint destSize = size();
			for(uint i=0; i < destSize; )
			{
				Reflection s = src.reflection(name(i));
				if(s.validType()){
					i ++;
				}else{
					erase(i);
					destSize --;
				}
			}

			for(uint i=0; i < srcSize; ++i)
			{
				Reflection s = src.reflection(i);
				String name = src.name(i);
				Reflection d = reflection(name);
				if(d.type() != s.type())
				{
					if(!d.validType())
						insert(name);
					if(srcType == EMethaType_PolyMap){
						TTempAllocator<gcMaxArgumntsCount * 16> alc;
						void* args[gcMaxArgumntsCount];
						s.constuctorValues(args, alc);
						d = construct(name, s.type(), EChildType_Normal, args);
					}
					else if(srcType == EMethaType_Map)
						d = reflection(name);
				}
				if(srcType != EMethaType_RefMap)
					d.assign(s);
				else
					setref(name, s);
			}
			return true;
		}
    default:
        Assert(false);
	}

	return false;
}






//********************* JSON *************************//

const utf8* skipSpaces(const utf8* c, const utf8* end)
{
	while(c != end && isSpace(*c))c++;
	return c;
}

const utf8* deserializeTraverseJSON(Reflection father, ConstString string)
{
	const utf8* c = string.first();
	c = skipSpaces(c, string.end());
	if(c == string.end())
		return string.end();

	if(*c == '{')
	{
		++c;
		c = skipSpaces(c, string.end());
		if(c == string.end())
			return string.end();

		while(*c != '}')
		{
			ConstString name;
			String sname;
			if(*c == '"')
			{
				ConstString cname;
				boolean sUse;
				c = parseString(cname, sname, sUse, ConstString(c + 1, string.end()));
				name = sUse ? ConstString(sname) : cname;
			}
			else
			{
				const utf8* f = c;
				while(c != string.end() && !isSpace(*c) && *c != ',' && *c != ']' && *c != '}')
					c++;
				name = ConstString(f, c);
			}

			c = skipSpaces(c, string.end());
			if(c == string.end())
				return string.end();

			if(*c != ':')//error
				return string.end();
			
			++c;

			father.insert(name);
			Reflection el = father.reflection(name);
			c = deserializeTraverseJSON(el, ConstString(c, string.end()));
			c = skipSpaces(c, string.end());
			if(c == string.end())
				return string.end();
			if(*c == ',')
				++c;
		}
		++c;
	}
	else if(*c == '[')
	{
		++c;
		c = skipSpaces(c, string.end());
		if(c == string.end())
			return string.end();

		while(*c != ']')
		{
			uint n = father.size();
			father.insert(n);
			Reflection el = father.reflection(n);
			c = deserializeTraverseJSON(el, ConstString(c, string.end()));
			c = skipSpaces(c, string.end());
			if(c == string.end())
				return string.end();
			if(*c == ',')
				++c;
		}
		++c;
	}
	else
	{
		c = skipSpaces(c, string.end());
		if(c == string.end())
			return string.end();

		ConstString val;
		String sval;
		if(*c == '"')
		{
			ConstString cval;
			boolean sUse;
			c = parseString(cval, sval, sUse, ConstString(c + 1, string.end()));
			val = sUse ? ConstString(sval) : cval;
		}
		else
		{
			const utf8* f = c;
			while(c != string.end() && !isSpace(*c) && *c != ',' && *c != ']' && *c != '}')
				c++;
			val = ConstString(f, c);
		}
		if(val == _CS("true"))
			father.set(1);
		else if(val == _CS("false"))
			father.set(0);
		else if(val == _CS("null"))
			father.set(0);
		else
			father.set(val);
	}
	return c;
}


void serializeTraverseJSON(String& outStr, Reflection father)
{
	EMethaType fatherMethaType = father.typeMethaType();
	switch(fatherMethaType)
	{
	case EMethaType_Value:
	case EMethaType_EnumValue:
	{
		String s;
		father.get(s);
		outStr << _CS("\"") << backslashEncoding(s) << _CS("\"");
	}
		break;
	case EMethaType_Method:
		break;

	case EMethaType_Array:
	case EMethaType_PolyArray:
	{
		outStr << _CS("[");
		boolean firstSerialized = false;
		for(uint n = 0, size = father.size(); n < size; ++n)
		{
			Reflection r = father.reflection(n).endcast();
			if(!r.valid() || r.typeMethaType() == EMethaType_Method)
				continue;

			if(firstSerialized)
				outStr << _CS(",");

			serializeTraverseJSON(outStr, r);
			firstSerialized = true;
		}
		outStr << _CS("]");
	}
		break;
	case EMethaType_Class:
	case EMethaType_Map:
	case EMethaType_PolyMap:
	{
		outStr << _CS("{");
		boolean firstSerialized = false;
		for(uint n = 0, size = father.size(); n < size; ++n)
		{
			Reflection r = father.reflection(n).endcast();
			if(!r.valid() || r.typeMethaType() == EMethaType_Method)
				continue;

			if(firstSerialized)
				outStr << _CS(",");

			outStr << _CS("\"") << backslashEncoding(father.name(n)) << _CS("\":");
			serializeTraverseJSON(outStr, r);
			firstSerialized = true;
		}
		outStr << _CS("}");
	}
		break;
	case EMethaType_RefArray:
		break;
	case EMethaType_RefMap:
		break;
	}
}

boolean Reflection::deserializeJSON(ConstString string)
{
	string = trimUTF8FileHeader(string);

	deserializeTraverseJSON(*this, string);
	return true;
}

boolean Reflection::serializeJSON(String& str)
{
	if(!valid())
		return false;
	serializeTraverseJSON(str, *this);
	return true;
}

struct TraverseURLEncodedElement
{
	String name;
	int index;
	int lastUsedIndex;

	TraverseURLEncodedElement()
	{
		index = -1;
		lastUsedIndex = -1;
	}
};

void serializeTraverseURLEncoded(String& outStr, Array<TraverseURLEncodedElement>& history, Reflection father)
{
	EMethaType fatherMethaType = father.typeMethaType();
	switch(fatherMethaType)
	{
		case EMethaType_Value:
		case EMethaType_EnumValue:
		{
			String s;
			if(!father.get(s))
				return;
			outStr << _CS("&");
			if(history.size() == 1)
			{
				outStr << history[0].name;
			}
			else
			{
				for(uint i = 0; i < history.size(); ++i)
				{
					TraverseURLEncodedElement& el = history[i];
					outStr << _CS("[");
					if(el.index < 0)
					{
						outStr << percentEncoding(el.name);
					}
					else
					{
						if(el.index != el.lastUsedIndex + 1)
							outStr << print(el.index);
						else
							el.lastUsedIndex = el.index;
					}
					outStr << _CS("]");
				}
			}
			
			outStr << _CS("=") << percentEncoding(s);
		}
		break;

	case EMethaType_Method:
		break;

	case EMethaType_Array:
	case EMethaType_PolyArray:
	{
		uint index = 0;
		for(uint n = 0, size = father.size(); n < size; ++n)
		{
			Reflection r = father.reflection(n).endcast();
			if(!r.valid() || r.typeMethaType() == EMethaType_Method)
				continue;

			TraverseURLEncodedElement& el = *history.push();
			el.index = index++;
			serializeTraverseURLEncoded(outStr, history, r);
			history.fpop();
		}				
	}
		break;
	case EMethaType_Class:
	case EMethaType_Map:
	case EMethaType_PolyMap:
	{
		uint index = 0;
		for(uint n = 0, size = father.size(); n < size; ++n)
		{
			Reflection r = father.reflection(n).endcast();
			if(!r.valid() || r.typeMethaType() == EMethaType_Method)
				continue;

			TraverseURLEncodedElement& el = *history.push();
			el.name = percentEncoding(father.name(n));
			serializeTraverseURLEncoded(outStr, history, r);
			history.fpop();
		}
	}
		break;
	case EMethaType_RefArray:
		break;
	case EMethaType_RefMap:
		break;
	}
}

boolean Reflection::serializeURLEncoded(String& str)
{
	if(!valid())
		return false;
	Array<TraverseURLEncodedElement> history;
	serializeTraverseURLEncoded(str, history, *this);
	return true;
}


}

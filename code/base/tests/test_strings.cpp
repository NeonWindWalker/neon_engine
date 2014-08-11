#include "../include.h"
#include "tests.h"

#include <string>

void Base::testString()
{
	logInfoSkim<<"<<<<< STRING TESTS >>>>>";

	int summ = 0;
	Base::uint cyclesCount = 100000;

#ifdef DEBUG
	cyclesCount /= 100;
#endif

	ConstString courceSplit = _CS(".split0.sp1.spl2.");
	Array<ConstString> spliterRanges;
	split(spliterRanges, courceSplit, utf8('.'));
	logInfoSkim<<"string to split " << courceSplit;
	for(uint i=0; i<spliterRanges.size(); ++i)
		logInfoSkim<<"split[" << i << "] = \"" << spliterRanges[i] << "\"";
	
	Timer timer;
	String str;
	timer.dt();
	for(uint i=0; i < cyclesCount; ++i){
		str = _CS("hfyioyuidtdyklojklgjk");
		str = _CS("hfyioyuidtdyklojklgjk");
		str = _CS("hfyioyuidtdyklojklgjk");
		str = _CS("hfyioyuidtdyklojklgjk");
		str = _CS("hfyioyuidtdyklojklgjk");
		str = _CS("hfyioyuidtdyklojklgjk");
		str = _CS("hfyioyuidtdyklojklgjk");
		str = _CS("hfyioyuidtdyklojklgjk");
		str = _CS("hfyioyuidtdyklojklgjk");
		str = _CS("hfyioyuidtdyklojklgjk");
	}
	logInfoSkim<<"string assign " << 1e9 * timer.dt() / cyclesCount / 10;
	logInfoSkim<<str;


	std::string sdt_str;
	timer.dt();
	for(uint i=0; i < cyclesCount; ++i){
		sdt_str = "hfyioyuidtdyklojklgjk";
		sdt_str = "hfyioyuidtdyklojklgjk";
		sdt_str = "hfyioyuidtdyklojklgjk";
		sdt_str = "hfyioyuidtdyklojklgjk";
		sdt_str = "hfyioyuidtdyklojklgjk";
		sdt_str = "hfyioyuidtdyklojklgjk";
		sdt_str = "hfyioyuidtdyklojklgjk";
		sdt_str = "hfyioyuidtdyklojklgjk";
		sdt_str = "hfyioyuidtdyklojklgjk";
		sdt_str = "hfyioyuidtdyklojklgjk";
	}
	logInfoSkim<<"std::string assign " << 1e9 * timer.dt() / cyclesCount / 10;
	logInfoSkim<<sdt_str.c_str();

	str.reserve(24);
	timer.dt();
	for(uint i=0; i < cyclesCount; ++i){
		memoryCopy(str.first(), "hfyioyuidtdyklojklgjk", 22);
		memoryCopy(str.first(), "hfyioyuidtdyklojklgjk", 22);
		memoryCopy(str.first(), "hfyioyuidtdyklojklgjk", 22);
		memoryCopy(str.first(), "hfyioyuidtdyklojklgjk", 22);
		memoryCopy(str.first(), "hfyioyuidtdyklojklgjk", 22);
		memoryCopy(str.first(), "hfyioyuidtdyklojklgjk", 22);
		memoryCopy(str.first(), "hfyioyuidtdyklojklgjk", 22);
		memoryCopy(str.first(), "hfyioyuidtdyklojklgjk", 22);
		memoryCopy(str.first(), "hfyioyuidtdyklojklgjk", 22);
		memoryCopy(str.first(), "hfyioyuidtdyklojklgjk", 22);
	}
	logInfoSkim<<"string memcpy " << 1e9 * timer.dt() / cyclesCount / 10;
	logInfoSkim<<str;

	timer.dt();
	for(uint i=0; i < cyclesCount; ++i){
		double d;
		parse(d, _CS("3.14156745"));
		summ += (int)d;
	}
	logInfoSkim<<"parse double time ns = " << 1e9 * timer.dt() / cyclesCount;

	timer.dt();
	for(uint i=0; i < cyclesCount; ++i){
		int n;
		parse(n, _CS("314156745"));
		summ += n;
	}
	logInfoSkim<<"parse int time ns = " << 1e9 * timer.dt() / cyclesCount;

	RedBlackTreeMap<Base::String > strmap;
	strmap(_CS("str"));

	Base::String s1,s2;
	s1 << cyclesCount;
	//Base::compare(s1,s2);

	logInfoSkim<<"summ " << summ;
	logInfoSkim<<".";
	
	Base::String backtraceStr;
	Base::backtraceString(backtraceStr);
	logInfo << backtraceStr;
}

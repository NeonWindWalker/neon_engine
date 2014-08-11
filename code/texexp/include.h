#include "../base/include.h"

namespace TexExp
{
	struct Parameters
	{
		Base::Array<Base::String> disableDownsapling;
		Base::Array<Base::String> disableMipGen;
		Base::Array<Base::String> disableCompression;
		Base::boolean dxtEnable;
		Base::boolean atcEnable;
		Base::boolean pvrEnable;
		Base::boolean etcEnable;

		Parameters() {
			dxtEnable = true;
			atcEnable = true;
			pvrEnable = true;
			etcEnable = true;
		}
	};

	extern void doTexExport(Base::ConstString rootPath, Parameters* prams = 0);
	extern void doTexExport(Base::ConstString rootPath, Base::ConstString localFileName, Parameters* prams = 0);
}
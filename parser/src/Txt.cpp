#include "pmx2txt/parser/Txt.h"

#include <iomanip>

using namespace std;

void ExportVersion(std::ostream& stream, const pmx::Model& model)
{
	stream << "PMX バージョン: " << fixed << setprecision(1) << model.version << defaultfloat << endl;
}

using UEncoding = uint8_t;
struct UnknownEncoding : exception {};
string ToString(UEncoding encoding)
{
	switch (encoding)
	{
	case 0: return "UTF-16(LE)";
	case 1: return "UTF-8";
	default: throw UnknownEncoding{};
	}
}

void ExportSetting(std::ostream& stream, const pmx::Model& model)
{
	stream << "文字エンコード: " << ToString(model.setting.encoding) << endl;
}

void pmx2txt::txt::Export(std::ostream& stream, const pmx::Model& model)
{
	ExportVersion(stream, model);
	ExportSetting(stream, model);
}

#include "pmx2txt/parser/Txt.h"

#include <iomanip>

using namespace std;

void ExportVersion(std::ostream& stream, const pmx::Model& model)
{
	stream << "PMX バージョン: " << fixed << setprecision(1) << model.version << defaultfloat << endl;
}

using UEncoding = uint8_t;
struct UnknownEncoding : exception {};
string EncodingToString(UEncoding encoding)
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
	stream << "文字エンコード: " << EncodingToString(model.setting.encoding) << endl;
	stream << "追加UV数: " << static_cast<int>(model.setting.uv) << endl;
}

void ExportModelInfo(std::ostream& stream, const pmx::Model& model)
{
	stream << "モデル名: " << model.model_name << endl;
	stream << "コメント: " << endl;
	stream << "----------------" << endl;
	stream << model.model_comment << endl;
	stream << "----------------" << endl;
	stream << "モデル名(英): " << model.model_english_name << endl;
	stream << "コメント(英): " << endl;
	stream << "----------------" << endl;
	stream << model.model_english_comment << endl;
	stream << "----------------" << endl;
}

void pmx2txt::txt::Export(std::ostream& stream, const pmx::Model& model)
{
	ExportVersion(stream, model);
	ExportSetting(stream, model);
	ExportModelInfo(stream, model);
}

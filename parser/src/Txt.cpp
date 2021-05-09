#include "pmx2txt/parser/Txt.h"

#include <iomanip>
#include <sstream>

using namespace std;

void ExportVersion(std::ostream& stream, const pmx::Model& model)
{
	auto defaultPrecision = stream.precision();
	stream << "PMX バージョン: " << fixed << setprecision(2) << model.version << defaultfloat << setprecision(defaultPrecision) << endl;
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
	if (model.model_english_name.length() > 0)
		stream << "モデル名(英): " << model.model_english_name << endl;
	if (model.model_english_comment.length() > 0)
	{
		stream << "コメント(英): " << endl;
		stream << "----------------" << endl;
		stream << model.model_english_comment << endl;
		stream << "----------------" << endl;
	}
}

template<typename T, size_t size>
string ArrayToString(T(&arr)[size])
{
	auto ss = stringstream{};
	ss << "(";
	for (size_t i = 0; i < size; ++i)
	{
		ss << arr[i];
		if (i < size - 1)
			ss << ",";
		else
			ss << ")";
	}
	return ss.str();
}

string BoneIdxToString(int idx, const pmx::Model& model)
{
	if (0 <= idx && idx < model.bones.size())
		return model.bones[idx].bone_name;
	else
		return "";
}

void ExportBDEF1(std::ostream& stream, const pmx::Model& model, const pmx::VertexSkinningBDEF1& skinning)
{
	stream << "BDEF1(";
	stream << BoneIdxToString(skinning.bone_index, model) << ":" << 1.0f << ")";
}

void ExportBDEF2(std::ostream& stream, const pmx::Model& model, const pmx::VertexSkinningBDEF2& skinning)
{
	stream << "BDEF2(";
	stream << BoneIdxToString(skinning.bone_index1, model) << ":" << skinning.bone_weight << ",";
	stream << BoneIdxToString(skinning.bone_index2, model) << ":" << (1.0f - skinning.bone_weight) << ")";
}

void ExportBDEF4(std::ostream& stream, const pmx::Model& model, const pmx::VertexSkinningBDEF4& skinning)
{
	stream << "BDEF4(";
	stream << BoneIdxToString(skinning.bone_index1, model) << ":" << skinning.bone_weight1 << ",";
	stream << BoneIdxToString(skinning.bone_index2, model) << ":" << skinning.bone_weight2 << ",";
	stream << BoneIdxToString(skinning.bone_index3, model) << ":" << skinning.bone_weight3 << ",";
	stream << BoneIdxToString(skinning.bone_index4, model) << ":" << skinning.bone_weight4 << ")";
}

void ExportSDEF(std::ostream& stream, const pmx::Model& model, const pmx::VertexSkinningSDEF& skinning)
{
	stream << "SDEF(";
	stream << BoneIdxToString(skinning.bone_index1, model) << ":" << skinning.bone_weight << ",";
	stream << BoneIdxToString(skinning.bone_index2, model) << ":" << (1.0f - skinning.bone_weight) << ")";
}

struct UnknownSkinning : exception {};
void ExportSkinning(std::ostream& stream, const pmx::Model& model, const pmx::Vertex& vertex)
{
	switch (vertex.skinning_type)
	{
	case pmx::VertexSkinningType::BDEF1:
		ExportBDEF1(stream, model, static_cast<const pmx::VertexSkinningBDEF1&>(*vertex.skinning));
		break;
	case pmx::VertexSkinningType::BDEF2:
		ExportBDEF2(stream, model, static_cast<const pmx::VertexSkinningBDEF2&>(*vertex.skinning));
		break;
	case pmx::VertexSkinningType::BDEF4:
		ExportBDEF4(stream, model, static_cast<const pmx::VertexSkinningBDEF4&>(*vertex.skinning));
		break;
	case pmx::VertexSkinningType::SDEF:
		ExportSDEF(stream, model, static_cast<const pmx::VertexSkinningSDEF&>(*vertex.skinning));
		break;
	default:
		throw UnknownSkinning{};
	}
}

void ExportVertices(std::ostream& stream, const pmx::Model& model)
{
	for (size_t i = 0; i < model.vertices.size(); ++i)
	{
		auto& vertex = model.vertices[i];
		stream << "頂点" << i << ": ";
		stream << "位置" << ArrayToString(vertex.positon) << ",";
		stream << "法線" << ArrayToString(vertex.normal) << ",";
		stream << "エッジ倍率" << vertex.edge << ",";
		stream << "UV" << ArrayToString(vertex.uv) << ",";
		for (size_t j = 0; j < model.setting.uv; ++j)
			stream << "追加UV" << (j + 1) << ArrayToString(vertex.uva[j]) << ",";
		ExportSkinning(stream, model, vertex);
		stream << endl;

		break;
	}
}

void ExportFaces(std::ostream& stream, const pmx::Model& model, int offset, int num)
{
	for (int i = offset; i < offset + num; ++i)
	{
		int indices[3];
		for (int j = 0; j < 3; ++j)
			indices[j] = model.indices[static_cast<int64_t>(i) * 3 + j];
		stream << ArrayToString(indices);
		stream << endl;

		break;
	}
}

string ConcatJPENNames(const string& jpName, const string& enName)
{
	auto ss = stringstream{};
	ss << jpName;
	if (enName.length() > 0)
	{
		ss << "(";
		ss << enName;
		ss << ")";
	}
	return ss.str();
}

string TextureIdxToString(int idx, const pmx::Model& model)
{
	if (0 <= idx && idx < model.textures.size())
		return model.textures[idx];
	else
		return "";
}

string ToonIdxToString(int idx)
{
	auto ss = stringstream{};
	ss << "toon";
	ss << std::setfill('0') << std::right << std::setw(2) << (idx + 1);
	ss << ".bmp";
	return ss.str();
}

using USphereMode = uint8_t;
struct UnknownSphereMode : exception {};
string SphereModeToString(USphereMode sphereMode)
{
	switch (sphereMode)
	{
	case 0: return "ｽﾌｨｱ無効";
	case 1: return "乗算ｽﾌｨｱ";
	case 2: return "加算ｽﾌｨｱ";
	case 3: return "サブTex";
	default: throw UnknownSphereMode{};
	}
}

void ExportMaterials(std::ostream& stream, const pmx::Model& model)
{
	int curFaceIdx = 0;
	for (int i = 0; i < model.materials.size(); ++i)
	{
		auto& material = model.materials[i];
		int numFaces = material.index_count / 3;
		float diffuse[3];
		for (int j = 0; j < 3; ++j)
			diffuse[j] = material.diffuse[j];
		bool reversible = (material.flag & 0b00000001) >> 0;
		bool groundShadow = (material.flag & 0b00000010) >> 1;
		bool selfShadowMap = (material.flag & 0b00000100) >> 2;
		bool selfShadow = (material.flag & 0b00001000) >> 3;
		bool drawEdge = (material.flag & 0b00010000) >> 4;
		auto textureFileName = TextureIdxToString(material.diffuse_texture_index, model);
		auto sphereFileName = TextureIdxToString(material.sphere_texture_index, model);
		auto toonFileName = material.common_toon_flag ? ToonIdxToString(material.toon_texture_index) : TextureIdxToString(material.toon_texture_index, model);
		stream << "材質「" << ConcatJPENNames(material.material_name, material.material_english_name) << "」: ";
		stream << "拡散色" << ArrayToString(diffuse) << ",";
		stream << "反射色" << ArrayToString(material.specular) << ",";
		stream << "環境色" << ArrayToString(material.ambient) << ",";
		stream << "非透過度" << material.diffuse[3] << ",";
		stream << "反射強度" << material.specularlity << ",";
		if (reversible)
			stream << "両面描画" << ",";
		if (groundShadow)
			stream << "地面影" << ",";
		if (selfShadowMap)
			stream << "ｾﾙﾌ影ﾏｯﾌﾟ" << ",";
		if (selfShadow)
			stream << "ｾﾙﾌ影" << ",";
		stream << "エッジ" << (drawEdge ? "有効" : "無効") << ",";
		stream << "エッジサイズ" << material.edge_size << ",";
		stream << "エッジ色" << ArrayToString(material.edge_color) << ",";
		if (textureFileName.length() > 0)
			stream << "Tex「" << textureFileName << "」,";
		if (toonFileName.length() > 0)
			stream << "Toon「" << toonFileName << "」,";
		if (sphereFileName.length() > 0)
			stream << "ｽﾌｨｱ「" << sphereFileName << "」,";
		stream << SphereModeToString(material.sphere_op_mode) << endl;
		if (material.memo.length() > 0)
		{
			stream << "----------------" << endl;
			stream << material.memo << endl;
			stream << "----------------" << endl;
		}
		ExportFaces(stream, model, curFaceIdx, numFaces);
		curFaceIdx += numFaces;
		break;
	}
}

void pmx2txt::txt::Export(std::ostream& stream, const pmx::Model& model)
{
	ExportVersion(stream, model);
	ExportSetting(stream, model);
	ExportModelInfo(stream, model);
	ExportVertices(stream, model);
	ExportMaterials(stream, model);
}

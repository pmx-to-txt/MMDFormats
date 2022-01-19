#include "pmx2txt/parser/Txt.h"

#include <iomanip>
#include <sstream>
#include <optional>

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

template<size_t size, typename T>
string ArrayToString(const T(&arr)[size], std::optional<streamsize> precision = nullopt)
{
	auto ss = stringstream{};
	if (precision.has_value())
		ss << std::setprecision(precision.value());
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

static constexpr double pi = 3.14159265;

template<size_t size, typename T>
string RotationArrayToString(const array<T, size>& arr)
{
	auto ss = stringstream{};
	ss << "(";
	for (size_t i = 0; i < size; ++i)
	{
		ss << (180.0 * arr[i] / pi);
		if (i < size - 1)
			ss << ",";
		else
			ss << ")";
	}
	return ss.str();
}

template<size_t size, typename T>
string ArrayToString(const T* arr, std::optional<streamsize> precision = nullopt)
{
	float buf[size];
	for (int j = 0; j < size; ++j)
		buf[j] = arr[j];
	return ArrayToString(buf, precision);
}

template<size_t size, typename T>
string ArrayToString(const array<T, size>& arr, std::optional<streamsize> precision = nullopt)
{
	return ArrayToString<size>(arr.data(), precision);
}

template<size_t size, size_t size2, typename T>
string ArrayToString(const array<T, size2>& arr)
{
	return ArrayToString<size>(arr.data());
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

string BoneIdxToString(int idx, const pmx::Model& model)
{
	if (0 <= idx && idx < model.bones.size())
		return ConcatJPENNames(model.bones[idx].bone_name, model.bones[idx].bone_english_name);
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

void ExportVertice(std::ostream& stream, const pmx::Model& model, const pmx::Vertex& vertex)
{
	stream << "位置" << ArrayToString(vertex.positon) << ",";
	stream << "法線" << ArrayToString(vertex.normal) << ",";
	stream << "エッジ倍率" << vertex.edge << ",";
	stream << "UV" << ArrayToString(vertex.uv) << ",";
	for (size_t j = 0; j < model.setting.uv; ++j)
		stream << "追加UV" << (j + 1) << ArrayToString(vertex.uva[j]) << ",";
	ExportSkinning(stream, model, vertex);
	stream << endl;
}

void ExportVertices(std::ostream& stream, const pmx::Model& model)
{
	for (size_t i = 0; i < model.vertices.size(); ++i)
	{
		auto& vertex = model.vertices[i];
		stream << "頂点" << i << ": ";
		ExportVertice(stream, model, vertex);

#ifdef _DEBUG
		break;
#endif
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

#ifdef _DEBUG
		break;
#endif
	}
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
	case 0: return "- 無効";
	case 1: return "x 乗算ｽﾌｨｱ";
	case 2: return "+ 加算ｽﾌｨｱ";
	case 3: return "サブTex";
	default: throw UnknownSphereMode{};
	}
}

bool GetBit(int flags, int bit)
{
	return (flags & (0x1 << bit)) != 0;
}

void ExportMaterials(std::ostream& stream, const pmx::Model& model)
{
	int curFaceIdx = 0;
	for (int i = 0; i < model.materials.size(); ++i)
	{
		auto& material = model.materials[i];
		int numFaces = material.index_count / 3;
		bool reversible = GetBit(material.flag, 0);
		bool groundShadow = GetBit(material.flag, 1);
		bool selfShadowMap = GetBit(material.flag, 2);
		bool selfShadow = GetBit(material.flag, 3);
		bool drawEdge = GetBit(material.flag, 4);
		auto textureFileName = TextureIdxToString(material.diffuse_texture_index, model);
		auto sphereFileName = TextureIdxToString(material.sphere_texture_index, model);
		auto toonFileName = material.common_toon_flag ? ToonIdxToString(material.toon_texture_index) : TextureIdxToString(material.toon_texture_index, model);
		stream << "材質「" << ConcatJPENNames(material.material_name, material.material_english_name) << "」: " << endl;
		stream << "-拡散色: " << ArrayToString<3>(material.diffuse) << endl;
		stream << "-反射色: " << ArrayToString(material.specular) << endl;
		stream << "-環境色: " << ArrayToString(material.ambient) << endl;
		stream << "-非透過度: " << material.diffuse[3] << endl;
		stream << "-反射強度: " << material.specularlity << endl;
		stream << "-描画フラグ: ";
		stream << (reversible ? "両面描画" : "片面描画") << ",";
		stream << (groundShadow ? "地面影あり" : "地面影なし") << ",";
		stream << (selfShadowMap ? "ｾﾙﾌ影ﾏｯﾌﾟあり" : "ｾﾙﾌ影ﾏｯﾌﾟなし") << ",";
		stream << (selfShadow ? "ｾﾙﾌ影あり" : "ｾﾙﾌ影なし") << endl;
		if (drawEdge)
		{
			stream << "-エッジサイズ: " << material.edge_size << endl;
			stream << "-エッジ色: " << ArrayToString(material.edge_color) << endl;
		}
		if (textureFileName.length() > 0)
			stream << "-Tex: " << textureFileName << endl;
		if (sphereFileName.length() > 0)
			stream << "-スフィア: " << sphereFileName << endl;
		stream << "-スフィアモード: " << SphereModeToString(material.sphere_op_mode) << endl;
		if (toonFileName.length() > 0)
			stream << "-Toon: " << toonFileName << endl;
		if (material.memo.length() > 0)
		{
			stream << "-メモ: " << endl;
			stream << "----------------" << endl;
			stream << material.memo << endl;
			stream << "----------------" << endl;
		}
		ExportFaces(stream, model, curFaceIdx, numFaces);
		curFaceIdx += numFaces;

#ifdef _DEBUG
		break;
#endif
	}
}

void ExportBones(std::ostream& stream, const pmx::Model& model)
{
	for (int i = 0; i < model.bones.size(); ++i)
	{
		auto& bone = model.bones[i];
		bool tipBone = GetBit(bone.bone_flag, 0);
		bool allowRotate = GetBit(bone.bone_flag, 1);
		bool allowTrans = GetBit(bone.bone_flag, 2);
		bool shown = GetBit(bone.bone_flag, 3);
		bool allowControl = GetBit(bone.bone_flag, 4);
		bool ik = GetBit(bone.bone_flag, 5);
		bool localAppend = GetBit(bone.bone_flag, 7);
		bool appendRotate = GetBit(bone.bone_flag, 8);
		bool appendTrans = GetBit(bone.bone_flag, 9);
		bool fixedAxis = GetBit(bone.bone_flag, 10);
		bool localAxis = GetBit(bone.bone_flag, 11);
		bool afterPhysic = GetBit(bone.bone_flag, 12);
		bool externParent = GetBit(bone.bone_flag, 13);
		stream << "ボーン「" << ConcatJPENNames(bone.bone_name, bone.bone_english_name) << "」: ";
		if (allowRotate)
			stream << "回転" << ",";
		if (allowTrans)
			stream << "移動" << ",";
		if (shown)
			stream << "表示" << ",";
		if (allowControl)
			stream << "操作" << ",";
		stream << "位置" << ArrayToString(bone.position) << ",";
		stream << "親ボーン「" << BoneIdxToString(bone.parent_index, model) << "」,";
		if (tipBone)
			stream << "表示先ボーン「" << BoneIdxToString(bone.target_index, model) << "」,";
		else
			stream << "表示先" << ArrayToString(bone.offset) << ",";
		if (appendRotate || appendTrans)
		{
			if (appendRotate)
				stream << "回転+" << ",";
			if (appendTrans)
				stream << "移動+" << ",";
			stream << "付与率" << bone.grant_weight << ",";
			if (localAppend)
				stream << "ﾛｰｶﾙ付与" << ",";
			stream << "付与親「" << BoneIdxToString(bone.grant_parent_index, model) << "」,";
		}
		if (fixedAxis)
			stream << "軸制限" << ArrayToString(bone.lock_axis_orientation) << ",";
		if (localAxis)
		{
			stream << "ﾛｰｶﾙ軸X" << ArrayToString(bone.local_axis_x_orientation, 3) << ",";
			stream << "ﾛｰｶﾙ軸Z" << ArrayToString(bone.local_axis_y_orientation, 3) << ",";
		}
		if (externParent)
			stream << "外部親Key" << bone.key << ",";
		if (ik)
		{
			stream << "Target「" << BoneIdxToString(bone.ik_target_bone_index, model) << "」,";
			stream << "Loop" << bone.ik_loop << ",";
			stream << "単位角" << (180.0 * bone.ik_loop_angle_limit / pi) << ",";
			for (size_t j = 0; j < bone.ik_links.size(); ++j)
			{
				auto& ik_link = bone.ik_links[j];
				stream << "Link「" << BoneIdxToString(ik_link.link_target, model) << "」";
				if (ik_link.angle_lock)
				{
					stream << ArrayToString(ik_link.min_radian) << "-" << ArrayToString(ik_link.max_radian);
				}
				stream << ",";
			}
		}
		if (afterPhysic)
			stream << "物理後" << ",";
		stream << "変形階層" << bone.level << endl;

#ifdef _DEBUG
		break;
#endif
	}
}


void ExportUVMorph(std::ostream& stream, const pmx::Morph& morph)
{
	if (morph.morph_type == pmx::MorphType::UV)
	{
		for (auto& uv_offset : morph.uv_offsets)
		{
			stream << "-頂点" << uv_offset.vertex_index << ": ";
			stream << "UV移動" << ArrayToString<2>(uv_offset.uv_offset) << endl;
#ifdef _DEBUG
			break;
#endif
		}
	}
	else
	{
		for (auto& uv_offset : morph.uv_offsets)
		{
			stream << "-頂点" << uv_offset.vertex_index << ": ";
			stream << "UV移動" << ArrayToString(uv_offset.uv_offset) << endl;
#ifdef _DEBUG
			break;
#endif
		}
	}
}

string MaterialIdxToString(int idx, const pmx::Model& model)
{
	if (0 <= idx && idx < model.materials.size())
		return ConcatJPENNames(model.materials[idx].material_name, model.materials[idx].material_english_name);
	else
		return "";
}

string MorphIdxToString(int idx, const pmx::Model& model)
{
	if (0 <= idx && idx < model.morphs.size())
		return ConcatJPENNames(model.morphs[idx].morph_name, model.morphs[idx].morph_english_name);
	else
		return "";
}

struct UnknownMorphCategory : exception {};
string MorphCategoryToString(pmx::MorphCategory category)
{
	switch (category)
	{
	case pmx::MorphCategory::ReservedCategory: return "（カテゴリ未設定）";
	case pmx::MorphCategory::Eye: return "左上(目)";
	case pmx::MorphCategory::Mouth: return "右上(ﾘｯﾌﾟ)";
	case pmx::MorphCategory::Eyebrow: return "左下(まゆ)";
	case pmx::MorphCategory::Other: return "右下(その他)";
	default: throw UnknownMorphCategory{};
	}
}

struct UnknownMorphType : exception {};
void ExportMorphs(std::ostream& stream, const pmx::Model& model)
{
	for (int i = 0; i < model.morphs.size(); ++i)
	{
		auto& morph = model.morphs[i];
		auto name = ConcatJPENNames(morph.morph_name, morph.morph_english_name);
		switch (morph.morph_type)
		{
		case pmx::MorphType::Vertex:
			stream << "頂点モーフ「" << name << "」[" << MorphCategoryToString(morph.category) << "]: " << endl;
			for (auto& vertex_offset : morph.vertex_offsets)
			{
				stream << "-頂点" << vertex_offset.vertex_index << ": ";
				stream << "移動" << ArrayToString(vertex_offset.position_offset) << endl;
#ifdef _DEBUG
				break;
#endif
			}
			break;
		case pmx::MorphType::UV:
			stream << "UVモーフ「" << name << "」[" << MorphCategoryToString(morph.category) << "]: " << endl;
			ExportUVMorph(stream, morph);
			break;
		case pmx::MorphType::AdditionalUV1:
			stream << "追加UV1モーフ「" << name << "」[" << MorphCategoryToString(morph.category) << "]: " << endl;
			ExportUVMorph(stream, morph);
			break;
		case pmx::MorphType::AdditionalUV2:
			stream << "追加UV2モーフ「" << name << "」[" << MorphCategoryToString(morph.category) << "]: " << endl;
			ExportUVMorph(stream, morph);
			break;
		case pmx::MorphType::AdditionalUV3:
			stream << "追加UV3モーフ「" << name << "」[" << MorphCategoryToString(morph.category) << "]: " << endl;
			ExportUVMorph(stream, morph);
			break;
		case pmx::MorphType::AdditionalUV4:
			stream << "追加UV4モーフ「" << name << "」[" << MorphCategoryToString(morph.category) << "]: " << endl;
			ExportUVMorph(stream, morph);
			break;
		case pmx::MorphType::Bone:
			stream << "ボーンモーフ「" << name << "」[" << MorphCategoryToString(morph.category) << "]: " << endl;
			for (auto& bone_offset : morph.bone_offsets)
			{
				stream << "-ボーン「" << BoneIdxToString(bone_offset.bone_index, model) << "」: ";
				stream << "移動" << ArrayToString(bone_offset.translation) << ",";
				stream << "回転クォータニオン" << ArrayToString(bone_offset.rotation) << endl;
#ifdef _DEBUG
				break;
#endif
			}
			break;
		case pmx::MorphType::Matrial:
			stream << "材質モーフ「" << name << "」[" << MorphCategoryToString(morph.category) << "]: " << endl;
			for (auto& material_offset : morph.material_offsets)
			{
				stream << "-材質「" << MaterialIdxToString(material_offset.material_index, model) << "」: ";
				stream << (material_offset.offset_operation ? "加算" : "乗算") << ",";
				stream << "拡散色" << ArrayToString<3>(material_offset.diffuse) << ",";
				stream << "反射色" << ArrayToString(material_offset.specular) << ",";
				stream << "環境色" << ArrayToString(material_offset.ambient) << ",";
				stream << "非透過度" << material_offset.diffuse[3] << ",";
				stream << "反射強度" << material_offset.specularity << ",";
				stream << "エッジサイズ" << material_offset.edge_size << ",";
				stream << "エッジ色" << ArrayToString(material_offset.edge_color) << ",";
				stream << "Tex" << ArrayToString(material_offset.texture_argb) << ",";
				stream << "ｽﾌｨｱ" << ArrayToString(material_offset.sphere_texture_argb) << ",";
				stream << "Toon" << ArrayToString(material_offset.toon_texture_argb) << endl;
#ifdef _DEBUG
				break;
#endif
			}
			break;
		case pmx::MorphType::Group:
			stream << "グループモーフ「" << name << "」[" << MorphCategoryToString(morph.category) << "]: " << endl;
			for (auto& group_offset : morph.group_offsets)
			{
				stream << "-モーフ「" << MorphIdxToString(group_offset.morph_index, model) << "」: ";
				stream << "影響度" << group_offset.morph_weight << endl;
#ifdef _DEBUG
				break;
#endif
			}
			break;
		default:
			throw UnknownMorphType{};
		}

#ifdef _DEBUG
		break;
#endif
	}
}

void ExportFrames(std::ostream& stream, const pmx::Model& model)
{
	for (auto& frame : model.frames)
	{
		stream << "表示枠「" << ConcatJPENNames(frame.frame_name, frame.frame_english_name) << "」: " << endl;
		for (auto& element : frame.elements)
		{
			if (element.element_target == 0)
				stream << "-ボーン「" << BoneIdxToString(element.index, model) << "」" << endl;
			else
				stream << "-モーフ「" << MorphIdxToString(element.index, model) << "」" << endl;

#ifdef _DEBUG
			break;
#endif
		}
	}
}

using URigidBodyShape = uint8_t;
struct UnknownRigidBodyShape : exception {};
string RigidBodyShapeToString(URigidBodyShape shape)
{
	switch (shape)
	{
	case 0: return "球";
	case 1: return "箱";
	case 2: return "カプセル";
	default: throw UnknownRigidBodyShape{};
	}
}

string RigidBodyShapeToSizeString(URigidBodyShape shape, const array<float, 3> &size)
{
	stringstream ss;
	switch (shape)
	{
	case 0:
		ss << "半径" << size[0];
		break;
	case 1:
		ss << "幅" << size[0] << "," << "高さ" << size[1] << "," << "奥行" << size[2];
		break;
	case 2:
		ss << "半径" << size[0] << "," << "高さ" << size[1];
		break;
	default:
		throw UnknownRigidBodyShape{};
	}
	return ss.str();
}


struct UnknownRigidBodyType : exception {};
void ExportRigidBodies(std::ostream& stream, const pmx::Model& model)
{
	for (auto& body : model.rigid_bodies)
	{
		auto name = ConcatJPENNames(body.rigid_body_name, body.rigid_body_english_name);
		switch (body.physics_calc_type)
		{
		case 0:
			stream << "ボーン追従剛体「" << name << "」: " << endl;
			break;
		case 1:
			stream << "物理演算剛体「" << name << "」: " << endl;
			break;
		case 2:
			stream << "物理+ボーン位置合わせ剛体「" << name << "」: " << endl;
			break;
		default:
			throw UnknownRigidBodyType{};
		}
		stream << "-関連ボーン: " << BoneIdxToString(body.target_bone, model) << endl;
		stream << "-形状: " << RigidBodyShapeToString(body.shape) << endl;
		stream << "-グループ: " << static_cast<int>(body.group + 1) << endl;
		stream << "-非衝突グループ: ";
		bool first = true;
		for (int i = 0; i < 16; ++i)
		{
			if (!GetBit(body.mask, i))
			{
				if (first)
					first = false;
				else
					stream << ",";
				stream << (i + 1);
			}
		}
		stream << endl;
		stream << "-サイズ: " << RigidBodyShapeToSizeString(body.shape, body.size) << endl;
		stream << "-位置: " << ArrayToString(body.position) << endl;
		stream << "-回転: " << RotationArrayToString(body.orientation) << endl;
		stream << "-質量: " << body.mass << endl;
		stream << "-移動減衰: " << body.move_attenuation << endl;
		stream << "-回転減衰: " << body.rotation_attenuation << endl;
		stream << "-反発力: " << body.repulsion << endl;
		stream << "-摩擦力: " << body.friction << endl;

#ifdef _DEBUG
		break;
#endif
	}
}

string RigidBodyIdxToString(int idx, const pmx::Model& model)
{
	if (0 <= idx && idx < model.rigid_bodies.size())
		return ConcatJPENNames(model.rigid_bodies[idx].rigid_body_name, model.rigid_bodies[idx].rigid_body_english_name);
	else
		return "";
}

void ExportJoints(std::ostream& stream, const pmx::Model& model)
{
	for (auto& joint : model.joints)
	{
		stream << "ジョイント「" << ConcatJPENNames(joint.joint_name, joint.joint_english_name) << "」: " << endl;
		stream << "-接続剛体A: " << RigidBodyIdxToString(joint.param.rigid_body1, model) << endl;
		stream << "-接続剛体B: " << RigidBodyIdxToString(joint.param.rigid_body2, model) << endl;
		stream << "-位置: " << ArrayToString(joint.param.position) << endl;
		stream << "-回転: " << RotationArrayToString(joint.param.orientaiton) << endl;
		stream << "-移動制限: " << ArrayToString(joint.param.move_limitation_min) << "-" << ArrayToString(joint.param.move_limitation_max) << endl;
		stream << "-回転制限: " << RotationArrayToString(joint.param.rotation_limitation_min) << "-" << RotationArrayToString(joint.param.rotation_limitation_max) << endl;
		stream << "-移動ばね: " << ArrayToString(joint.param.spring_move_coefficient) << endl;
		stream << "-回転ばね: " << ArrayToString(joint.param.spring_rotation_coefficient) << endl;

#ifdef _DEBUG
		break;
#endif
	}
}

void pmx2txt::txt::Export(std::ostream& stream, const pmx::Model& model)
{
	ExportVersion(stream, model);
	ExportSetting(stream, model);
	ExportModelInfo(stream, model);
	ExportVertices(stream, model);
	ExportMaterials(stream, model);
	ExportBones(stream, model);
	ExportMorphs(stream, model);
	ExportFrames(stream, model);
	ExportRigidBodies(stream, model);
	ExportJoints(stream, model);
}

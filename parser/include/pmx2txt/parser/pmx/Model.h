#pragma once

#include <iostream>
#include <string>
#include <vector>

#include "pmx2txt/parser/pmx/Bone.h"
#include "pmx2txt/parser/pmx/Material.h"
#include "pmx2txt/parser/pmx/Vertex.h"
#include "pmx2txt/parser/pmx/Morph.h"
#include "pmx2txt/parser/pmx/Frame.h"
#include "pmx2txt/parser/pmx/RigidBody.h"
#include "pmx2txt/parser/pmx/SoftBody.h"
#include "pmx2txt/parser/pmx/Joint.h"

namespace pmx
{
	/// PMXモデル
	class Model
	{
	public:
		/// バージョン
		float version;
		/// 設定
		pmx::Setting setting;
		/// モデル名
		std::string model_name;
		/// モデル英名
		std::string model_english_name;
		/// コメント
		std::string model_comment;
		/// 英語コメント
		std::string model_english_comment;
		/// 頂点配列
		std::vector<pmx::Vertex> vertices;
		/// インデックス配列
		std::vector<int> indices;
		/// テクスチャ配列
		std::vector<std::string> textures;
		/// マテリアル
		std::vector<pmx::Material> materials;
		/// ボーン配列
		std::vector<pmx::Bone> bones;
		/// モーフ配列
		std::vector<pmx::Morph> morphs;
		/// 表示枠配列
		std::vector<pmx::Frame> frames;
		/// 剛体配列
		std::vector<pmx::RigidBody> rigid_bodies;
		/// ジョイント配列
		std::vector<pmx::Joint> joints;
		/// ソフトボディ配列
		std::vector<pmx::SoftBody> soft_bodies;

	public:
		Model() noexcept;
		void parse(std::istream& stream);
		std::size_t dump(std::ostream& stream);
	};
}

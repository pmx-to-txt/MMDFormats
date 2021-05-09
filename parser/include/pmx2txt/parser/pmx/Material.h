#pragma once

#include <array>
#include <iostream>
#include <string>

#include "pmx2txt/parser/pmx/Setting.h"

namespace pmx
{
	/// マテリアル
	class Material
	{
	private:
		const pmx::Setting& setting;
	public:
		/// モデル名
		std::string material_name;
		/// モデル英名
		std::string material_english_name;
		/// 減衰色
		std::array<float, 4> diffuse;
		/// 光沢色
		std::array<float, 3> specular;
		/// 光沢度
		float specularlity;
		/// 環境色
		std::array<float, 3> ambient;
		/// 描画フラグ
		uint8_t flag;
		/// エッジ色
		std::array<float, 4> edge_color;
		/// エッジサイズ
		float edge_size;
		/// アルベドテクスチャインデックス
		int diffuse_texture_index;
		/// スフィアテクスチャインデックス
		int sphere_texture_index;
		/// スフィアテクスチャ演算モード
		uint8_t sphere_op_mode;
		/// 共有トゥーンフラグ
		uint8_t common_toon_flag;
		/// トゥーンテクスチャインデックス
		int toon_texture_index;
		/// メモ
		std::string memo;
		/// 頂点インデックス数
		int index_count;

	public:
		Material(const pmx::Setting& setting_) noexcept;
		void parse(std::istream& stream);
		std::size_t dump(std::ostream& stream);
	};
}

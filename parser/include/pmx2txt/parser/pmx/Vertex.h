#pragma once

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <memory>

#include "pmx2txt/parser/pmx/enum.h"
#include "pmx2txt/parser/pmx/Setting.h"
#include "pmx2txt/parser/pmx/VertexSkinning.h"

namespace pmx
{
	class Vertex
	{
	private:
		const pmx::Setting& setting;
	public:
		/// 位置
		float positon[3];
		/// 法線
		float normal[3];
		/// テクスチャ座標
		float uv[2];
		/// 追加テクスチャ座標
		float uva[4][4];
		/// スキニングタイプ
		pmx::VertexSkinningType skinning_type{ pmx::VertexSkinningType::BDEF1 };
		/// スキニング
		std::unique_ptr<pmx::VertexSkinning> skinning{ nullptr };
		/// エッジ倍率
		float edge;

	public:
		Vertex(const pmx::Setting& setting_) noexcept;
		void parse(std::istream& stream);
		std::size_t dump(std::ostream& stream);
	};
}

#pragma once

#include <array>
#include <vector>
#include <string>
#include <iostream>
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
		std::array<float,3> positon;
		/// 法線
		std::array<float, 3> normal;
		/// テクスチャ座標
		std::array<float, 2> uv;
		/// 追加テクスチャ座標
		std::array<std::array<float, 4>, 4> uva;
		/// スキニングタイプ
		pmx::VertexSkinningType skinning_type;
		/// スキニング
		std::unique_ptr<pmx::VertexSkinning> skinning{ nullptr };
		/// エッジ倍率
		float edge;

	public:
		Vertex(const pmx::Setting& setting_) noexcept;
		void parse(std::istream& stream);
		std::size_t dump(std::ostream& stream) const;
	};
}

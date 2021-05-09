#pragma once

#include <array>
#include <iostream>

#include "pmx2txt/parser/pmx/Setting.h"

namespace pmx
{
	/// リンク
	class IkLink
	{
	private:
		const pmx::Setting& setting;
	public:
		/// リンクボーンインデックス
		int link_target;
		/// 角度制限
		uint8_t angle_lock;
		/// 最大制限角度
		std::array<float, 3> max_radian;
		/// 最小制限角度
		std::array<float, 3> min_radian;

	public:
		IkLink(const pmx::Setting& setting_) noexcept;
		void parse(std::istream& stream);
		std::size_t dump(std::ostream& stream);
	};
}

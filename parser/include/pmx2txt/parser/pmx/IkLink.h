#pragma once

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
		float max_radian[3];
		/// 最小制限角度
		float min_radian[3];

	public:
		IkLink(const pmx::Setting& setting_) noexcept;
		void parse(std::istream& stream);
		std::size_t dump(std::ostream& stream);
	};
}

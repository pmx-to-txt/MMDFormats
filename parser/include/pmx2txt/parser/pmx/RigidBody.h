#pragma once

#include <array>
#include <iostream>
#include <string>

#include "pmx2txt/parser/pmx/Setting.h"

namespace pmx
{
	class RigidBody
	{
	private:
		const pmx::Setting& setting;
	public:
		/// 剛体名
		std::string rigid_body_name;
		/// 剛体英名
		std::string rigid_body_english_name;
		/// 関連ボーンインデックス
		int target_bone;
		/// グループ
		uint8_t group;
		/// マスク
		uint16_t mask;
		/// 形状
		uint8_t shape;
		std::array<float, 3> size; // dimension
		std::array<float, 3> position;
		std::array<float, 3> orientation;
		float mass;
		float move_attenuation;
		float rotation_attenuation;
		float repulsion;
		float friction;
		uint8_t physics_calc_type;

	public:
		RigidBody(const pmx::Setting& setting_) noexcept;
		void parse(std::istream& stream);
		std::size_t dump(std::ostream& stream) const;
	};
}

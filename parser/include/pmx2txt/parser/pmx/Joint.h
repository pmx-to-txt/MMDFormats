#pragma once

#include <iostream>

#include "pmx2txt/parser/pmx/enum.h"
#include "pmx2txt/parser/pmx/Setting.h"

namespace pmx
{
	class JointParam
	{
	private:
		const pmx::Setting& setting;
	public:
		int rigid_body1;
		int rigid_body2;
		float position[3];
		float orientaiton[3];
		float move_limitation_min[3];
		float move_limitation_max[3];
		float rotation_limitation_min[3];
		float rotation_limitation_max[3];
		float spring_move_coefficient[3];
		float spring_rotation_coefficient[3];

	public:
		JointParam(const pmx::Setting& setting_) noexcept;
		void parse(std::istream& stream);
		std::size_t dump(std::ostream& stream);
	};

	class Joint
	{
	private:
		const pmx::Setting& setting;
	public:
		std::string joint_name;
		std::string joint_english_name;
		JointType joint_type = static_cast<JointType>(-1);
		JointParam param;

	public:
		Joint(const pmx::Setting& setting_) noexcept;
		void parse(std::istream& stream);
		std::size_t dump(std::ostream& stream);
	};
}

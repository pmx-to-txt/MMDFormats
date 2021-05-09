#pragma once

#include <array>
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
		std::array<float, 3> position;
		std::array<float, 3> orientaiton;
		std::array<float, 3> move_limitation_min;
		std::array<float, 3> move_limitation_max;
		std::array<float, 3> rotation_limitation_min;
		std::array<float, 3> rotation_limitation_max;
		std::array<float, 3> spring_move_coefficient;
		std::array<float, 3> spring_rotation_coefficient;

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

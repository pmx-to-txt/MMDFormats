#pragma once

#include <iostream>

#include "pmx2txt/parser/pmx/Setting.h"

namespace pmx
{
	class AnchorRigidBody
	{
	private:
		const pmx::Setting& setting;
	public:
		int related_rigid_body;
		int related_vertex;
		bool is_near;

	public:
		AnchorRigidBody(const pmx::Setting& setting_) noexcept;
		void parse(std::istream& stream);
		std::size_t dump(std::ostream& stream);
	};
}

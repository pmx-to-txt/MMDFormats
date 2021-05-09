#pragma once

#include <array>
#include <iostream>

#include "pmx2txt/parser/pmx/Setting.h"

namespace pmx
{
	class VertexSkinning
	{
	protected:
		const pmx::Setting& setting;
	public:
		VertexSkinning(const pmx::Setting& setting_) noexcept;
		virtual void parse(std::istream& stream) = 0;
		virtual std::size_t dump(std::ostream& stream) = 0;
	};

	class VertexSkinningBDEF1 : public VertexSkinning
	{
	public:
		int bone_index;

	public:
		VertexSkinningBDEF1(const pmx::Setting& setting_) noexcept;
		void parse(std::istream& stresam) override;
		std::size_t dump(std::ostream& stream) override;
	};

	class VertexSkinningBDEF2 : public VertexSkinning
	{
	public:
		int bone_index1;
		int bone_index2;
		float bone_weight;

	public:
		VertexSkinningBDEF2(const pmx::Setting& setting_) noexcept;
		void parse(std::istream& stresam)override;
		std::size_t dump(std::ostream& stream) override;
	};

	class VertexSkinningBDEF4 : public VertexSkinning
	{
	public:
		int bone_index1;
		int bone_index2;
		int bone_index3;
		int bone_index4;
		float bone_weight1;
		float bone_weight2;
		float bone_weight3;
		float bone_weight4;

	public:
		VertexSkinningBDEF4(const pmx::Setting& setting_) noexcept;
		void parse(std::istream& stresam)override;
		std::size_t dump(std::ostream& stream) override;
	};

	class VertexSkinningSDEF : public VertexSkinning
	{
	public:
		int bone_index1;
		int bone_index2;
		float bone_weight;
		std::array<float,3> sdef_c;
		std::array<float, 3> sdef_r0;
		std::array<float, 3> sdef_r1;

	public:
		VertexSkinningSDEF(const pmx::Setting& setting_) noexcept;
		void parse(std::istream& stresam)override;
		std::size_t dump(std::ostream& stream) override;
	};

	class VertexSkinningQDEF : public VertexSkinning
	{
	public:
		int bone_index1;
		int bone_index2;
		int bone_index3;
		int bone_index4;
		float bone_weight1;
		float bone_weight2;
		float bone_weight3;
		float bone_weight4;

	public:
		VertexSkinningQDEF(const pmx::Setting& setting_) noexcept;
		void parse(std::istream& stresam)override;
		std::size_t dump(std::ostream& stream) override;
	};
}

#pragma once

#include <array>
#include <vector>
#include <string>
#include <iostream>

#include "pmx2txt/parser/pmx/enum.h"
#include "pmx2txt/parser/pmx/Setting.h"

namespace pmx
{
	class MorphOffset
	{
	protected:
		const pmx::Setting& setting;
	public:
		MorphOffset(const pmx::Setting& setting_) noexcept;
		void virtual parse(std::istream& stream) = 0;
		std::size_t virtual dump(std::ostream& stream) = 0;
	};

	class MorphVertexOffset : public MorphOffset
	{
	public:
		int vertex_index;
		std::array<float,3> position_offset;

	public:
		MorphVertexOffset(const pmx::Setting& setting_) noexcept;
		void parse(std::istream& stream) override;
		std::size_t dump(std::ostream& stream)override;
	};

	class MorphUVOffset : public MorphOffset
	{
	public:
		int vertex_index;
		std::array<float, 4> uv_offset;

	public:
		MorphUVOffset(const pmx::Setting& setting_) noexcept;
		void parse(std::istream& stream) override;
		std::size_t dump(std::ostream& stream)override;
	};

	class MorphBoneOffset : public MorphOffset
	{
	public:
		int bone_index;
		std::array<float, 3> translation;
		std::array<float, 4> rotation;

	public:
		MorphBoneOffset(const pmx::Setting& setting_) noexcept;
		void parse(std::istream& stream) override;
		std::size_t dump(std::ostream& stream)override;
	};

	class MorphMaterialOffset : public MorphOffset
	{
	public:
		int material_index = -1;
		uint8_t offset_operation = -1;
		std::array<float, 4> diffuse;
		std::array<float, 3> specular;
		float specularity;
		std::array<float, 3> ambient;
		std::array<float, 4> edge_color;
		float edge_size;
		std::array<float, 4> texture_argb;
		std::array<float, 4> sphere_texture_argb;
		std::array<float, 4> toon_texture_argb;

	public:
		MorphMaterialOffset(const pmx::Setting& setting_)noexcept;
		void parse(std::istream& stream) override;
		std::size_t dump(std::ostream& stream)override;
	};

	class MorphGroupOffset : public MorphOffset
	{
	public:
		int morph_index;
		float morph_weight;

	public:
		MorphGroupOffset(const pmx::Setting& setting_) noexcept;
		void parse(std::istream& stream) override;
		std::size_t dump(std::ostream& stream)override;
	};

	class MorphFlipOffset : public MorphOffset
	{
	public:
		int morph_index;
		float morph_value;

	public:
		MorphFlipOffset(const pmx::Setting& setting_) noexcept;
		void parse(std::istream& stream) override;
		std::size_t dump(std::ostream& stream)override;
	};

	class MorphImpulseOffset : public MorphOffset
	{
	public:
		int rigid_body_index;
		uint8_t is_local;
		std::array<float, 3> velocity;
		std::array<float, 3> angular_torque;

	public:
		MorphImpulseOffset(const pmx::Setting& setting_) noexcept;
		void parse(std::istream& stream) override;
		std::size_t dump(std::ostream& stream)override;
	};

	/// モーフ
	class Morph
	{
	private:
		const pmx::Setting& setting;
	public:
		/// モーフ名
		std::string morph_name;
		/// モーフ英名
		std::string morph_english_name;
		/// カテゴリ
		MorphCategory category;
		/// モーフタイプ
		MorphType morph_type;
		/// オフセット数
		int offset_count;
		/// 頂点モーフ配列
		std::vector<MorphVertexOffset> vertex_offsets;
		/// UVモーフ配列
		std::vector<MorphUVOffset> uv_offsets;
		/// ボーンモーフ配列
		std::vector<MorphBoneOffset> bone_offsets;
		/// マテリアルモーフ配列
		std::vector<MorphMaterialOffset> material_offsets;
		/// グループモーフ配列
		std::vector<MorphGroupOffset> group_offsets;
		/// フリップモーフ配列
		std::vector<MorphFlipOffset> flip_offsets;
		/// インパルスモーフ配列
		std::vector<MorphImpulseOffset> Impulse_offsets;

	public:
		Morph(const pmx::Setting& setting_) noexcept;
		void parse(std::istream& stream);
		std::size_t dump(std::ostream& stream);
	};

}
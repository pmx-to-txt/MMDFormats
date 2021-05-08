#pragma once

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <memory>

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
		float position_offset[3];

	public:
		MorphVertexOffset(const pmx::Setting& setting_) noexcept;
		void parse(std::istream& stream) override;
		std::size_t dump(std::ostream& stream)override;
	};

	class MorphUVOffset : public MorphOffset
	{
	public:
		int vertex_index;
		float uv_offset[4];

	public:
		MorphUVOffset(const pmx::Setting& setting_) noexcept;
		void parse(std::istream& stream) override;
		std::size_t dump(std::ostream& stream)override;
	};

	class MorphBoneOffset : public MorphOffset
	{
	public:
		int bone_index;
		float translation[3];
		float rotation[4];

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
		float diffuse[4];
		float specular[3];
		float specularity;
		float ambient[3];
		float edge_color[4];
		float edge_size;
		float texture_argb[4];
		float sphere_texture_argb[4];
		float toon_texture_argb[4];

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
		float velocity[3];
		float angular_torque[3];

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
		MorphCategory category = static_cast<MorphCategory>(-1);
		/// モーフタイプ
		MorphType morph_type = static_cast<MorphType>(-1);
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
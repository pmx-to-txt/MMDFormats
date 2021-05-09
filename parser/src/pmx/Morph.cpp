#include <stdexcept>
#include <utility>

#include "pmx2txt/parser/pmx/Morph.h"

#include "pmx2txt/parser/pmx/enum.h"
#include "pmx2txt/parser/pmx/util.h"

pmx::MorphOffset::MorphOffset(const pmx::Setting& setting_) noexcept
	: setting(setting_)
{
}

pmx::MorphVertexOffset::MorphVertexOffset(const pmx::Setting& setting_) noexcept
	: pmx::MorphOffset(setting_)
	, vertex_index(0)
	, position_offset({ 0.0f })
{
}

void pmx::MorphVertexOffset::parse(std::istream& stream)
{
	this->vertex_index = pmx::util::parseIndex(stream, this->setting.vertex_index_size);
	stream.read((char*)this->position_offset.data(), sizeof(float) * this->position_offset.size());
}

std::size_t pmx::MorphVertexOffset::dump(std::ostream& stream) const
{
	std::size_t total{ 0 };
	total += pmx::util::dumpIndex(stream, this->vertex_index, this->setting.vertex_index_size);
	stream.write(static_cast<const char*>(static_cast<const void*>(this->position_offset.data())), sizeof(float) * this->position_offset.size());
	total += sizeof(float) * 3;
	return total;
}

pmx::MorphUVOffset::MorphUVOffset(const pmx::Setting& setting_) noexcept
	: pmx::MorphOffset(setting_)
	, vertex_index(0)
	, uv_offset({ 0.0f })
{
}

void pmx::MorphUVOffset::parse(std::istream& stream)
{
	this->vertex_index = pmx::util::parseIndex(stream, this->setting.vertex_index_size);
	stream.read((char*)this->uv_offset.data(), sizeof(float) * this->uv_offset.size());
}

std::size_t pmx::MorphUVOffset::dump(std::ostream& stream) const
{
	std::size_t total{ 0 };
	total += pmx::util::dumpIndex(stream, this->vertex_index, this->setting.vertex_index_size);
	stream.write(static_cast<const char*>(static_cast<const void*>(this->uv_offset.data())), sizeof(float) * this->uv_offset.size());
	total += sizeof(float) * 4;
	return total;
}

pmx::MorphBoneOffset::MorphBoneOffset(const pmx::Setting& setting_) noexcept
	: pmx::MorphOffset(setting_)
	, bone_index(0)
	, translation({ 0.0f })
	, rotation({ 0.0f })
{
}

void pmx::MorphBoneOffset::parse(std::istream& stream)
{
	this->bone_index = pmx::util::parseIndex(stream, this->setting.bone_index_size);
	stream.read((char*)this->translation.data(), sizeof(float) * this->translation.size());
	stream.read((char*)this->rotation.data(), sizeof(float) * this->rotation.size());
}

std::size_t pmx::MorphBoneOffset::dump(std::ostream& stream) const
{
	std::size_t total{ 0 };
	total += pmx::util::dumpIndex(stream, this->bone_index, this->setting.bone_index_size);
	stream.write(static_cast<const char*>(static_cast<const void*>(this->translation.data())), sizeof(float) * this->translation.size());
	stream.write(static_cast<const char*>(static_cast<const void*>(this->rotation.data())), sizeof(float) * this->rotation.size());
	total += sizeof(float) * 7;
	return total;
}

pmx::MorphMaterialOffset::MorphMaterialOffset(const pmx::Setting& setting_) noexcept
	: pmx::MorphOffset(setting_)
	, material_index(-1)
	, offset_operation(255)
	, diffuse({ 0.0f })
	, specular({ 0.0f })
	, specularity(0.0f)
	, ambient({ 0.0f })
	, edge_color({ 0.0f })
	, edge_size(0.0f)
	, texture_argb({ 0.0f })
	, sphere_texture_argb({ 0.0f })
	, toon_texture_argb({ 0.0f })
{
}

void pmx::MorphMaterialOffset::parse(std::istream& stream)
{
	this->material_index = pmx::util::parseIndex(stream, this->setting.material_index_size);
	stream.read((char*)&this->offset_operation, sizeof(uint8_t));
	stream.read((char*)this->diffuse.data(), sizeof(float) * this->diffuse.size());
	stream.read((char*)this->specular.data(), sizeof(float) * this->specular.size());
	stream.read((char*)&this->specularity, sizeof(float));
	stream.read((char*)this->ambient.data(), sizeof(float) * this->ambient.size());
	stream.read((char*)this->edge_color.data(), sizeof(float) * this->edge_color.size());
	stream.read((char*)&this->edge_size, sizeof(float));
	stream.read((char*)this->texture_argb.data(), sizeof(float) * this->texture_argb.size());
	stream.read((char*)this->sphere_texture_argb.data(), sizeof(float) * this->sphere_texture_argb.size());
	stream.read((char*)this->toon_texture_argb.data(), sizeof(float) * this->toon_texture_argb.size());
}

std::size_t pmx::MorphMaterialOffset::dump(std::ostream& stream) const
{
	std::size_t total{ 0 };
	total += pmx::util::dumpIndex(stream, this->material_index, this->setting.material_index_size);
	stream.write(static_cast<const char*>(static_cast<const void*>(&this->offset_operation)), sizeof(uint8_t));
	stream.write(static_cast<const char*>(static_cast<const void*>(this->diffuse.data())), sizeof(float) * this->diffuse.size());
	stream.write(static_cast<const char*>(static_cast<const void*>(this->specular.data())), sizeof(float) * this->specular.size());
	stream.write(static_cast<const char*>(static_cast<const void*>(&this->specularity)), sizeof(float));
	stream.write(static_cast<const char*>(static_cast<const void*>(this->ambient.data())), sizeof(float) * this->ambient.size());
	stream.write(static_cast<const char*>(static_cast<const void*>(this->edge_color.data())), sizeof(float) * this->edge_color.size());
	stream.write(static_cast<const char*>(static_cast<const void*>(&this->edge_size)), sizeof(float));
	stream.write(static_cast<const char*>(static_cast<const void*>(this->texture_argb.data())), sizeof(float) * this->texture_argb.size());
	stream.write(static_cast<const char*>(static_cast<const void*>(this->sphere_texture_argb.data())), sizeof(float) * this->sphere_texture_argb.size());
	stream.write(static_cast<const char*>(static_cast<const void*>(this->toon_texture_argb.data())), sizeof(float) * this->toon_texture_argb.size());
	total += sizeof(float) * 28;
	return total;
}

pmx::MorphGroupOffset::MorphGroupOffset(const pmx::Setting& setting_) noexcept
	: pmx::MorphOffset(setting_)
	, morph_index(0)
	, morph_weight(0.0f)
{}

void pmx::MorphGroupOffset::parse(std::istream& stream)
{
	this->morph_index = pmx::util::parseIndex(stream, this->setting.morph_index_size);
	stream.read((char*)&this->morph_weight, sizeof(float));
}

std::size_t pmx::MorphGroupOffset::dump(std::ostream& stream) const
{
	std::size_t total{ 0 };
	total += pmx::util::dumpIndex(stream, this->morph_index, this->setting.morph_index_size);
	stream.write(static_cast<const char*>(static_cast<const void*>(&this->morph_weight)), sizeof(float));
	total += sizeof(float);
	return total;
}

pmx::MorphFlipOffset::MorphFlipOffset(const pmx::Setting& setting_) noexcept
	: pmx::MorphOffset(setting_)
	, morph_index(0)
	, morph_value(0.0f)
{}

void pmx::MorphFlipOffset::parse(std::istream& stream)
{
	this->morph_index = pmx::util::parseIndex(stream, this->setting.morph_index_size);
	stream.read((char*)&this->morph_value, sizeof(float));
}

std::size_t pmx::MorphFlipOffset::dump(std::ostream& stream) const
{
	std::size_t total{ 0 };
	total += pmx::util::dumpIndex(stream, this->morph_index, this->setting.morph_index_size);
	stream.write(static_cast<const char*>(static_cast<const void*>(&this->morph_value)), sizeof(float));
	total += sizeof(float);
	return total;
}

pmx::MorphImpulseOffset::MorphImpulseOffset(const pmx::Setting& setting_) noexcept
	: pmx::MorphOffset(setting_)
	, rigid_body_index(0)
	, is_local(0)
	, velocity({0.0f})
	, angular_torque({ 0.0f })
{
}

void pmx::MorphImpulseOffset::parse(std::istream& stream)
{
	this->rigid_body_index = pmx::util::parseIndex(stream, this->setting.rigidbody_index_size);
	stream.read((char*)&this->is_local, sizeof(uint8_t));
	stream.read((char*)this->velocity.data(), sizeof(float) * this->velocity.size());
	stream.read((char*)this->angular_torque.data(), sizeof(float) * this->angular_torque.size());
}

std::size_t pmx::MorphImpulseOffset::dump(std::ostream& stream) const
{
	std::size_t total{ 0 };
	total += pmx::util::dumpIndex(stream, this->rigid_body_index, this->setting.rigidbody_index_size);
	stream.write(static_cast<const char*>(static_cast<const void*>(&this->is_local)), sizeof(uint8_t));
	stream.write(static_cast<const char*>(static_cast<const void*>(this->velocity.data())), sizeof(float) * this->velocity.size());
	stream.write(static_cast<const char*>(static_cast<const void*>(this->angular_torque.data())), sizeof(float) * this->angular_torque.size());
	total += sizeof(float) * 6 + sizeof(uint8_t);
	return total;
}

pmx::Morph::Morph(const pmx::Setting& setting_) noexcept
	: setting(setting_)
	, morph_name()
	, morph_english_name()
	, category(static_cast<pmx::MorphCategory>(-1))
	, morph_type(static_cast<pmx::MorphType>(-1))
	, offset_count(0)
	, vertex_offsets()
	, uv_offsets()
	, bone_offsets()
	, material_offsets()
	, group_offsets()
	, flip_offsets()
	, Impulse_offsets()
{
}

void pmx::Morph::parse(std::istream& stream)
{
	this->morph_name = pmx::util::parseString(stream, this->setting.encoding);
	this->morph_english_name = pmx::util::parseString(stream, this->setting.encoding);
	stream.read((char*)&category, sizeof(MorphCategory));
	stream.read((char*)&morph_type, sizeof(MorphType));
	stream.read((char*)&this->offset_count, sizeof(int));
	switch (this->morph_type)
	{
	case MorphType::Group:
		this->group_offsets.reserve(this->offset_count);
		for (auto i = 0; i < this->offset_count; ++i)
		{
			this->group_offsets.emplace_back(this->setting).parse(stream);
		}
		break;
	case MorphType::Vertex:
		this->vertex_offsets.reserve(this->offset_count);
		for (auto i = 0; i < this->offset_count; ++i)
		{
			this->vertex_offsets.emplace_back(this->setting).parse(stream);
		}
		break;
	case MorphType::Bone:
		this->bone_offsets.reserve(this->offset_count);
		for (auto i = 0; i < this->offset_count; ++i)
		{
			this->bone_offsets.emplace_back(this->setting).parse(stream);
		}
		break;
	case MorphType::Matrial:
		this->material_offsets.reserve(this->offset_count);
		for (auto i = 0; i < this->offset_count; ++i)
		{
			this->material_offsets.emplace_back(this->setting).parse(stream);
		}
		break;
	case MorphType::UV:
	case MorphType::AdditionalUV1:
	case MorphType::AdditionalUV2:
	case MorphType::AdditionalUV3:
	case MorphType::AdditionalUV4:
		this->uv_offsets.reserve(this->offset_count);
		for (auto i = 0; i < this->offset_count; ++i)
		{
			this->uv_offsets.emplace_back(this->setting).parse(stream);
		}
		break;
	default:
		throw;
	}
}

std::size_t pmx::Morph::dump(std::ostream& stream) const
{
	std::size_t total{ 0 };
	total += pmx::util::dumpString(stream, this->morph_name, this->setting.encoding);
	total += pmx::util::dumpString(stream, this->morph_english_name, this->setting.encoding);


	stream.write(static_cast<const char*>(static_cast<const void*>(&this->category)), sizeof(MorphCategory));
	total += sizeof(MorphCategory);

	stream.write(static_cast<const char*>(static_cast<const void*>(&this->morph_type)), sizeof(MorphType));
	total += sizeof(MorphType);

	stream.write(static_cast<const char*>(static_cast<const void*>(&this->offset_count)), sizeof(int));
	total += sizeof(int);

	switch (this->morph_type)
	{
	case MorphType::Group:
		for (int i = 0; i < offset_count; i++)
		{
			total += group_offsets.at(i).dump(stream);
		}
		break;
	case MorphType::Vertex:
		for (int i = 0; i < offset_count; i++)
		{
			total += vertex_offsets.at(i).dump(stream);
		}
		break;
	case MorphType::Bone:
		for (int i = 0; i < offset_count; i++)
		{
			total += bone_offsets.at(i).dump(stream);
		}
		break;
	case MorphType::Matrial:
		for (int i = 0; i < offset_count; i++)
		{
			total += material_offsets.at(i).dump(stream);
		}
		break;
	case MorphType::UV:
	case MorphType::AdditionalUV1:
	case MorphType::AdditionalUV2:
	case MorphType::AdditionalUV3:
	case MorphType::AdditionalUV4:
		for (int i = 0; i < offset_count; i++)
		{
			total += uv_offsets.at(i).dump(stream);
		}
		break;
	default:
		throw;
	}
	return total;
}

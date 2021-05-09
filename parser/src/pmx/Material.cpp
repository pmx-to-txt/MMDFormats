#include <stdexcept>
#include <utility>

#include "pmx2txt/parser/pmx/Material.h"

#include "pmx2txt/parser/pmx/util.h"

pmx::Material::Material(const Setting& setting_) noexcept
	: setting(setting_)
	, material_name()
	, material_english_name()
	, diffuse({ 0.0f })
	, specular({ 0.0f })
	, specularlity(0.0f)
	, ambient({ 0.0f })
	, flag(0)
	, edge_color({ 0.0f })
	, edge_size(0.0f)
	, diffuse_texture_index(0)
	, sphere_texture_index(0)
	, sphere_op_mode(0)
	, common_toon_flag(0)
	, toon_texture_index(0)
	, index_count(0)
{
}

void pmx::Material::parse(std::istream& stream)
{
	this->material_name = std::move(pmx::util::parseString(stream, this->setting.encoding));
	this->material_english_name = std::move(pmx::util::parseString(stream, this->setting.encoding));
	stream.read((char*)this->diffuse.data(), sizeof(float) * this->diffuse.size());
	stream.read((char*)this->specular.data(), sizeof(float) * this->specular.size());
	stream.read((char*)&this->specularlity, sizeof(float));
	stream.read((char*)this->ambient.data(), sizeof(float) * this->ambient.size());
	stream.read((char*)&this->flag, sizeof(uint8_t));
	stream.read((char*)this->edge_color.data(), sizeof(float) * this->edge_color.size());
	stream.read((char*)&this->edge_size, sizeof(float));
	this->diffuse_texture_index = pmx::util::parseIndex(stream, this->setting.texture_index_size);
	this->sphere_texture_index = pmx::util::parseIndex(stream, this->setting.texture_index_size);
	stream.read((char*)&this->sphere_op_mode, sizeof(uint8_t));
	stream.read((char*)&this->common_toon_flag, sizeof(uint8_t));
	if (this->common_toon_flag)
	{
		stream.read((char*)&this->toon_texture_index, sizeof(uint8_t));
	}
	else {
		this->toon_texture_index = pmx::util::parseIndex(stream, this->setting.texture_index_size);
	}
	this->memo = std::move(pmx::util::parseString(stream, this->setting.encoding));
	stream.read((char*)&this->index_count, sizeof(int));
}

std::size_t pmx::Material::dump(std::ostream& stream)
{
	std::size_t total{ 0 };
	total += pmx::util::dumpString(stream, this->material_name, this->setting.encoding);
	total += pmx::util::dumpString(stream, this->material_english_name, this->setting.encoding);
	stream.write(static_cast<char*>(static_cast<void*>(this->diffuse.data())), sizeof(float) * this->diffuse.size());
	stream.write(static_cast<char*>(static_cast<void*>(this->specular.data())), sizeof(float) * this->specular.size());
	stream.write(static_cast<char*>(static_cast<void*>(&this->specularlity)), sizeof(float));
	stream.write(static_cast<char*>(static_cast<void*>(this->ambient.data())), sizeof(float) * this->ambient.size());
	stream.write(static_cast<char*>(static_cast<void*>(&this->flag)), sizeof(uint8_t));
	stream.write(static_cast<char*>(static_cast<void*>(this->edge_color.data())), sizeof(float) * this->edge_color.size());
	stream.write(static_cast<char*>(static_cast<void*>(&this->edge_size)), sizeof(float));
	total += sizeof(float) * 16 + sizeof(uint8_t);
	total += pmx::util::dumpIndex(stream, this->diffuse_texture_index, this->setting.texture_index_size);
	total += pmx::util::dumpIndex(stream, this->sphere_texture_index, this->setting.texture_index_size);
	stream.write(static_cast<char*>(static_cast<void*>(&this->sphere_op_mode)), sizeof(uint8_t));
	stream.write(static_cast<char*>(static_cast<void*>(&this->common_toon_flag)), sizeof(uint8_t));
	total += sizeof(uint8_t) * 2;
	if (this->common_toon_flag)
	{
		stream.write(static_cast<char*>(static_cast<void*>(&this->toon_texture_index)), sizeof(uint8_t));
		total += sizeof(uint8_t);
	}
	else {
		total += pmx::util::dumpIndex(stream, this->toon_texture_index, this->setting.texture_index_size);
	}
	total += pmx::util::dumpString(stream, this->memo, this->setting.encoding);
	stream.write(static_cast<char*>(static_cast<void*>(&this->index_count)), sizeof(int));
	total += sizeof(int);
	return total;
}

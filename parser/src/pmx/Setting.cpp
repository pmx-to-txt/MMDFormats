#include <stdexcept>
#include <utility>

#include "pmx2txt/parser/pmx/Setting.h"
#include "pmx2txt/parser/pmx/util.h"

pmx::Setting::Setting() noexcept
	: encoding(0)
	, uv(0)
	, vertex_index_size(0)
	, texture_index_size(0)
	, material_index_size(0)
	, bone_index_size(0)
	, morph_index_size(0)
	, rigidbody_index_size(0)
{}

void pmx::Setting::parse(std::istream& stream)
{
	uint8_t count{};
	stream.read((char*)&count, sizeof(uint8_t));
	if (count < 8)
	{
		throw std::runtime_error("PMX Header is too short!");
	}
	stream.read((char*)&encoding, sizeof(uint8_t));
	stream.read((char*)&uv, sizeof(uint8_t));
	stream.read((char*)&vertex_index_size, sizeof(uint8_t));
	stream.read((char*)&texture_index_size, sizeof(uint8_t));
	stream.read((char*)&material_index_size, sizeof(uint8_t));
	stream.read((char*)&bone_index_size, sizeof(uint8_t));
	stream.read((char*)&morph_index_size, sizeof(uint8_t));
	stream.read((char*)&rigidbody_index_size, sizeof(uint8_t));
	uint8_t temp{};
	for (int i = 8; i < count; i++)
	{
		stream.read((char*)&temp, sizeof(uint8_t));
	}
}

std::size_t pmx::Setting::dump(std::ostream& stream) const
{
	stream.write(u8"\x08", sizeof(uint8_t));
	stream.write(static_cast<const char*>(static_cast<const void*>(&encoding)), sizeof(uint8_t));
	stream.write(static_cast<const char*>(static_cast<const void*>(&uv)), sizeof(uint8_t));
	stream.write(static_cast<const char*>(static_cast<const void*>(&vertex_index_size)), sizeof(uint8_t));
	stream.write(static_cast<const char*>(static_cast<const void*>(&texture_index_size)), sizeof(uint8_t));
	stream.write(static_cast<const char*>(static_cast<const void*>(&material_index_size)), sizeof(uint8_t));
	stream.write(static_cast<const char*>(static_cast<const void*>(&bone_index_size)), sizeof(uint8_t));
	stream.write(static_cast<const char*>(static_cast<const void*>(&morph_index_size)), sizeof(uint8_t));
	stream.write(static_cast<const char*>(static_cast<const void*>(&rigidbody_index_size)), sizeof(uint8_t));
	return 9u;
}

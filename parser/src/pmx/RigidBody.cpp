#include <stdexcept>
#include <utility>

#include "pmx2txt/parser/pmx/RigidBody.h"

#include "pmx2txt/parser/pmx/util.h"

pmx::RigidBody::RigidBody(const pmx::Setting& setting_) noexcept
	: setting(setting_)
	, rigid_body_name()
	, rigid_body_english_name()
	, target_bone(0)
	, group(0)
	, mask(0)
	, shape(0)
	, size({ 0.0f })
	, position({ 0.0f })
	, orientation({ 0.0f })
	, mass(0.0f)
	, move_attenuation(0.0f)
	, rotation_attenuation(0.0f)
	, repulsion(0.0f)
	, friction(0.0f)
	, physics_calc_type(0)
{
}

void pmx::RigidBody::parse(std::istream& stream)
{
	this->rigid_body_name = pmx::util::parseString(stream, this->setting.encoding);
	this->rigid_body_english_name = pmx::util::parseString(stream, this->setting.encoding);
	this->target_bone = pmx::util::parseIndex(stream, this->setting.bone_index_size);
	stream.read((char*)&this->group, sizeof(uint8_t));
	stream.read((char*)&this->mask, sizeof(uint16_t));
	stream.read((char*)&this->shape, sizeof(uint8_t));
	stream.read((char*)this->size.data(), sizeof(float) * this->size.size());
	stream.read((char*)this->position.data(), sizeof(float) * this->position.size());
	stream.read((char*)this->orientation.data(), sizeof(float) * this->orientation.size());
	stream.read((char*)&this->mass, sizeof(float));
	stream.read((char*)&this->move_attenuation, sizeof(float));
	stream.read((char*)&this->rotation_attenuation, sizeof(float));
	stream.read((char*)&this->repulsion, sizeof(float));
	stream.read((char*)&this->friction, sizeof(float));
	stream.read((char*)&this->physics_calc_type, sizeof(uint8_t));
}

std::size_t pmx::RigidBody::dump(std::ostream& stream) const
{
	std::size_t total{ 0 };
	total += pmx::util::dumpString(stream, this->rigid_body_name, this->setting.encoding);
	total += pmx::util::dumpString(stream, this->rigid_body_english_name, this->setting.encoding);
	total += pmx::util::dumpIndex(stream, this->target_bone, this->setting.bone_index_size);

	stream.write(static_cast<const char*>(static_cast<const void*>(&this->group)), sizeof(uint8_t));
	stream.write(static_cast<const char*>(static_cast<const void*>(&this->mask)), sizeof(uint16_t));
	stream.write(static_cast<const char*>(static_cast<const void*>(&this->shape)), sizeof(uint8_t));
	stream.write(static_cast<const char*>(static_cast<const void*>(this->size.data())), sizeof(float) * this->size.size());
	stream.write(static_cast<const char*>(static_cast<const void*>(this->position.data())), sizeof(float) * this->position.size());
	stream.write(static_cast<const char*>(static_cast<const void*>(this->orientation.data())), sizeof(float) * this->orientation.size());
	stream.write(static_cast<const char*>(static_cast<const void*>(&this->mass)), sizeof(float));
	stream.write(static_cast<const char*>(static_cast<const void*>(&this->move_attenuation)), sizeof(float));
	stream.write(static_cast<const char*>(static_cast<const void*>(&this->rotation_attenuation)), sizeof(float));
	stream.write(static_cast<const char*>(static_cast<const void*>(&this->repulsion)), sizeof(float));
	stream.write(static_cast<const char*>(static_cast<const void*>(&this->friction)), sizeof(float));
	stream.write(static_cast<const char*>(static_cast<const void*>(&this->physics_calc_type)), sizeof(uint8_t));

	total += sizeof(float) * 14 + sizeof(uint16_t) + sizeof(uint8_t) * 3;
	return total;
}

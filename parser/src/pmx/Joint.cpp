#include <stdexcept>
#include <utility>

#include "pmx2txt/parser/pmx/Joint.h"

#include "pmx2txt/parser/pmx/util.h"

pmx::JointParam::JointParam(const pmx::Setting& setting_) noexcept
	: setting(setting_)
	, rigid_body1(0)
	, rigid_body2(0)
	, position({ 0.0f })
	, orientaiton({ 0.0f })
	, move_limitation_min({ 0.0f })
	, move_limitation_max({ 0.0f })
	, rotation_limitation_min({ 0.0f })
	, rotation_limitation_max({ 0.0f })
	, spring_move_coefficient({ 0.0f })
	, spring_rotation_coefficient({ 0.0f })
{
}

void pmx::JointParam::parse(std::istream& stream)
{
	this->rigid_body1 = pmx::util::parseIndex(stream, this->setting.rigidbody_index_size);
	this->rigid_body2 = pmx::util::parseIndex(stream, this->setting.rigidbody_index_size);
	stream.read((char*)this->position.data(), sizeof(float) * this->position.size());
	stream.read((char*)this->orientaiton.data(), sizeof(float) * this->orientaiton.size());
	stream.read((char*)this->move_limitation_min.data(), sizeof(float) * this->move_limitation_min.size());
	stream.read((char*)this->move_limitation_max.data(), sizeof(float) * this->move_limitation_max.size());
	stream.read((char*)this->rotation_limitation_min.data(), sizeof(float) * this->rotation_limitation_min.size());
	stream.read((char*)this->rotation_limitation_max.data(), sizeof(float) * this->rotation_limitation_max.size());
	stream.read((char*)this->spring_move_coefficient.data(), sizeof(float) * this->spring_move_coefficient.size());
	stream.read((char*)this->spring_rotation_coefficient.data(), sizeof(float) * this->spring_rotation_coefficient.size());
}

std::size_t pmx::JointParam::dump(std::ostream& stream)
{
	std::size_t total{ 0 };
	total += pmx::util::dumpIndex(stream, this->rigid_body1, this->setting.rigidbody_index_size);
	total += pmx::util::dumpIndex(stream, this->rigid_body2, this->setting.rigidbody_index_size);

	stream.write(static_cast<char*>(static_cast<void*>(this->position.data())), sizeof(float) * this->position.size());
	stream.write(static_cast<char*>(static_cast<void*>(this->orientaiton.data())), sizeof(float) * this->orientaiton.size());
	stream.write(static_cast<char*>(static_cast<void*>(this->move_limitation_min.data())), sizeof(float) * this->move_limitation_min.size());
	stream.write(static_cast<char*>(static_cast<void*>(this->move_limitation_max.data())), sizeof(float) * this->move_limitation_max.size());
	stream.write(static_cast<char*>(static_cast<void*>(this->rotation_limitation_min.data())), sizeof(float) * this->rotation_limitation_min.size());
	stream.write(static_cast<char*>(static_cast<void*>(this->rotation_limitation_max.data())), sizeof(float) * this->rotation_limitation_max.size());
	stream.write(static_cast<char*>(static_cast<void*>(this->spring_move_coefficient.data())), sizeof(float) * this->spring_move_coefficient.size());
	stream.write(static_cast<char*>(static_cast<void*>(this->spring_rotation_coefficient.data())), sizeof(float) * this->spring_rotation_coefficient.size());

	total += sizeof(float) * 24;
	return total;
}

pmx::Joint::Joint(const pmx::Setting& setting_) noexcept
	: setting(setting_)
	, param(setting_)
{
}

void pmx::Joint::parse(std::istream& stream)
{
	this->joint_name = pmx::util::parseString(stream, this->setting.encoding);
	this->joint_english_name = pmx::util::parseString(stream, this->setting.encoding);
	stream.read((char*)&this->joint_type, sizeof(uint8_t));
	this->param.parse(stream);
}

std::size_t pmx::Joint::dump(std::ostream& stream)
{
	std::size_t total{ 0 };
	total += pmx::util::dumpString(stream, joint_name, this->setting.encoding);
	total += pmx::util::dumpString(stream, joint_english_name, this->setting.encoding);

	stream.write(static_cast<char*>(static_cast<void*>(&this->joint_type)), sizeof(uint8_t));
	total += sizeof(uint8_t);

	total += this->param.dump(stream);
	return total;
}

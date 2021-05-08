#include <stdexcept>
#include <utility>

#include "pmx2txt/parser/pmx/Joint.h"

#include "pmx2txt/parser/pmx/util.h"

pmx::JointParam::JointParam(const pmx::Setting& setting_) noexcept
	: setting(setting_)
	, rigid_body1(0)
	, rigid_body2(0)
{
	for (int i = 0; i < 3; ++i) {
		position[i] = 0.0f;
		orientaiton[i] = 0.0f;
		move_limitation_min[i] = 0.0f;
		move_limitation_max[i] = 0.0f;
		rotation_limitation_min[i] = 0.0f;
		rotation_limitation_max[i] = 0.0f;
		spring_move_coefficient[i] = 0.0f;
		spring_rotation_coefficient[i] = 0.0f;
	}
}

void pmx::JointParam::parse(std::istream& stream)
{
	this->rigid_body1 = pmx::util::parseIndex(stream, this->setting.rigidbody_index_size);
	this->rigid_body2 = pmx::util::parseIndex(stream, this->setting.rigidbody_index_size);
	stream.read((char*)this->position, sizeof(float) * 3);
	stream.read((char*)this->orientaiton, sizeof(float) * 3);
	stream.read((char*)this->move_limitation_min, sizeof(float) * 3);
	stream.read((char*)this->move_limitation_max, sizeof(float) * 3);
	stream.read((char*)this->rotation_limitation_min, sizeof(float) * 3);
	stream.read((char*)this->rotation_limitation_max, sizeof(float) * 3);
	stream.read((char*)this->spring_move_coefficient, sizeof(float) * 3);
	stream.read((char*)this->spring_rotation_coefficient, sizeof(float) * 3);
}

std::size_t pmx::JointParam::dump(std::ostream& stream)
{
	std::size_t total{ 0 };
	total += pmx::util::dumpIndex(stream, this->rigid_body1, this->setting.rigidbody_index_size);
	total += pmx::util::dumpIndex(stream, this->rigid_body2, this->setting.rigidbody_index_size);

	stream.write(static_cast<char*>(static_cast<void*>(this->position)), sizeof(float) * 3);
	stream.write(static_cast<char*>(static_cast<void*>(this->orientaiton)), sizeof(float) * 3);
	stream.write(static_cast<char*>(static_cast<void*>(this->move_limitation_min)), sizeof(float) * 3);
	stream.write(static_cast<char*>(static_cast<void*>(this->move_limitation_max)), sizeof(float) * 3);
	stream.write(static_cast<char*>(static_cast<void*>(this->rotation_limitation_min)), sizeof(float) * 3);
	stream.write(static_cast<char*>(static_cast<void*>(this->rotation_limitation_max)), sizeof(float) * 3);
	stream.write(static_cast<char*>(static_cast<void*>(this->spring_move_coefficient)), sizeof(float) * 3);
	stream.write(static_cast<char*>(static_cast<void*>(this->spring_rotation_coefficient)), sizeof(float) * 3);

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

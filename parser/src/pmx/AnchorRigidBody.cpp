#include <stdexcept>
#include <utility>

#include "pmx2txt/parser/pmx/AnchorRigidBody.h"

#include "pmx2txt/parser/pmx/util.h"

pmx::AnchorRigidBody::AnchorRigidBody(const Setting& setting_) noexcept
	: setting(setting_)
	, related_rigid_body(0)
	, related_vertex(0)
	, is_near(false)
{}

void pmx::AnchorRigidBody::parse(std::istream& stream)
{
	this->related_rigid_body = pmx::util::parseIndex(stream, this->setting.rigidbody_index_size);
	this->related_vertex = pmx::util::parseIndex(stream, this->setting.vertex_index_size);
	stream.read((char*)&this->is_near, sizeof(uint8_t));
}

std::size_t pmx::AnchorRigidBody::dump(std::ostream& stream)
{
	std::size_t total{ 0 };
	total += pmx::util::dumpIndex(stream, this->related_rigid_body, this->setting.rigidbody_index_size);
	total += pmx::util::dumpIndex(stream, this->related_vertex, this->setting.vertex_index_size);

	stream.write(static_cast<char*>(static_cast<void*>(&this->is_near)), sizeof(uint8_t));
	total += sizeof(uint8_t);
	return total;
}

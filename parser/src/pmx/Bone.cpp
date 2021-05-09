#include <stdexcept>
#include <utility>

#include "pmx2txt/parser/pmx/Bone.h"

#include "pmx2txt/parser/pmx/util.h"

pmx::Bone::Bone(const pmx::Setting& setting_) noexcept
	: setting(setting_)
	, bone_name()
	, bone_english_name()
	, position({ 0.0f })
	, parent_index(0)
	, level(0)
	, bone_flag(0)
	, offset({ 0.0f })
	, target_index(0)
	, grant_parent_index(0)
	, grant_weight(0.0f)
	, lock_axis_orientation({ 0.0f })
	, local_axis_x_orientation({ 0.0f })
	, local_axis_y_orientation({ 0.0f })
	, key(0)
	, ik_target_bone_index(0)
	, ik_loop(0)
	, ik_loop_angle_limit(0.0f)
	, ik_link_count(0)
{
}

void pmx::Bone::parse(std::istream& stream)
{
	this->bone_name = std::move(pmx::util::parseString(stream, this->setting.encoding));
	this->bone_english_name = std::move(pmx::util::parseString(stream, this->setting.encoding));
	stream.read((char*)this->position.data(), sizeof(float) * this->position.size());
	this->parent_index = pmx::util::parseIndex(stream, this->setting.bone_index_size);
	stream.read((char*)&this->level, sizeof(int));
	stream.read((char*)&this->bone_flag, sizeof(uint16_t));
	if (this->bone_flag & 0x0001) {
		this->target_index = pmx::util::parseIndex(stream, this->setting.bone_index_size);
	}
	else {
		stream.read((char*)this->offset.data(), sizeof(float) * this->offset.size());
	}
	if (this->bone_flag & (0x0100 | 0x0200)) {
		this->grant_parent_index = pmx::util::parseIndex(stream, this->setting.bone_index_size);
		stream.read((char*)&this->grant_weight, sizeof(float));
	}
	if (this->bone_flag & 0x0400) {
		stream.read((char*)this->lock_axis_orientation.data(), sizeof(float) * this->lock_axis_orientation.size());
	}
	if (this->bone_flag & 0x0800) {
		stream.read((char*)this->local_axis_x_orientation.data(), sizeof(float) * this->local_axis_x_orientation.size());
		stream.read((char*)this->local_axis_y_orientation.data(), sizeof(float) * this->local_axis_y_orientation.size());
	}
	if (this->bone_flag & 0x2000) {
		stream.read((char*)&this->key, sizeof(int));
	}
	if (this->bone_flag & 0x0020) {
		this->ik_target_bone_index = pmx::util::parseIndex(stream, this->setting.bone_index_size);
		stream.read((char*)&ik_loop, sizeof(int));
		stream.read((char*)&ik_loop_angle_limit, sizeof(float));
		stream.read((char*)&ik_link_count, sizeof(int));
		this->ik_links.reserve(ik_link_count);
		for (auto i = 0; i < ik_link_count; ++i) {
			this->ik_links.emplace_back(this->setting).parse(stream);
		}
	}
}

std::size_t pmx::Bone::dump(std::ostream& stream)
{
	std::size_t total{ 0 };
	total += pmx::util::dumpString(stream, this->bone_name, this->setting.encoding);
	total += pmx::util::dumpString(stream, this->bone_english_name, this->setting.encoding);

	stream.write(static_cast<char*>(static_cast<void*>(this->position.data())), sizeof(float) * this->position.size());
	total += sizeof(float) * 3;

	total += pmx::util::dumpIndex(stream, this->parent_index, this->setting.bone_index_size);

	stream.write(static_cast<char*>(static_cast<void*>(&this->level)), sizeof(int));
	total += sizeof(int);

	stream.write(static_cast<char*>(static_cast<void*>(&this->bone_flag)), sizeof(uint16_t));
	total += sizeof(uint16_t);

	if (this->bone_flag & 0x0001) {
		total += pmx::util::dumpIndex(stream, this->target_index, this->setting.bone_index_size);
	}
	else {
		stream.write(static_cast<char*>(static_cast<void*>(this->offset.data())), sizeof(float) * this->offset.size());
		total += sizeof(float) * 3;
	}
	if (this->bone_flag & (0x0100 | 0x0200)) {
		total += pmx::util::dumpIndex(stream, this->grant_parent_index, this->setting.bone_index_size);
		stream.write(static_cast<char*>(static_cast<void*>(&this->grant_weight)), sizeof(float));
		total += sizeof(float);
	}
	if (this->bone_flag & 0x0400) {
		stream.write(static_cast<char*>(static_cast<void*>(this->lock_axis_orientation.data())), sizeof(float) * this->lock_axis_orientation.size());
		total += sizeof(float) * 3;
	}
	if (this->bone_flag & 0x0800) {
		stream.write(static_cast<char*>(static_cast<void*>(this->local_axis_x_orientation.data())), sizeof(float) * this->local_axis_x_orientation.size());
		stream.write(static_cast<char*>(static_cast<void*>(this->local_axis_y_orientation.data())), sizeof(float) * this->local_axis_y_orientation.size());
		total += sizeof(float) * 6;
	}
	if (this->bone_flag & 0x2000) {
		stream.write(static_cast<char*>(static_cast<void*>(&this->key)), sizeof(int));
		total += sizeof(int);
	}
	if (this->bone_flag & 0x0020) {
		total += pmx::util::dumpIndex(stream, this->ik_target_bone_index, this->setting.bone_index_size);

		stream.write(static_cast<char*>(static_cast<void*>(&this->ik_loop)), sizeof(int));
		total += sizeof(int);

		stream.write(static_cast<char*>(static_cast<void*>(&this->ik_loop_angle_limit)), sizeof(float));
		total += sizeof(float);

		stream.write(static_cast<char*>(static_cast<void*>(&this->ik_link_count)), sizeof(int));
		total += sizeof(int);

		for (int i = 0; i < this->ik_link_count; i++) {
			total += ik_links.at(i).dump(stream);
		}
	}

	return total;
}

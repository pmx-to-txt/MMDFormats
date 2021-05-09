#include <stdexcept>
#include <utility>

#include "pmx2txt/parser/pmx/VertexSkinning.h"

#include "pmx2txt/parser/pmx/util.h"

pmx::VertexSkinning::VertexSkinning(const pmx::Setting& setting_) noexcept
	: setting(setting_)
{
}

pmx::VertexSkinningBDEF1::VertexSkinningBDEF1(const pmx::Setting& setting_) noexcept
	:pmx::VertexSkinning(setting_)
	, bone_index(0)
{}

void pmx::VertexSkinningBDEF1::parse(std::istream& stream)
{
	this->bone_index = pmx::util::parseIndex(stream, this->setting.bone_index_size);
}

std::size_t pmx::VertexSkinningBDEF1::dump(std::ostream& stream)
{
	return pmx::util::dumpIndex(stream, this->bone_index, this->setting.bone_index_size);
}

pmx::VertexSkinningBDEF2::VertexSkinningBDEF2(const pmx::Setting& setting_) noexcept
	: pmx::VertexSkinning(setting_)
	, bone_index1(0)
	, bone_index2(0)
	, bone_weight(0.0f)
{}

void pmx::VertexSkinningBDEF2::parse(std::istream& stream)
{
	this->bone_index1 = pmx::util::parseIndex(stream, this->setting.bone_index_size);
	this->bone_index2 = pmx::util::parseIndex(stream, this->setting.bone_index_size);
	stream.read((char*)&this->bone_weight, sizeof(float));
}

std::size_t pmx::VertexSkinningBDEF2::dump(std::ostream& stream)
{
	std::size_t total{ 0 };
	total += pmx::util::dumpIndex(stream, this->bone_index1, this->setting.bone_index_size);
	total += pmx::util::dumpIndex(stream, this->bone_index2, this->setting.bone_index_size);
	stream.write(static_cast<char*>(static_cast<void*>(&this->bone_weight)), sizeof(float));
	total += sizeof(float);
	return total;
}

pmx::VertexSkinningBDEF4::VertexSkinningBDEF4(const pmx::Setting& setting_) noexcept
	: pmx::VertexSkinning(setting_)
	, bone_index1(0)
	, bone_index2(0)
	, bone_index3(0)
	, bone_index4(0)
	, bone_weight1(0.0f)
	, bone_weight2(0.0f)
	, bone_weight3(0.0f)
	, bone_weight4(0.0f)
{}

void pmx::VertexSkinningBDEF4::parse(std::istream& stream)
{
	this->bone_index1 = pmx::util::parseIndex(stream, this->setting.bone_index_size);
	this->bone_index2 = pmx::util::parseIndex(stream, this->setting.bone_index_size);
	this->bone_index3 = pmx::util::parseIndex(stream, this->setting.bone_index_size);
	this->bone_index4 = pmx::util::parseIndex(stream, this->setting.bone_index_size);
	stream.read((char*)&this->bone_weight1, sizeof(float));
	stream.read((char*)&this->bone_weight2, sizeof(float));
	stream.read((char*)&this->bone_weight3, sizeof(float));
	stream.read((char*)&this->bone_weight4, sizeof(float));
}

std::size_t pmx::VertexSkinningBDEF4::dump(std::ostream& stream)
{
	std::size_t total{ 0 };
	total += pmx::util::dumpIndex(stream, this->bone_index1, this->setting.bone_index_size);
	total += pmx::util::dumpIndex(stream, this->bone_index2, this->setting.bone_index_size);
	total += pmx::util::dumpIndex(stream, this->bone_index3, this->setting.bone_index_size);
	total += pmx::util::dumpIndex(stream, this->bone_index4, this->setting.bone_index_size);
	stream.write(static_cast<char*>(static_cast<void*>(&this->bone_weight1)), sizeof(float));
	stream.write(static_cast<char*>(static_cast<void*>(&this->bone_weight2)), sizeof(float));
	stream.write(static_cast<char*>(static_cast<void*>(&this->bone_weight3)), sizeof(float));
	stream.write(static_cast<char*>(static_cast<void*>(&this->bone_weight4)), sizeof(float));
	total += sizeof(float) * 4;
	return total;
}

pmx::VertexSkinningSDEF::VertexSkinningSDEF(const pmx::Setting& setting_) noexcept
	: pmx::VertexSkinning(setting_)
	, bone_index1(0)
	, bone_index2(0)
	, bone_weight(0.0f)
	, sdef_c({ 0.0f,0.0f, 0.0f })
	, sdef_r0({ 0.0f,0.0f, 0.0f })
	, sdef_r1({ 0.0f,0.0f, 0.0f })
{
}

void pmx::VertexSkinningSDEF::parse(std::istream& stream)
{
	this->bone_index1 = pmx::util::parseIndex(stream, this->setting.bone_index_size);
	this->bone_index2 = pmx::util::parseIndex(stream, this->setting.bone_index_size);
	stream.read(static_cast<char*>(static_cast<void*>(&this->bone_weight)), sizeof(float));
	stream.read(static_cast<char*>(static_cast<void*>(this->sdef_c.data())), sizeof(float) * this->sdef_c.size());
	stream.read(static_cast<char*>(static_cast<void*>(this->sdef_r0.data())), sizeof(float) * this->sdef_r0.size());
	stream.read(static_cast<char*>(static_cast<void*>(this->sdef_r1.data())), sizeof(float) * this->sdef_r1.size());
}

std::size_t pmx::VertexSkinningSDEF::dump(std::ostream& stream)
{
	std::size_t total{ 0 };
	total += pmx::util::dumpIndex(stream, this->bone_index1, this->setting.bone_index_size);
	total += pmx::util::dumpIndex(stream, this->bone_index2, this->setting.bone_index_size);
	stream.write(static_cast<char*>(static_cast<void*>(&this->bone_weight)), sizeof(float));
	stream.write(static_cast<char*>(static_cast<void*>(this->sdef_c.data())), sizeof(float) * this->sdef_c.size());
	stream.write(static_cast<char*>(static_cast<void*>(this->sdef_r0.data())), sizeof(float) * this->sdef_r0.size());
	stream.write(static_cast<char*>(static_cast<void*>(this->sdef_r1.data())), sizeof(float) * this->sdef_r1.size());
	total += sizeof(float) * 13;
	return total;
}

pmx::VertexSkinningQDEF::VertexSkinningQDEF(const pmx::Setting& setting_) noexcept
	: pmx::VertexSkinning(setting_)
	, bone_index1(0)
	, bone_index2(0)
	, bone_index3(0)
	, bone_index4(0)
	, bone_weight1(0.0f)
	, bone_weight2(0.0f)
	, bone_weight3(0.0f)
	, bone_weight4(0.0f)
{}

void pmx::VertexSkinningQDEF::parse(std::istream& stream)
{
	this->bone_index1 = pmx::util::parseIndex(stream, this->setting.bone_index_size);
	this->bone_index2 = pmx::util::parseIndex(stream, this->setting.bone_index_size);
	this->bone_index3 = pmx::util::parseIndex(stream, this->setting.bone_index_size);
	this->bone_index4 = pmx::util::parseIndex(stream, this->setting.bone_index_size);
	stream.read(static_cast<char*>(static_cast<void*>(&this->bone_weight1)), sizeof(float));
	stream.read(static_cast<char*>(static_cast<void*>(&this->bone_weight2)), sizeof(float));
	stream.read(static_cast<char*>(static_cast<void*>(&this->bone_weight3)), sizeof(float));
	stream.read(static_cast<char*>(static_cast<void*>(&this->bone_weight4)), sizeof(float));
}

std::size_t pmx::VertexSkinningQDEF::dump(std::ostream& stream)
{
	std::size_t total{ 0 };
	total += pmx::util::dumpIndex(stream, this->bone_index1, this->setting.bone_index_size);
	total += pmx::util::dumpIndex(stream, this->bone_index2, this->setting.bone_index_size);
	total += pmx::util::dumpIndex(stream, this->bone_index3, this->setting.bone_index_size);
	total += pmx::util::dumpIndex(stream, this->bone_index4, this->setting.bone_index_size);
	stream.write(static_cast<char*>(static_cast<void*>(&this->bone_weight1)), sizeof(float));
	stream.write(static_cast<char*>(static_cast<void*>(&this->bone_weight2)), sizeof(float));
	stream.write(static_cast<char*>(static_cast<void*>(&this->bone_weight3)), sizeof(float));
	stream.write(static_cast<char*>(static_cast<void*>(&this->bone_weight4)), sizeof(float));
	total += sizeof(float) * 4;
	return total;
}

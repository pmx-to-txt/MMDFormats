#include <stdexcept>
#include <utility>

#include "pmx2txt/parser/pmx/Vertex.h"

#include "pmx2txt/parser/pmx/Setting.h"
#include "pmx2txt/parser/pmx/util.h"

pmx::Vertex::Vertex(const pmx::Setting& setting_) noexcept
	: setting(setting_)
	, edge(0.0f)
	, positon({ 0.0f,0.0f, 0.0f, })
	, normal({ 0.0f,0.0f, 0.0f, })
	, uv({ 0.0f,0.0f, })
	, uva({ std::array<float, 4>{0.0f} })
	, skinning_type(pmx::VertexSkinningType::BDEF1)
	, skinning(nullptr)
{
}

void pmx::Vertex::parse(std::istream& stream)
{
	stream.read(static_cast<char*>(static_cast<void*>(this->positon.data())), sizeof(float) * this->positon.size());
	stream.read(static_cast<char*>(static_cast<void*>(this->normal.data())), sizeof(float) * this->normal.size());
	stream.read(static_cast<char*>(static_cast<void*>(this->uv.data())), sizeof(float) * this->uv.size());
	for (int i = 0; i < this->setting.uv; ++i)
	{
		stream.read(static_cast<char*>(static_cast<void*>(this->uva.at(i).data())), sizeof(float) * this->uva.at(i).size());
	}
	stream.read((char*)&this->skinning_type, sizeof(pmx::VertexSkinningType));
	switch (this->skinning_type)
	{
	case pmx::VertexSkinningType::BDEF1:
		this->skinning = std::make_unique<pmx::VertexSkinningBDEF1>(this->setting);
		this->skinning->parse(stream);
		break;
	case pmx::VertexSkinningType::BDEF2:
		this->skinning = std::make_unique<pmx::VertexSkinningBDEF2>(this->setting);
		this->skinning->parse(stream);
		break;
	case pmx::VertexSkinningType::BDEF4:
		this->skinning = std::make_unique<pmx::VertexSkinningBDEF4>(this->setting);
		this->skinning->parse(stream);
		break;
	case pmx::VertexSkinningType::SDEF:
		this->skinning = std::make_unique<pmx::VertexSkinningSDEF>(this->setting);
		this->skinning->parse(stream);
		break;
	case pmx::VertexSkinningType::QDEF:
		this->skinning = std::make_unique<pmx::VertexSkinningQDEF>(this->setting);
		this->skinning->parse(stream);
		break;
	default:
		throw "invalid skinning type";
	}
	//this->skinning->parse(stream, setting);
	stream.read((char*)&this->edge, sizeof(float));
}

std::size_t pmx::Vertex::dump(std::ostream& stream) const
{
	std::size_t total{ 0 };
	stream.write(static_cast<const char*>(static_cast<const void*>(this->positon.data())), sizeof(float) * this->positon.size());
	stream.write(static_cast<const char*>(static_cast<const void*>(this->normal.data())), sizeof(float) * this->normal.size());
	stream.write(static_cast<const char*>(static_cast<const void*>(this->uv.data())), sizeof(float) * this->uv.size());
	total += sizeof(float) * 8;
	for (uint8_t i = 0; i < this->setting.uv; ++i)
	{
		stream.write(static_cast<const char*>(static_cast<const void*>(this->uva.at(i).data())), sizeof(float) * this->uva.at(i).size());
		total += sizeof(float) * 4;
	}
	stream.write(static_cast<const char*>(static_cast<const void*>(&this->skinning_type)), sizeof(pmx::VertexSkinningType));
	total += sizeof(pmx::VertexSkinningType);
	total += this->skinning->dump(stream);
	stream.write(static_cast<const char*>(static_cast<const void*>(&this->edge)), sizeof(float));
	total += sizeof(float);
	return total;
}

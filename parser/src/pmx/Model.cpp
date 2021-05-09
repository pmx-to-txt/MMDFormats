#include <array>
#include <stdexcept>
#include <utility>

#include "pmx2txt/parser/pmx/Model.h"

#include "pmx2txt/parser/pmx/util.h"

pmx::Model::Model() noexcept
	: version( 0.0f)
	, setting()
	, model_name()
	, model_english_name()
	, model_comment()
	, model_english_comment()
	, vertices()
	, indices()
	, textures()
	, materials()
	, bones()
	, morphs()
	, frames()
	, rigid_bodies()
	, joints()
	, soft_bodies()
{}

void pmx::Model::parse(std::istream& stream)
{
	// マジック
	std::array<char, 4> magic{0};
	stream.read((char*)magic.data(), sizeof(char) * magic.size());
	if (magic.at(0) != 0x50 || magic.at(1) != 0x4d || magic.at(2) != 0x58 || magic.at(3) != 0x20)
	{
		std::cerr << "invalid magic number." << std::endl;
		throw;
	}
	// バージョン
	stream.read((char*)&version, sizeof(float));
	if (version != 2.0f && version != 2.1f)
	{
		std::cerr << "this is not ver2.0 or ver2.1 but " << version << "." << std::endl;
		throw;
	}
	// ファイル設定
	this->setting.parse(stream);

	// モデル情報
	this->model_name = std::move(pmx::util::parseString(stream, setting.encoding));
	this->model_english_name = std::move(pmx::util::parseString(stream, setting.encoding));
	this->model_comment = std::move(pmx::util::parseString(stream, setting.encoding));
	this->model_english_comment = std::move(pmx::util::parseString(stream, setting.encoding));

	// 頂点
	{
		int count{ 0 };
		stream.read((char*)&count, sizeof(int));
		this->vertices.reserve(count);
		for (auto i = 0; i < count; ++i)
		{
			this->vertices.emplace_back(this->setting).parse(stream);
		}
	}

	// 面
	{
		int count{ 0 };
		stream.read((char*)&count, sizeof(int));
		this->indices.resize(count);
		for (auto& item : this->indices)
		{
			item = pmx::util::parseIndex(stream, setting.vertex_index_size);
		}
	}
	
	// テクスチャ
	{
		int count{ 0 };
		stream.read((char*)&count, sizeof(int));
		this->textures.resize(count);
		for (auto& item : this->textures)
		{
			item = pmx::util::parseString(stream, setting.encoding);
		}
	}

	// マテリアル
	{
		int count{ 0 };
		stream.read((char*)&count, sizeof(int));
		this->materials.reserve(count);
		for (auto i = 0; i < count; ++i)
		{
			this->materials.emplace_back(this->setting).parse(stream);
		}
	}

	// ボーン
	{
		int count{ 0 };
		stream.read((char*)&count, sizeof(int));
		this->bones.reserve(count);
		for (auto i = 0; i < count; ++i)
		{
			this->bones.emplace_back(this->setting).parse(stream);
		}
	}

	// モーフ
	{
		int count{ 0 };
		stream.read((char*)&count, sizeof(int));
		this->morphs.reserve(count);
		for (auto i = 0; i < count; ++i)
		{
			this->morphs.emplace_back(this->setting).parse(stream);
		}
	}

	// 表示枠
	{
		int count{ 0 };
		stream.read((char*)&count, sizeof(int));
		this->frames.reserve(count);
		for (auto i = 0; i < count; ++i)
		{
			this->frames.emplace_back(this->setting).parse(stream);
		}
	}

	// 剛体
	{
		int count{ 0 };
		stream.read((char*)&count, sizeof(int));
		this->rigid_bodies.reserve(count);
		for (auto i = 0; i < count; ++i)
		{
			this->rigid_bodies.emplace_back(this->setting).parse(stream);
		}
	}

	// ジョイント
	{
		int count{ 0 };
		stream.read((char*)&count, sizeof(int));
		this->joints.reserve(count);
		for (auto i = 0; i < count; ++i)
		{
			this->joints.emplace_back(this->setting).parse(stream);
		}
	}

	//// ソフトボディ
	//if (this->version == 2.1f)
	//{
	//	stream.read((char*) &this->soft_body_count, sizeof(int));
	//	this->soft_bodies = std::make_unique<PmxSoftBody []>(this->soft_body_count);
	//	for (int i = 0; i < this->soft_body_count; i++)
	//	{
	//		this->soft_bodies[i].parse(stream, &setting);
	//	}
	//}
}

std::size_t pmx::Model::dump(std::ostream& stream) const
{
	std::size_t total{ 0 };
	// マジック
	constexpr std::array<char, 4> magic = { 0x50, 0x4d, 0x58, 0x20 };
	stream.write(magic.data(), sizeof(char) * 4);
	total += sizeof(char) * 4;


	// バージョン
	stream.write(static_cast<const char*>(static_cast<const void*>(&this->version)), sizeof(float));
	total += sizeof(float);

	// ファイル設定
	total += this->setting.dump(stream);

	// モデル情報
	total += pmx::util::dumpString(stream, this->model_name, setting.encoding);
	total += pmx::util::dumpString(stream, this->model_english_name, setting.encoding);
	total += pmx::util::dumpString(stream, this->model_comment, setting.encoding);
	total += pmx::util::dumpString(stream, this->model_english_comment, setting.encoding);

	// 頂点
	{
		int count = this->vertices.size();
		stream.write(static_cast<const char*>(static_cast<const void*>(&count)), sizeof(int));
		total += sizeof(int);
	}

	for (auto& item : this->vertices)
	{
		total += item.dump(stream);
	}

	// 面
	{
		int count = this->indices.size();
		stream.write(static_cast<const char*>(static_cast<const void*>(&count)), sizeof(int));
		total += sizeof(int);
	}

	for (auto& item : this->indices)
	{
		total += pmx::util::dumpIndex(stream, item, setting.vertex_index_size);
	}

	// テクスチャ
	{
		int count = this->textures.size();
		stream.write(static_cast<const char*>(static_cast<const void*>(&count)), sizeof(int));
		total += sizeof(int);
	}

	for (auto& item : this->textures)
	{
		total += pmx::util::dumpString(stream, item, setting.encoding);
	}

	// マテリアル
	{
		int count = this->materials.size();
		stream.write(static_cast<const char*>(static_cast<const void*>(&count)), sizeof(int));
		total += sizeof(int);
	}

	for (auto& item : this->materials)
	{
		total += item.dump(stream);
	}

	// ボーン
	{
		int count = this->bones.size();
		stream.write(static_cast<const char*>(static_cast<const void*>(&count)), sizeof(int));
		total += sizeof(int);
	}

	for (auto& item : this->bones)
	{
		total += item.dump(stream);
	}

	// モーフ
	{
		int count = this->morphs.size();
		stream.write(static_cast<const char*>(static_cast<const void*>(&count)), sizeof(int));
		total += sizeof(int);
	}

	for (auto& item : this->morphs)
	{
		total += item.dump(stream);
	}

	// 表示枠
	{
		int count = this->frames.size();
		stream.write(static_cast<const char*>(static_cast<const void*>(&count)), sizeof(int));
		total += sizeof(int);
	}

	for (auto& item : this->frames)
	{
		total += item.dump(stream);
	}

	// 剛体
	{
		int count = this->rigid_bodies.size();
		stream.write(static_cast<const char*>(static_cast<const void*>(&count)), sizeof(int));
		total += sizeof(int);
	}

	for (auto& item : this->rigid_bodies)
	{
		total += item.dump(stream);
	}

	// ジョイント
	{
		int count = this->joints.size();
		stream.write(static_cast<const char*>(static_cast<const void*>(&count)), sizeof(int));
		total += sizeof(int);
	}

	for (auto& item : this->joints)
	{
		total += item.dump(stream);
	}
	return total;
}

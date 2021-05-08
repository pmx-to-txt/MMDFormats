#ifndef PMX2TXT_PARSER_TXT_H
#define PMX2TXT_PARSER_TXT_H

#include <iosfwd>

#include "pmx/Model.h"

namespace pmx2txt
{
	namespace txt
	{
		void Export(std::ostream& stream, const pmx::Model& model);

		void Import(std::istream& stream, pmx::Model& model);
	}
}

#endif

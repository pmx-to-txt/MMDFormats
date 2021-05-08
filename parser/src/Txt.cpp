#include "pmx2txt/parser/Txt.h"

using namespace std;

void pmx2txt::txt::Export(std::ostream& stream, const pmx::Model& model)
{
	stream << model.version << endl;
}

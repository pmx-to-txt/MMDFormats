#include "pmx2txt/parser/Txt.h"

#include <iomanip>

using namespace std;

void pmx2txt::txt::Export(std::ostream& stream, const pmx::Model& model)
{
	stream << "Version: " << fixed << setprecision(1) << model.version << defaultfloat << endl;
}

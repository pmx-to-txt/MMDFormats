#include "pmx2txt/parser/Pmx.h"
#include "pmx2txt/parser/Txt.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>

#define PRINT(x, y) std::cout << std::setw(24) << x << ":" \
                            << std::setw(24) << std::right << y << std::endl;

struct TooFewArguments : std::exception { };
struct FileOpenError : std::exception { FileOpenError(const std::string& fileName) : fileName{ fileName } {} std::string fileName; };
int main(int argc, char* argv[]) {
	try
	{
		if (argc < 2)
			throw TooFewArguments{};
		std::ifstream file{ argv[1], std::ios::in | std::ios::binary };
		if (!file)
			throw FileOpenError{ argv[1] };

		pmx::Model x;
		x.parse(file);

		system("chcp.com 65001 > nul");
		pmx2txt::txt::Export(std::cout, x);
	}
	catch (...)
	{
		try { throw; }
		catch (const TooFewArguments&)
		{
			std::cout << "Usage: " << argv[0] << " [PMX file path]" << std::endl;
		}
		catch (const FileOpenError& e)
		{
			std::cout << "Can't open " << e.fileName << ", please check" << std::endl;
		}
		catch (...)
		{
			std::cout << "Unknown Error" << std::endl;
		}
		return 1;
	}
}

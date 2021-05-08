#include "pmx2txt/parser/Pmx.h"
#include "pmx2txt/parser/Txt.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>

#define PRINT(x, y) std::cout << std::setw(24) << x << ":" \
                            << std::setw(24) << std::right << y << std::endl;

int main(int argc, char *argv[]) {
    if(argc < 2) {
        std::cout << "Usage: ./demo [PMX file path]" << std::endl;
        return 0;
    }
    std::filebuf fb;
    if(!fb.open(argv[1], std::ios::in | std::ios::binary)) {
        std::cout << "Can't open the " << argv[1] << ", please check" << std::endl;
        exit(-1);
    }

    std::istream is(&fb);
    pmx::Model x;
    x.parse(is);

    system("chcp.com 65001");
    pmx2txt::txt::Export(std::cout, x);

    /*
    std::ofstream tmpOut("tmp.bin", std::ios::out|std::ios::binary);
    x.dump(tmpOut);
    
    PRINT("PMX version", x.version)
    PRINT("Model Name", x.model_name)
    PRINT("Model English Name", x.model_english_name)
    PRINT("Comment", x.model_comment)
    PRINT("English Comment", x.model_english_comment)
    PRINT("#vertex", x.vertices.size())
    PRINT("#index", x.indices.size())
    PRINT("#texture", x.textures.size())
    PRINT("#material", x.materials.size())
    PRINT("#bone", x.bones.size())
    PRINT("#morph", x.morphs.size())
    PRINT("#frame", x.frames.size())
    PRINT("#rigid body", x.rigid_bodies.size())
    PRINT("#joint", x.joints.size())
    PRINT("#soft body", x.soft_bodies.size())
    */
}

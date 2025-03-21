#include "pch.h"
#include "FileLoader.h"
#include <fstream>
#include <sstream>

/*
# Vertices
v 0.0 0.0 0.0  # 1 a
v 0.0 1.0 0.0  # 2 b

# Normal vectors
# One for each face. Shared by all vertices in that face.
vn  1.0  0.0  0.0  # 1 cghd
vn -1.0  0.0  0.0  # 2 aefb

# Face 1: cghd = cgh + chd
f 3//1 7//1 8//1
f 3//1 8//1 4//1

# Face 2: aefb = aef + afb
f 1//2 5//2 6//2
f 1//2 6//2 2//2

vt 0.25 1.00  # 1 f(5) = f for face 5
vt 0.50 1.00  # 2 g(5)
vt 0    0.75  # 3 f(2)


# 머티리얼 파일.
mtllib cube.mtl

# 
usemtl texture

*/

FObjInfo FFileLoader::ImportObj(const FString& FileName)
{
    std::ifstream ObjFile(FileName.c_str());
    
	FObjInfo ObjInfo;

    std::string Line;
   
    if (!ObjFile.is_open())
    {
        std::cerr << "파일을 열 수 없습니다: " << FileName << std::endl;
        return ObjInfo;
    }

    std::string Line;

}

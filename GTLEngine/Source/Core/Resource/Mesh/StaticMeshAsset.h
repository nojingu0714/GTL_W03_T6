#pragma once

#include "Math/Vector.h"
#include <Resource/Types.h>

#include <fstream>
#include <sstream>

struct FNormalVertex
{
    FVector Pos;      // 버텍스 포지션.
    FVector Normal;   // 버텍스 노말.
    FVector4 Color;
    //FVector2 Tex;     // 버텍스 텍스쳐 좌표.
};


// 스태틱 메시 정보.
struct FStaticMesh
{
    // Mesh obj 파일 경로 이름.
	FString PathFileName;

	// 버텍스 정보.
	TArray<FNormalVertex> Vertices;
    // 인덱스 정보.
	TArray<uint32> Indices;
};

// Raw Data. Obj 원본 데이터.
struct FObjInfo
{
    FVector V;      // 버텍스 포지션.
    FVector Normal;   // 버텍스 노말.
    FVector4 Color;
    //FVector2 Tex; 

    TArray<FObjMaterialInfo> Materials;

};

struct FObjMaterialInfo
{
    FString MatName;
    // 텍스쳐 정보.
    // Diffuse Scalar
    // Diffuse Texture
    FVector Ka; // Ambient Reflection
    FVector Kd; // Diffuse Reflection
    FVector Ks; // Specular Reflection
    FVector Ke; // Emissive reflectivity
    float Ns; // Specular Exponent
    int Illum; // Illumination Model 
    FString MapKd; // Diffuse Texture
    FString MapKa; // Ambient Texture
    FString MapKs; // Specular Texture
    FString MapKe; // Emissive Texture
    float d; // 투명도.
    float Tr; // 반비례 투명도.
    float Ni; // Refractive index.
    FString MapBump; // Bump map.
    FString MapDisplace; // Displacement Map.
};

struct FObjImporter
{
    // Obj Parsing (*.obj to FObjInfo)
    // Material Parsing (*.obj to MaterialInfo)
    // Convert the Raw data to Cooked data (FStaticMesh)
    static void ObjParsing(const FString& InFileName)
    {
        std::ifstream ObjFile(InFileName.c_str());
     

        if (!ObjFile.is_open()) {
            throw std::runtime_error("Failed to open OBJ file.");
        }
        
        std::string line;
        FString currentMaterial;
        FObjInfo NewObjInfo;


        while (std::getline(ObjFile, line)) {
            std::stringstream ss(line);
            std::string token;

            // Skip empty lines and comment lines (lines starting with #)
            if (line.empty() || line[0] == '#') continue;

            // Handle vertices (v)
            if (line.substr(0, 2) == "v ") {
                FVector vertex;
                FVector4 color(1.0f, 1.0f, 1.0f, 1.0f); // Default color: white

                // Parse vertex coordinates
                ss >> token >> vertex.X >> vertex.Y >> vertex.Z;

                // Check if color (RGBA) is provided (i.e., 4 components)
                if (ss >> color.X >> color.Y >> color.Z >> color.W) {
                    // If we successfully read 4 components, this means we have RGBA color
                    NewObjInfo.Vertices.push_back(FNormalVertex{ vertex, FVector(0, 0, 0), color, /*FVector2(0, 0)*/ });
                }
                else {
                    // If no color was provided, use default color (white)
                    NewObjInfo.Vertices.push_back(FNormalVertex{ vertex, FVector(0, 0, 0), color, /*FVector2(0, 0)*/ });
                }
            }
            // Handle normals (vn)
            else if (line.substr(0, 3) == "vn ") {
                FVector normal;
                ss >> token >> normal.X >> normal.Y >> normal.Z;
                NewObjInfo.Normal.Add(FVertexSimple{ normal, 0 });  // 0 is a placeholder for index
            }
            // Handle texture coordinates (vt)
            else if (line.substr(0, 3) == "vt ") {
                FVector2 uv;
                ss >> token >> uv.X >> uv.Y;
                NewObjInfo.UV.Add(FVertexUV{ uv, 0 });  // 0 is a placeholder for index
            }
            // Handle face (f) which contains vertex indices
            else if (line.substr(0, 2) == "f ") {
                std::vector<int> indices;
                while (ss >> token) {
                    size_t v_idx, t_idx, n_idx;
                    if (sscanf(token.c_str(), "%zu/%zu/%zu", &v_idx, &t_idx, &n_idx) != 3) {
                        throw std::runtime_error("Face parsing error. Incorrect face format.");
                    }
                    indices.push_back(v_idx - 1);  // 0-based index adjustment
                }
                for (auto idx : indices) {
                    NewObjInfo.Indices.Add(idx);
                }
            }
            // Handle material library (mtllib)
            else if (line.substr(0, 7) == "mtllib ") {
                FString mtlFileName;
                ss >> token >> mtlFileName;
                NewObjInfo.MaterialLib = mtlFileName;
            }
            // Handle material usage (usemtl)
            else if (line.substr(0, 7) == "usemtl ") {
                ss >> token >> currentMaterial;
            }
        }
        ObjFile.close();

        // If we have a material library, parse the materials
        if (!NewObjInfo.MaterialLib.IsEmpty()) {
            ParseMaterialFile(NewObjInfo.MaterialLib, NewObjInfo);
        }

        return NewObjInfo;
    }
    
};
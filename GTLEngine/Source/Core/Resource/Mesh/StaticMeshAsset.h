#pragma once

#include "Math/Vector.h"
#include <Resource/Types.h>

#include <fstream>
#include <sstream>

#include "Utils/GTLStringLibrary.h"

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
    FVector Ka; // Ambient Reflection
    FVector Kd; // Diffuse Reflection
    FVector Ks; // Specular Reflection
    FVector Ke; // Emissive Reflectivity
    float Ns = 0.0f; // Specular Exponent
    int Illum = 0; // Illumination Model
    float d = 1.0f; // 투명도 (opacity)
    float Tr = 0.0f; // 반비례 투명도 (Transparency)
    float Ni = 1.0f; // Refractive Index
    FString MapKa; // Ambient Texture Map
    FString MapKd; // Diffuse Texture Map
    FString MapKs; // Specular Texture Map
    FString MapKe; // Emissive Texture Map
    FString MapBump; // Bump Map
    FString MapDisplace; // Displacement Map

    // 필요에 따라 추가: 예) map_d, refl, bump scale(-bm), Tf 등.
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


        while (std::getline(ObjFile, line))
        {
			line = UGTLStringLibrary::StringRemoveNoise(line);

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

    static bool ParseMtlFile(const std::string& InFileName, FObjMaterialInfo& OutInfo)
    {
        std::ifstream MtlFile(InFileName.c_str());

        if (!MtlFile.is_open()) {
            throw std::runtime_error("Failed to open OBJ file.");
        }

		std::string line;
        while (std::getline(MtlFile, line))
        {
			line = UGTLStringLibrary::StringRemoveNoise(line);
			if (line.empty() || line[0] == '#') continue;

			std::istringstream ss(line);
			std::string token;
			ss >> token;

            if (token == "newmtl ")
            {
				std::string matName;
				ss >> matName;
				OutInfo.MatName = UGTLStringLibrary::StringToWString(matName);
            }
            else if (token == "Ka ")
            {
				float r, g, b;
				ss >> r >> g >> b;
				OutInfo.Ka = FVector(r, g, b);
			}
			else if (token == "Kd ")
			{
				float r, g, b;
				ss >> r >> g >> b;
				OutInfo.Kd = FVector(r, g, b);
            }
            else if (token == "Ks ")
            {
                float r, g, b;
                ss >> r >> g >> b;
                OutInfo.Ks = FVector(r, g, b);
            }
            else if (token == "Ke ")
            {
				float r, g, b;
				ss >> r >> g >> b;
				OutInfo.Ke = FVector(r, g, b);
			}
            else if (token == "illum ")
            {
				int illum;
				ss >> illum;
				OutInfo.Illum = illum;
            }
            else if (token == "Ns ")
            {
                float ns;
				ss >> ns;
				OutInfo.Ns = ns;
			}
			else if (token == "d ")
			{
				float d;
				ss >> d;
				OutInfo.d = d;
            }
            else if (token == "Tr ")
            {
				float tr;
				ss >> tr;
				OutInfo.Tr = tr;
			}
			else if (token == "Ni ")
			{
				float ni;
				ss >> ni;
				OutInfo.Ni = ni;
			}
			else if (token == "map_Ka ")
			{
				std::string mapKa;
				ss >> mapKa;
				OutInfo.MapKa = UGTLStringLibrary::StringToWString(mapKa);
			}
			else if (token == "map_Kd ")
			{
				std::string mapKd;
				ss >> mapKd;
				OutInfo.MapKd = UGTLStringLibrary::StringToWString(mapKd);
			}
			else if (token == "map_Ks ")
			{
				std::string mapKs;
				ss >> mapKs;
				OutInfo.MapKs = UGTLStringLibrary::StringToWString(mapKs);
			}
			else if (token == "map_Ke ")
			{
				std::string mapKe;
				ss >> mapKe;
				OutInfo.MapKe = UGTLStringLibrary::StringToWString(mapKe);
			}
			else if (token == "map_Bump ")
			{
				std::string mapBump;
				ss >> mapBump;
				OutInfo.MapBump = UGTLStringLibrary::StringToWString(mapBump);
			}
			else if (token == "map_Displace ")
			{
				std::string mapDisplace;
				ss >> mapDisplace;
				OutInfo.MapDisplace = UGTLStringLibrary::StringToWString(mapDisplace);
			}
            else
			{
				// Unknown token
				OutInfo = FObjMaterialInfo();
                return false;
			}

        }
		MtlFile.close();
		return true;
    }
    
};
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

struct FFace // obj 파일에 있는 f  a/b/c 값들 ( a :정점 / b : 텍스처 / c : 노멀 )
{
    TArray<int> Vertices;      // 정점 인덱스
    TArray<int> TexCoords;     // 텍스처 좌표 인덱스
    TArray<int> Normals;       // 노멀 벡터 인덱스
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

// Raw Data. Obj 원본 데이터.
struct FObjInfo
{
    TArray<FVector> Vertices;      // 버텍스 포지션.
    TArray<FVector> Normals;   // 버텍스 노말.
    TArray<FVector4> Colors;
    TArray<FFace> Indices;
    //FVector2 UVs; 

    TArray<FObjMaterialInfo> Materials;
    TMap<FString, FFace> Faces;
};

struct FObjImporter
{
    // Obj Parsing (*.obj to FObjInfo)
    // Material Parsing (*.obj to MaterialInfo)
    // Convert the Raw data to Cooked data (FStaticMesh)
    static bool ParseObjFile(const std::string& InPath, const std::string& InFileName, FObjInfo& OutObjInfo)
    {
        std::string FileName = InPath + "/" + InFileName;
        std::ifstream ObjFile(FileName.c_str());
        FString CurrentMaterial;  // 현재 활성화된 텍스처 이름
        std::string line;

        if (!ObjFile.is_open()) {
            throw std::runtime_error("Failed to open OBJ file.");
            return false;
        }
       
        while (std::getline(ObjFile, line))
        {
			line = UGTLStringLibrary::StringRemoveNoise(line);

            std::stringstream ss(line);
            std::string token;

            // Skip empty lines and comment lines (lines starting with #)
            if (line.empty() || line[0] == '#') continue;

            // Handle vertices (v)
            if (line.substr(0, 2) == "v ") {
                FVector Vertex;
                FVector4 Color(1.0f, 1.0f, 1.0f, 1.0f); // Default color: white

                // Parse vertex coordinates
                ss >> token >> Vertex.X >> Vertex.Y >> Vertex.Z;

                // Check if color (RGBA) is provided (i.e., 4 components)
                if (ss >> Color.X >> Color.Y >> Color.Z >> Color.W) {
                    // If we successfully read 4 components, this means we have RGBA color
                    OutObjInfo.Colors.push_back(Color);
                }
                else {
                    // If no color was provided, use default color (white)
                    OutObjInfo.Vertices.push_back({Vertex});
                }
            }
            // Handle normals (vn)
            else if (line.substr(0, 3) == "vn ") {
                FVector normal;
                ss >> token >> normal.X >> normal.Y >> normal.Z;
                OutObjInfo.Normals.push_back(FVector{ normal});  // 0 is a placeholder for index
            }
            // Handle texture coordinates (vt)
            else if (line.substr(0, 3) == "vt ") {
             //   FVector2 uv;
             //   ss >> token >> uv.X >> uv.Y;
             //   OutObjInfo.UVs.push_back(uv});  // 0 is a placeholder for index
            }
            // Handle face (f) which contains vertex indices
            // Handle material library (mtllib)
            else if (line.substr(0, 7) == "mtllib ") {
                std::string MtlFileName;  // std::string으로 먼저 읽고
                ss >> token >> MtlFileName;
                MtlFileName = InPath + "/" + MtlFileName;
                FObjMaterialInfo NewMtlInfo;
                if (!ParseMtlFile(MtlFileName, NewMtlInfo))
                {
                    return false;
                }
                    
                OutObjInfo.Materials.push_back(NewMtlInfo);
            }
            // Handle material usage (usemtl)
            else if (line.substr(0, 7) == "usemtl ")
            {
                std::string MtlName;
                ss >> token >> MtlName;

                CurrentMaterial = UGTLStringLibrary::StringToWString(MtlName);

                if (!OutObjInfo.Faces.contains(CurrentMaterial)) {
                    OutObjInfo.Faces.insert(make_pair(CurrentMaterial, FFace()));
                }

            }
            else if (line.substr(0, 2) == "f ") {
                TArray<int> faceVertices;
                TArray<int> faceTexCoords;
                TArray<int> faceNormals;

                // 면 정보 파싱
                std::stringstream faceStream(line);
                std::string faceToken;
                while (faceStream >> faceToken) {
                    std::vector<std::string> parts;
                    std::stringstream tokenStream(faceToken);
                    std::string part;
                    while (std::getline(tokenStream, part, '/')) {
                        parts.push_back(part);
                    }

                    if (parts.size() == 1) {
                        // 정점만 있는 경우
                        faceVertices.push_back(std::stoi(parts[0]) - 1);
                    }
                    else if (parts.size() == 2) {
                        // 정점 + 텍스처
                        faceVertices.push_back(std::stoi(parts[0]) - 1);
                        faceTexCoords.push_back(std::stoi(parts[1]) - 1);
                    }
                    else if (parts.size() == 3) {
                        // 정점 + 텍스처 + 노멀
                        faceVertices.push_back(std::stoi(parts[0]) - 1);
                        faceTexCoords.push_back(std::stoi(parts[1]) - 1);
                        faceNormals.push_back(std::stoi(parts[2]) - 1);
                    }
                }

                // 활성화된 텍스처에 해당하는 face 추가
                FFace newFace;
                newFace.Vertices = TArray<int>(faceVertices.begin(), faceVertices.end());
                newFace.TexCoords = TArray<int>(faceTexCoords.begin(), faceTexCoords.end());
                newFace.Normals = TArray<int>(faceNormals.begin(), faceNormals.end());

                if(CurrentMaterial.length()!=0)
                    OutObjInfo.Faces[CurrentMaterial]=newFace;
                else
                    OutObjInfo.Faces[L"None"] = newFace;
            }
        }
        ObjFile.close();

        return true;
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
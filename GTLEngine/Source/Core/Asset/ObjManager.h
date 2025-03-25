#pragma once

#include "Mesh/StaticMeshAsset.h"
#include "Mesh/UStaticMesh.h"
#include "ObjectFactory.h"

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

        FVector TempVector;
        FVector2 TempVector2;

        if (!ObjFile.is_open()) {
            throw std::runtime_error("Failed to open OBJ file.");
            return false;
        }

        OutObjInfo.PathFileName = UGTLStringLibrary::StringToWString(FileName);
        while (std::getline(ObjFile, line))
        {

            std::stringstream ss(line);
            std::string token;

            // Skip empty lines and comment lines (lines starting with #)
            if (line.empty() || line[0] == '#') continue;
            line = UGTLStringLibrary::StringRemoveNoise(line);

            // Handle vertices (v)
            if (line.substr(0, 2) == "v ") {
                static FVector Vertex;
                static FVector4 Color;
                Color = { 0.0f,0.0f, 0.0f, 1.0f }; // Default color: white

                // Parse vertex coordinates
                ss >> token >> TempVector.X >> TempVector.Y >> TempVector.Z;
                OutObjInfo.Vertices.push_back({ TempVector });

                // Check if color (RGBA) is provided (i.e., 4 components)
                if (ss >> Color.X >> Color.Y >> Color.Z >> Color.W) {
                    // If we successfully read 4 components, this means we have RGBA color
                    OutObjInfo.Colors.push_back(Color);
                }
                else
                    OutObjInfo.Colors.push_back(Color);
                // If no color was provided, use default color (white)
            }
            // Handle normals (vn)
            else if (line.substr(0, 3) == "vn ") {
                static FVector normal;
                ss >> token >> normal.X >> normal.Y >> normal.Z;
                OutObjInfo.Normals.push_back(FVector{ normal });  // 0 is a placeholder for index
            }
            // Handle texture coordinates (vt)
            else if (line.substr(0, 3) == "vt ") {
                static FVector2 uv;
                ss >> token >> uv.X >> uv.Y;
                uv.Y = -uv.Y;
                OutObjInfo.UV.push_back({ uv });  // 0 is a placeholder for index
            }
            // Handle face (f) which contains vertex indices
            // Handle material library (mtllib)
            else if (line.substr(0, 7) == "mtllib ") {
                static std::string MtlFileName;  // std::string으로 먼저 읽고
                ss >> token >> MtlFileName;
                MtlFileName = InPath + "/" + MtlFileName;
                
                static FObjMaterialInfo NewMtlInfo;
                if (!ParseMtlFile(InPath, MtlFileName, OutObjInfo.Materials))
                {
                    return false;
                }
            }
            // Handle material usage (usemtl)
            else if (line.substr(0, 7) == "usemtl ")
            {
                // "usemtl " 이후의 모든 텍스트를 메터리얼 이름으로 사용
                std::string MtlName = line.substr(7);  // "usemtl " 이후부터 끝까지

                // 메터리얼 이름을 WString으로 변환
                CurrentMaterial = UGTLStringLibrary::StringToWString(MtlName);

                // FaceMap에 메터리얼이 없으면 새로 추가
                if (!OutObjInfo.FaceMap.contains(CurrentMaterial)) {
                    OutObjInfo.FaceMap.insert(make_pair(CurrentMaterial, TArray<FFace>()));
                }

            }
            else if (line.substr(0, 2) == "f ") {
                TArray<int> faceVertices;
                TArray<int> faceTexCoords;
                TArray<int> faceNormals;

                // 면 정보 파싱
                std::stringstream faceStream(line);
                std::string faceToken;

                faceStream >> faceToken; // f 날리기

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
                    else if (parts[1] == "") {
                        // 정점 + 텍스처
                        faceVertices.push_back(std::stoi(parts[0]) - 1);
                        faceNormals.push_back(std::stoi(parts[2]) - 1);
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

              //  if (CurrentMaterial.length() != 0)
                    OutObjInfo.FaceMap[CurrentMaterial].push_back(newFace);

            }
            TempVector.X = TempVector.Y = TempVector.Z = 0;
            TempVector2.X = TempVector2.Y = 0;
        }
        ObjFile.close();

        return true;
    }

    static bool ParseMtlFile(const std::string& PathName, const std::string& InFileName, TArray<FObjMaterialInfo>& InfoArray)
    {
        std::ifstream MtlFile(InFileName.c_str());

        if (!MtlFile.is_open()) {
            throw std::runtime_error("Failed to open OBJ file.");
        }

        FObjMaterialInfo NewMatInfo = {};

        std::string line;
        while (std::getline(MtlFile, line))
        {
            if (line.empty() || line[0] == '#') continue;
            line = UGTLStringLibrary::StringRemoveNoise(line);  // 기존 노이즈 제거 코드 유지

            size_t pos = 0;
            while (pos < line.length() && line[pos] == ' ') pos++;  // 앞쪽 공백 제거

            if (pos >= line.length()) continue;  // 공백만 있는 라인은 건너뛰기

            // 새로운 메터리얼 처리
            if (line.compare(pos, 6, "newmtl") == 0)
            {
                if (NewMatInfo.MatName != TEXT(""))
                {
                    InfoArray.push_back(NewMatInfo);
                    NewMatInfo = {};
                }

                pos += 7;  // "newmtl" 뒤의 공백 건너뛰기
                while (pos < line.length() && line[pos] == ' ') pos++;  // 공백 건너뛰기

                // 메터리얼 이름에 공백이 있을 수 있으므로, 한 줄에서 나머지 부분을 다 읽어오기
                size_t endPos = line.find_first_of("\r\n", pos);  // 개행문자나 캐리지 리턴을 만나면 끝
                if (endPos == std::string::npos) endPos = line.length();  // 개행문자가 없으면 끝까지

                std::string matName = line.substr(pos, endPos - pos);
                NewMatInfo.MatName = UGTLStringLibrary::StringToWString(matName);
                continue;
            }

            // Ka, Kd, Ks 등 메터리얼의 다른 속성들 처리
            if (line.compare(pos, 2, "Ka") == 0)
            {
                pos += 3;
                float r, g, b;
                sscanf_s(line.substr(pos).c_str(), "%f %f %f", &r, &g, &b);
                NewMatInfo.Ka = FVector(r, g, b);
                continue;
            }
            else if (line.compare(pos, 2, "Kd") == 0)
            {
                pos += 3;
                float r, g, b;
                sscanf_s(line.substr(pos).c_str(), "%f %f %f", &r, &g, &b);
                NewMatInfo.Kd = FVector(r, g, b);
                continue;
            }
            else if (line.compare(pos, 2, "Ks") == 0)
            {
                pos += 3;
                float r, g, b;
                sscanf_s(line.substr(pos).c_str(), "%f %f %f", &r, &g, &b);
                NewMatInfo.Ks = FVector(r, g, b);
                continue;
            }
            else if (line.compare(pos, 2, "Ke") == 0)
            {
                pos += 3;
                float r, g, b;
                sscanf_s(line.substr(pos).c_str(), "%f %f %f", &r, &g, &b);
                NewMatInfo.Ke = FVector(r, g, b);
                continue;
            }

            // 추가적인 속성들 처리
            else if (line.compare(pos, 5, "illum") == 0)
            {
                pos += 6;
                int illum;
                sscanf_s(line.substr(pos).c_str(), "%d", &illum);
                NewMatInfo.Illum = illum;
                continue;
            }
            else if (line.compare(pos, 2, "Ns") == 0)
            {
                pos += 3;
                float ns;
                sscanf_s(line.substr(pos).c_str(), "%f", &ns);
                NewMatInfo.Ns = ns;
                continue;
            }
            else if (line.compare(pos, 1, "d") == 0)
            {
                pos += 2;
                float d;
                sscanf_s(line.substr(pos).c_str(), "%f", &d);
                NewMatInfo.d = d;
                continue;
            }
            else if (line.compare(pos, 2, "Tr") == 0)
            {
                pos += 3;
                float tr;
                sscanf_s(line.substr(pos).c_str(), "%f", &tr);
                NewMatInfo.Tr = tr;
                continue;
            }
            else if (line.compare(pos, 2, "Ni") == 0)
            {
                pos += 3;
                float ni;
                sscanf_s(line.substr(pos).c_str(), "%f", &ni);
                NewMatInfo.Ni = ni;
                continue;
            }
            else if (line.compare(pos, 6, "map_Ka") == 0)
            {
                pos += 7;
                size_t endPos = line.find_first_of(" \t", pos);
                std::string mapKa = line.substr(pos, endPos - pos);
                NewMatInfo.MapKa = UGTLStringLibrary::StringToWString(PathName + "/" + mapKa);
                continue;
            }
            else if (line.compare(pos, 6, "map_Kd") == 0)
            {
                pos += 7;
                size_t endPos = line.find_first_of(" \t", pos);
                std::string mapKd = line.substr(pos, endPos - pos);
                NewMatInfo.MapKd = UGTLStringLibrary::StringToWString(PathName + "/" + mapKd);
                continue;
            }
            else if (line.compare(pos, 6, "map_Ks") == 0)
            {
                pos += 7;
                size_t endPos = line.find_first_of(" \t", pos);
                std::string mapKs = line.substr(pos, endPos - pos);
                NewMatInfo.MapKs = UGTLStringLibrary::StringToWString(PathName + "/" + mapKs);
                continue;
            }
            else if (line.compare(pos, 6, "map_Ke") == 0)
            {
                pos += 7;
                size_t endPos = line.find_first_of(" \t", pos);
                std::string mapKe = line.substr(pos, endPos - pos);
                NewMatInfo.MapKe = UGTLStringLibrary::StringToWString(PathName + "/" + mapKe);
                continue;
            }
            else if (line.compare(pos, 8, "map_Bump") == 0)
            {
                pos += 9;
                size_t endPos = line.find_first_of(" \t", pos);
                std::string mapBump = line.substr(pos, endPos - pos);
                NewMatInfo.MapBump = UGTLStringLibrary::StringToWString(PathName + "/" + mapBump);
                continue;
            }
            else if (line.compare(pos, 12, "map_Displace") == 0)
            {
                pos += 13;
                size_t endPos = line.find_first_of(" \t", pos);
                std::string mapDisplace = line.substr(pos, endPos - pos);
                NewMatInfo.MapDisplace = UGTLStringLibrary::StringToWString(PathName + "/" + mapDisplace);
                continue;
            }
            else
            {
                // Unknown token 처리
              //  NewMatInfo = FObjMaterialInfo();
               // return false;
            }

        }
        MtlFile.close();
        if (NewMatInfo.MatName != TEXT(""))
            InfoArray.push_back(NewMatInfo);

        return true;
    }

};

class FObjManager
{
private:
    static TMap<FString, FStaticMesh*> ObjStaticMeshMap; // TMap 기반의 메시 데이터

public:
    static FStaticMesh* ConvertObjToStaticMesh(const FObjInfo& ObjInfo);;

    // FStaticMesh를 로드하는 함수 (Cooked Data)
    static FStaticMesh* LoadObjStaticMeshAsset(const FString& PathFileName);

    static UStaticMesh* LoadObjStaticMesh(const FString& PathFileName);

};


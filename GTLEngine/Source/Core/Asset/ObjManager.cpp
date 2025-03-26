#include "pch.h"
#include "ObjManager.h"
#include "Material.h"
#include <filesystem>

TMap<FString, FStaticMesh*> FObjManager::ObjStaticMeshMap;

std::string CreateVertexKey(int VertexIndex, int NormalIndex, int UVIndex)
{
	std::string key;
	return key.append(std::to_string(VertexIndex)).append(std::to_string(NormalIndex)).append(std::to_string(UVIndex));
}

FStaticMesh* FObjManager::LoadObjStaticMeshAsset(const FString& PathFileName)
{
	// 이미 있으면 바로 반환.
	if (ObjStaticMeshMap.contains(PathFileName))
	{
		return ObjStaticMeshMap[PathFileName];
	}
	// OBJ Parsing and create a new FStaticMesh
	// PathFileName으로 Obj 파싱하고 나온 ObjInfo로 FStaticMesh 생성해서 Map에 추가.

	// 없으면 만들어서 map에 캐싱 후 반환.
	size_t offset = PathFileName.find_last_of(L"/");
	std::string PathName = UGTLStringLibrary::WStringToString(PathFileName.substr(0, offset));
	std::string FileName = UGTLStringLibrary::WStringToString(PathFileName.substr(offset + 1, PathFileName.size()));

	FObjInfo NewObj;

	FObjImporter::ParseObjFile(PathName, FileName, NewObj);
	
	FStaticMesh* NewStaticMesh = FObjManager::ConvertObjToStaticMesh(NewObj);
	ObjStaticMeshMap.emplace(NewStaticMesh->PathFileName, NewStaticMesh);
	return ObjStaticMeshMap[PathFileName];
}

UStaticMesh* FObjManager::LoadObjStaticMesh(const FString& PathFileName)
{
	for (TObjectIterator<UStaticMesh> It; It; ++It)
	{
		UStaticMesh* StaticMesh = *It;
		if (StaticMesh->GetAssetPathFileName() == PathFileName)
			return *It;
	}

	FStaticMesh* Asset = FObjManager::LoadObjStaticMeshAsset(PathFileName);
	UStaticMesh* StaticMesh = FObjectFactory::ConstructObject<UStaticMesh>();
	StaticMesh->SetStaticMeshAsset(Asset);
	return StaticMesh;
}


FStaticMesh* FObjManager::ConvertObjToStaticMesh(const FObjInfo& ObjInfo)
{
	FStaticMesh* NewStaticMesh = new FStaticMesh();
	NewStaticMesh->PathFileName = ObjInfo.PathFileName;

	int IndexCount = 0;

	for (const auto& MaterialFacePair : ObjInfo.FaceMap)
	{
		FString MaterialName = MaterialFacePair.first;
		const TArray<FFace>& Faces = MaterialFacePair.second;

		FStaticMeshSection NewSection;
		NewSection.MaterialName = MaterialName;

		IndexCount = 0;
		TMap<std::string, int> UniqueVertexMap;

		for (int b = 0;b < Faces.size();b++)
		{
			for (int a = 0;a < 3;a++) // 3 : Triangulated Face 만 적용 가능.
			{
				int VertexIndex = Faces[b].Vertices[a];
				// UV 및 Normal이 존재하는지 확인 후 인덱스 설정
				int UVIndex = (Faces[b].TexCoords.size() > 0) ? Faces[b].TexCoords[a] : 0;
				int NormalIndex = (Faces[b].Normals.size() > 0) ? Faces[b].Normals[a] : 0;

				// 정점의 Key 생성 (정점, 노멀, UV를 기반으로 Key 생성)
				std::string VertexKey = CreateVertexKey(VertexIndex, NormalIndex, UVIndex);

				// 이미 존재하는 정점이면 건너뛰기
				if (UniqueVertexMap.contains(VertexKey))
				{
					NewSection.Indices.push_back(UniqueVertexMap[VertexKey]);
					continue;
				}

				// 새로운 정점이면 정점 정보를 추가
				FVector Position = ObjInfo.Vertices[VertexIndex];
				FVector4 Color =  ObjInfo.Colors.empty()?FVector4(1.0f,1.0,1.0f,1.0f) : ObjInfo.Colors[VertexIndex];
				FVector Normal = ObjInfo.Normals.empty() ? FVector() : ObjInfo.Normals[NormalIndex];
				FVector2 UV = ObjInfo.UV.empty()? FVector2(0, 0):ObjInfo.UV[UVIndex];

				FVertexPNCT Vertex(Position, Normal, Color, UV);
				NewSection.Vertices.push_back(Vertex);
				NewStaticMesh->Vertices.push_back(Position);

				// 새로운 정점 인덱스 맵핑
				UniqueVertexMap[VertexKey] = IndexCount;
				NewSection.Indices.push_back(IndexCount);  // 새로 추가된 정점의 인덱스를 인덱스 버퍼에 추가

				IndexCount++;  // 인덱스 카운트를 증가
			}
		}

		NewStaticMesh->Sections.push_back(NewSection);

	}



	for (const FObjMaterialInfo& MaterialInfo : ObjInfo.Materials)
	{
		// MaterialInfo를 이용하여 Material을 생성하고 StaticMesh에 추가
		UMaterial* NewMaterial = new UMaterial();
		NewMaterial->InitMaterial(MaterialInfo);
		FMaterialManager::CreateMaterial(MaterialInfo.MatName, MaterialInfo);
	}

	return NewStaticMesh;
}

bool FObjImporter::ParseObjFile(const std::string& InPath, const std::string& InFileName, FObjInfo& OutObjInfo)
{
	std::string BinaryFileName = InFileName.substr(0, InFileName.size() - 4) + ".bin";
	std::string BinaryPathName = "Binary/";

	std::ifstream BinFile(BinaryPathName+BinaryFileName, std::ios::binary);

	///If binary file is open read that;
	if (BinFile.is_open())
	{
		LoadObjFromBinary(BinaryPathName + BinaryFileName, OutObjInfo);
		return true;
	}

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

	SaveObjToBinary(BinaryFileName,OutObjInfo);

	return true;
}

bool FObjImporter::ParseMtlFile(const std::string& PathName, const std::string& InFileName, TArray<FObjMaterialInfo>& InfoArray)
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

void FObjImporter::SaveObjToBinary(const std::string& BinFileName, const FObjInfo& ObjInfo)
{
	std::filesystem::path BinaryDir = "Binary"; // Binary 폴더 지정
	std::filesystem::create_directories(BinaryDir); // 폴더가 없으면 생성

	std::filesystem::path FilePath = BinaryDir/(BinFileName); // 파일 경로 생

	std::ofstream BinFile(FilePath.generic_string(), std::ios::binary);
	if (!BinFile.is_open()) {
		std::cerr << "Failed to open binary file for writing: " << FilePath << std::endl;
		return;
	}

	uint32_t MatrialLen = ObjInfo.Materials.size();
	BinFile.write(reinterpret_cast<const char*>(&MatrialLen), sizeof(uint32_t));

	for (auto Material : ObjInfo.Materials)
	{
		// MatName 저장 (길이 + 문자열)
		uint32_t MatNameLen = Material.MatName.length();
		BinFile.write(reinterpret_cast<const char*>(&MatNameLen), sizeof(uint32_t));
		BinFile.write(reinterpret_cast<const char*>(Material.MatName.c_str()), MatNameLen * sizeof(wchar_t));


		// Ka (Ambient Reflection)
		BinFile.write(reinterpret_cast<const char*>(&Material.Ka), sizeof(FVector));

		// Kd (Diffuse Reflection)
		BinFile.write(reinterpret_cast<const char*>(&Material.Kd), sizeof(FVector));

		// Ks (Specular Reflection)
		BinFile.write(reinterpret_cast<const char*>(&Material.Ks), sizeof(FVector));

		// Ke (Emissive Reflectivity)
		BinFile.write(reinterpret_cast<const char*>(&Material.Ke), sizeof(FVector));

		// Ns (Specular Exponent)
		BinFile.write(reinterpret_cast<const char*>(&Material.Ns), sizeof(float));

		// Illum (Illumination Model)
		BinFile.write(reinterpret_cast<const char*>(&Material.Illum), sizeof(int));

		// d (Opacity)
		BinFile.write(reinterpret_cast<const char*>(&Material.d), sizeof(float));

		// Tr (Transparency)
		BinFile.write(reinterpret_cast<const char*>(&Material.Tr), sizeof(float));

		// Ni (Refractive Index)
		BinFile.write(reinterpret_cast<const char*>(&Material.Ni), sizeof(float));

		// MapKa (Ambient Texture Map)
		uint32_t MapKaLen = Material.MapKa.length();
		BinFile.write(reinterpret_cast<const char*>(&MapKaLen), sizeof(uint32_t));
		BinFile.write(reinterpret_cast<const char*>(Material.MapKa.c_str()), MapKaLen * sizeof(wchar_t));

		// MapKd (Diffuse Texture Map)
		uint32_t MapKdLen = Material.MapKd.length();
		BinFile.write(reinterpret_cast<const char*>(&MapKdLen), sizeof(uint32_t));
		BinFile.write(reinterpret_cast<const char*>(Material.MapKd.c_str()), MapKdLen * sizeof(wchar_t));

		// MapKs (Specular Texture Map)
		uint32_t MapKsLen = Material.MapKs.length();
		BinFile.write(reinterpret_cast<const char*>(&MapKsLen), sizeof(uint32_t));
		BinFile.write(reinterpret_cast<const char*>(Material.MapKs.c_str()), MapKsLen * sizeof(wchar_t));

		// MapKe (Emissive Texture Map)
		uint32_t MapKeLen = Material.MapKe.length();
		BinFile.write(reinterpret_cast<const char*>(&MapKeLen), sizeof(uint32_t));
		BinFile.write(reinterpret_cast<const char*>(Material.MapKe.c_str()), MapKeLen * sizeof(wchar_t));

		// MapBump (Bump Map)
		uint32_t MapBumpLen = Material.MapBump.length();
		BinFile.write(reinterpret_cast<const char*>(&MapBumpLen), sizeof(uint32_t));
		BinFile.write(reinterpret_cast<const char*>(Material.MapBump.c_str()), MapBumpLen * sizeof(wchar_t));

		// MapDisplace (Displacement Map)
		uint32_t MapDisplaceLen = Material.MapDisplace.length();
		BinFile.write(reinterpret_cast<const char*>(&MapDisplaceLen), sizeof(uint32_t));
		BinFile.write(reinterpret_cast<const char*>(Material.MapDisplace.c_str()), MapDisplaceLen * sizeof(wchar_t));
	}


	// 문자열 길이 저장
	uint32_t PathFileNameLen = ObjInfo.PathFileName.length();
	BinFile.write(reinterpret_cast<const char*>(&PathFileNameLen), sizeof(uint32_t));

	BinFile.write(reinterpret_cast<const char*>(ObjInfo.PathFileName.c_str()), PathFileNameLen * sizeof(wchar_t));

	// 1. Vertex 개수 저장
	uint32_t VertexCount = ObjInfo.Vertices.size();
	BinFile.write(reinterpret_cast<const char*>(&VertexCount), sizeof(uint32_t));

	// 2. Vertex 데이터 저장
	for (const auto& Vertex : ObjInfo.Vertices) {
		BinFile.write(reinterpret_cast<const char*>(&Vertex), sizeof(FVector));
	}

	// 1. Color 개수 저장
	uint32_t ColorCount = ObjInfo.Colors.size();
	BinFile.write(reinterpret_cast<const char*>(&ColorCount), sizeof(uint32_t));

	// 2. Color 데이터 저장
	for (const auto& Color : ObjInfo.Colors) {
		BinFile.write(reinterpret_cast<const char*>(&Color), sizeof(FVector4));
	}


	// 3. Normal 개수 저장
	uint32_t NormalCount = ObjInfo.Normals.size();
	BinFile.write(reinterpret_cast<const char*>(&NormalCount), sizeof(uint32_t));

	// 4. Normal 데이터 저장
	for (const auto& Normal : ObjInfo.Normals) {
		BinFile.write(reinterpret_cast<const char*>(&Normal), sizeof(FVector));
	}

	// 5. UV 개수 저장
	uint32_t UVCount = ObjInfo.UV.size();
	BinFile.write(reinterpret_cast<const char*>(&UVCount), sizeof(uint32_t));

	// 6. UV 데이터 저장
	for (const auto& UV : ObjInfo.UV) {
		BinFile.write(reinterpret_cast<const char*>(&UV), sizeof(FVector2));
	}

	// 7. FaceMap 개수 저장
	uint32_t FaceMapSize = ObjInfo.FaceMap.size();
	BinFile.write(reinterpret_cast<const char*>(&FaceMapSize), sizeof(uint32_t));

	// 8. FaceMap 데이터 저장
	for (const auto& [MaterialName, Faces] : ObjInfo.FaceMap)
	{
		// 문자열 길이 저장
		uint32_t MaterialNameLength = MaterialName.length();
		BinFile.write(reinterpret_cast<const char*>(&MaterialNameLength), sizeof(uint32_t));

		BinFile.write(reinterpret_cast<const char*>(MaterialName.c_str()), MaterialNameLength * sizeof(wchar_t));

		uint32_t FaceCount = Faces.size();
		BinFile.write(reinterpret_cast<const char*>(&FaceCount), sizeof(uint32_t));

		// Face 데이터 저장
		for (const auto& Face : Faces)
		{
			uint32_t VertexSize = Face.Vertices.size();
			BinFile.write(reinterpret_cast<const char*>(&VertexSize), sizeof(uint32_t));
			BinFile.write(reinterpret_cast<const char*>(Face.Vertices.data()), VertexSize * sizeof(int));

			uint32_t TexCoordSize = Face.TexCoords.size();
			BinFile.write(reinterpret_cast<const char*>(&TexCoordSize), sizeof(uint32_t));
			BinFile.write(reinterpret_cast<const char*>(Face.TexCoords.data()), TexCoordSize * sizeof(int));

			uint32_t NormalSize = Face.Normals.size();
			BinFile.write(reinterpret_cast<const char*>(&NormalSize), sizeof(uint32_t));
			BinFile.write(reinterpret_cast<const char*>(Face.Normals.data()), NormalSize * sizeof(int));
		}
	}

	BinFile.close();
	std::cout << "OBJ successfully saved as binary: " << BinFileName << std::endl;
	return;
}

void FObjImporter::LoadObjFromBinary(const std::string& BinFileName, FObjInfo& ObjInfo)
{
	std::ifstream BinFile(BinFileName, std::ios::binary);
	if (!BinFile.is_open()) {
		std::cerr << "Failed to open binary file for reading: " << BinFileName << std::endl;
		return;
	}

	// 1. Vertex 개수 읽기
	uint32_t MatrialCount;
	BinFile.read(reinterpret_cast<char*>(&MatrialCount), sizeof(uint32_t));


	for (int a = 0;a < MatrialCount;a++)
	{
		FObjMaterialInfo NewMatInfo;

		// MatName 길이 읽기
		uint32_t MatNameLen;
		BinFile.read(reinterpret_cast<char*>(&MatNameLen), sizeof(uint32_t));

		// MatName 읽기
		std::wstring MatName(MatNameLen, L'\0');
		BinFile.read(reinterpret_cast<char*>(&MatName[0]), MatNameLen * sizeof(wchar_t));
		NewMatInfo.MatName = MatName;

		// Ka (Ambient Reflection)
		BinFile.read(reinterpret_cast<char*>(&NewMatInfo.Ka), sizeof(FVector));

		// Kd (Diffuse Reflection)
		BinFile.read(reinterpret_cast<char*>(&NewMatInfo.Kd), sizeof(FVector));

		// Ks (Specular Reflection)
		BinFile.read(reinterpret_cast<char*>(&NewMatInfo.Ks), sizeof(FVector));

		// Ke (Emissive Reflectivity)
		BinFile.read(reinterpret_cast<char*>(&NewMatInfo.Ke), sizeof(FVector));

		// Ns (Specular Exponent)
		BinFile.read(reinterpret_cast<char*>(&NewMatInfo.Ns), sizeof(float));

		// Illum (Illumination Model)
		BinFile.read(reinterpret_cast<char*>(&NewMatInfo.Illum), sizeof(int));

		// d (Opacity)
		BinFile.read(reinterpret_cast<char*>(&NewMatInfo.d), sizeof(float));

		// Tr (Transparency)
		BinFile.read(reinterpret_cast<char*>(&NewMatInfo.Tr), sizeof(float));

		// Ni (Refractive Index)
		BinFile.read(reinterpret_cast<char*>(&NewMatInfo.Ni), sizeof(float));

		// MapKa 읽기
		uint32_t MapKaLen;
		BinFile.read(reinterpret_cast<char*>(&MapKaLen), sizeof(uint32_t));
		std::wstring MapKa(MapKaLen, L'\0');
		BinFile.read(reinterpret_cast<char*>(&MapKa[0]), MapKaLen * sizeof(wchar_t));
		NewMatInfo.MapKa = MapKa;

		// MapKd 읽기
		uint32_t MapKdLen;
		BinFile.read(reinterpret_cast<char*>(&MapKdLen), sizeof(uint32_t));
		std::wstring MapKd(MapKdLen, L'\0');
		BinFile.read(reinterpret_cast<char*>(&MapKd[0]), MapKdLen * sizeof(wchar_t));
		NewMatInfo.MapKd = MapKd;

		// MapKs 읽기
		uint32_t MapKsLen;
		BinFile.read(reinterpret_cast<char*>(&MapKsLen), sizeof(uint32_t));
		std::wstring MapKs(MapKsLen, L'\0');
		BinFile.read(reinterpret_cast<char*>(&MapKs[0]), MapKsLen * sizeof(wchar_t));
		NewMatInfo.MapKs = MapKs;

		// MapKe 읽기
		uint32_t MapKeLen;
		BinFile.read(reinterpret_cast<char*>(&MapKeLen), sizeof(uint32_t));
		std::wstring MapKe(MapKeLen, L'\0');
		BinFile.read(reinterpret_cast<char*>(&MapKe[0]), MapKeLen * sizeof(wchar_t));
		NewMatInfo.MapKe = MapKe;

		// MapBump 읽기
		uint32_t MapBumpLen;
		BinFile.read(reinterpret_cast<char*>(&MapBumpLen), sizeof(uint32_t));
		std::wstring MapBump(MapBumpLen, L'\0');
		BinFile.read(reinterpret_cast<char*>(&MapBump[0]), MapBumpLen * sizeof(wchar_t));
		NewMatInfo.MapBump = MapBump;

		// MapDisplace 읽기
		uint32_t MapDisplaceLen;
		BinFile.read(reinterpret_cast<char*>(&MapDisplaceLen), sizeof(uint32_t));
		std::wstring MapDisplace(MapDisplaceLen, L'\0');
		BinFile.read(reinterpret_cast<char*>(&MapDisplace[0]), MapDisplaceLen * sizeof(wchar_t));
		NewMatInfo.MapDisplace = MapDisplace;

		// 읽은 자료를 Materials 배열에 추가
		ObjInfo.Materials.push_back(NewMatInfo);
	}


	uint32_t PathFileNameLen;
	BinFile.read(reinterpret_cast<char*>(&PathFileNameLen), sizeof(uint32_t));

	std::wstring PathFileName(PathFileNameLen, L'\0');
	BinFile.read(reinterpret_cast<char*>(&PathFileName[0]), PathFileNameLen * sizeof(wchar_t));

	ObjInfo.PathFileName = PathFileName;

	// 1. Vertex 개수 읽기
	uint32_t VertexCount;
	BinFile.read(reinterpret_cast<char*>(&VertexCount), sizeof(uint32_t));

	// 2. Vertex 데이터 읽기
	ObjInfo.Vertices.resize(VertexCount);
	BinFile.read(reinterpret_cast<char*>(ObjInfo.Vertices.data()), VertexCount * sizeof(FVector));


	// 1. Color 개수 읽기
	uint32_t ColorCount;
	BinFile.read(reinterpret_cast<char*>(&ColorCount), sizeof(uint32_t));

	// 2. Color 데이터 읽기
	ObjInfo.Colors.resize(ColorCount);
	BinFile.read(reinterpret_cast<char*>(ObjInfo.Colors.data()), ColorCount * sizeof(FVector4));


	// 3. Normal 개수 읽기
	uint32_t NormalCount;
	BinFile.read(reinterpret_cast<char*>(&NormalCount), sizeof(uint32_t));

	// 4. Normal 데이터 읽기
	ObjInfo.Normals.resize(NormalCount);
	BinFile.read(reinterpret_cast<char*>(ObjInfo.Normals.data()), NormalCount * sizeof(FVector));

	// 5. UV 개수 읽기
	uint32_t UVCount;
	BinFile.read(reinterpret_cast<char*>(&UVCount), sizeof(uint32_t));

	// 6. UV 데이터 읽기
	ObjInfo.UV.resize(UVCount);
	BinFile.read(reinterpret_cast<char*>(ObjInfo.UV.data()), UVCount * sizeof(FVector2));

	// 7. FaceMap 개수 읽기
	uint32_t FaceMapSize;
	BinFile.read(reinterpret_cast<char*>(&FaceMapSize), sizeof(uint32_t));

	// 8. FaceMap 데이터 읽기
	for (uint32_t i = 0; i < FaceMapSize; ++i)
	{
		uint32_t MaterialNameLength;
		BinFile.read(reinterpret_cast<char*>(&MaterialNameLength), sizeof(uint32_t));

		std::wstring MaterialName(MaterialNameLength, '\0');
		BinFile.read(reinterpret_cast<char*>(&MaterialName[0]), MaterialNameLength * sizeof(wchar_t));

		uint32_t FaceCount;
		BinFile.read(reinterpret_cast<char*>(&FaceCount), sizeof(uint32_t));

		TArray<FFace> Faces;
		for (uint32_t j = 0; j < FaceCount; ++j)
		{
			FFace newFace;

			uint32_t VertexSize;
			BinFile.read(reinterpret_cast<char*>(&VertexSize), sizeof(uint32_t));
			newFace.Vertices.resize(VertexSize);
			BinFile.read(reinterpret_cast<char*>(newFace.Vertices.data()), VertexSize * sizeof(int));

			uint32_t TexCoordSize;
			BinFile.read(reinterpret_cast<char*>(&TexCoordSize), sizeof(uint32_t));
			newFace.TexCoords.resize(TexCoordSize);
			BinFile.read(reinterpret_cast<char*>(newFace.TexCoords.data()), TexCoordSize * sizeof(int));

			uint32_t NormalSize;
			BinFile.read(reinterpret_cast<char*>(&NormalSize), sizeof(uint32_t));
			newFace.Normals.resize(NormalSize);
			BinFile.read(reinterpret_cast<char*>(newFace.Normals.data()), NormalSize * sizeof(int));

			Faces.push_back(newFace);
		}

		ObjInfo.FaceMap[MaterialName] = Faces;
	}

	BinFile.close();
	std::cout << "OBJ binary loaded successfully: " << BinFileName << std::endl;
	return;
}

#include "pch.h"
#include "ObjManager.h"
#include "Material.h"


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

	//SaveObjToBinary("TestBin",NewObj);

	//FObjInfo NewObj2;
	//LoadObjFromBinary("TestBin", NewObj2);

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

void FObjManager::SaveObjToBinary(const std::string& BinFileName, const FObjInfo& ObjInfo)
{
	std::ofstream BinFile(BinFileName, std::ios::binary);
	if (!BinFile.is_open()) {
		std::cerr << "Failed to open binary file for writing: " << BinFileName << std::endl;
		return;
	}

	// 1. Vertex 개수 저장
	uint32_t VertexCount = ObjInfo.Vertices.size();
	BinFile.write(reinterpret_cast<const char*>(&VertexCount), sizeof(uint32_t));

	// 2. Vertex 데이터 저장
	for (const auto& Vertex : ObjInfo.Vertices) {
		BinFile.write(reinterpret_cast<const char*>(&Vertex), sizeof(FVector));
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
		uint32_t MaterialNameLength = 4;//MaterialName.length();
		BinFile.write(reinterpret_cast<const char*>(&MaterialNameLength), sizeof(uint32_t));

		BinFile.write(reinterpret_cast<const char*>(MaterialName.c_str()), MaterialNameLength * sizeof(wchar_t));


		// Face 개수 저장
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

void FObjManager::LoadObjFromBinary(const std::string& BinFileName, FObjInfo& ObjInfo)
{
	std::ifstream BinFile(BinFileName, std::ios::binary);
	if (!BinFile.is_open()) {
		std::cerr << "Failed to open binary file for reading: " << BinFileName << std::endl;
		return;
	}

	// 1. Vertex 개수 읽기
	uint32_t VertexCount;
	BinFile.read(reinterpret_cast<char*>(&VertexCount), sizeof(uint32_t));

	// 2. Vertex 데이터 읽기
	ObjInfo.Vertices.resize(VertexCount);
	BinFile.read(reinterpret_cast<char*>(ObjInfo.Vertices.data()), VertexCount * sizeof(FVector));

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
		BinFile.read(reinterpret_cast<char*>(&MaterialName[0]), MaterialNameLength);

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
				FVector4 Color = ObjInfo.Colors[VertexIndex];
				FVector Normal = ObjInfo.Normals.empty() ? FVector() : ObjInfo.Normals[NormalIndex];
				FVector2 UV = ObjInfo.UV.size() > 0 ? ObjInfo.UV[UVIndex] : FVector2(0, 0);

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


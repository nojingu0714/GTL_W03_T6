#pragma once

class UWorld;

/*
	Scene의 로드와 세이브.
	현재 Scene의 정보를 보유하도록 하여 UI에서 정보를 받아가 띄울 수 있도록 하는 역할.
*/
class FSceneManager
{

public:
	static void SaveScene(const std::string& InSceneName, const std::string& InPath = "Contents/Scene/");
	static void NewScene();
	static void LoadScene(const std::string& InSceneName);

};
#pragma once
#include "Systems/AINode.h"
#include "Components/SplineComponent.h"
class FileLoader
{
public:
	FileLoader(const FileLoader&) = delete;

	static FileLoader& GetSingleton();

	template <class Archive>
	void Save(Archive& ar, SIN::SplineComponent & m);
	
	template <class Archive>
	void Load(Archive& ar, SIN::SplineComponent & m);

	template <class Archive>
	void serialize(Archive& ar){}

private:
	FileLoader();
};
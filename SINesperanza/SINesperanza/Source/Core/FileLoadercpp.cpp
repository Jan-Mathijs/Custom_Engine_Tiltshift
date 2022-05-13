#include "Core/FileLoader.h"

template <class Archive>
void FileLoader::Save(Archive& ar, SIN::SplineComponent & m)
{
	ar(m.GetType(), m.GetPureTrackPoints(), m.GetLength());
	
}

template <class Archive>
void FileLoader::Load(Archive& ar, SIN::SplineComponent & m)
{
	ar(m.GetType(), m.GetPureTrackPoints(), m.GetLength());
}

FileLoader& FileLoader::GetSingleton()
{
	static FileLoader singleton;
	return singleton;
}
#pragma once
struct PlayerTag{};

struct SplineTag{};
struct StartScreen{};
struct WallTagLeft{};
struct WallTagRight {};
struct SpriteTag
{
	SpriteTag(std::string strin) { m_FileName = strin; }
	SpriteTag(){}
	std::string GetString() { return m_FileName; }
	private: std::string m_FileName;
};
struct AITag{};
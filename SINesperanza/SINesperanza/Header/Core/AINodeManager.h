#pragma once
#include "Systems/AINode.h"
#include <vector>
#include "cereal/archives/binary.hpp"
class AINodeManager
{
public:
	AINodeManager(const AINodeManager&) = delete;
	~AINodeManager();

	static AINodeManager& GetSingleton();
	
	void GenerateNodes(float a_Posx, float a_Posy, float a_Dirx, float a_Diry, float t);
	
	AINode* GetNode(int a_Index);

	void SafeNodes();

	void LoadNodes(bool a_load);

	bool IsTheMapLoaded() { return m_LoadMap; }

	void SetNodeDepth(int a_notedepht);
private:
	
	AINodeManager();

	std::vector<AINode> m_AINodes;

	uint32_t m_id;
	std::shared_ptr<std::unordered_map<uint32_t, AINode>> data;

	std::string m_FileName = "AINodesSave.cereal";

	bool m_LoadMap = false;

	template <class Archive>
	void Save(Archive& ar, AINode const& m);

	template <class Archive>
	void Load(Archive& ar, AINode const& m);

	int m_NodeDepth = 0;
};
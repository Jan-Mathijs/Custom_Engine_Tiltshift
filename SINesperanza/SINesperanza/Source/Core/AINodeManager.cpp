#include "Core/AINodeManager.h"
#include <fstream>

AINodeManager::AINodeManager()
{
	
}

AINodeManager::~AINodeManager()
{
	
}

AINode* AINodeManager::GetNode(int a_Index)
{
	int index = a_Index;
	if (a_Index >= m_NodeDepth) index = 0;
	AINode* temp = &m_AINodes.at(index);
	return temp;
}

AINodeManager& AINodeManager::GetSingleton()
{
	static AINodeManager singleton;
	return singleton;
}

void AINodeManager::GenerateNodes(float a_posx, float a_posy, float a_dirx, float a_diry, float m_t)
{
	if(m_LoadMap)
	{
		LoadNodes(true);
	}else
	{
		glm::vec2 tempPos = glm::vec2(a_posx, a_posy);
		glm::vec2 tempdir = glm::vec2(a_dirx, a_diry);
		AINode newNode(tempPos, tempdir, m_t);
		m_AINodes.push_back(newNode);
	}
	
}

void AINodeManager::SafeNodes()
{
	std::ofstream os(m_FileName.c_str(), std::ios::binary);
	cereal::BinaryOutputArchive archive(os);

	if (m_AINodes.size() == 0) return;
	for(int i = 0; i < m_AINodes.size(); i++)
	{
		Save(archive, m_AINodes.at(i));
	}
}

void AINodeManager::LoadNodes(bool a_load)
{
	if(a_load)
	{
		std::ofstream os(m_FileName.c_str(), std::ios::binary);
		cereal::BinaryOutputArchive archive(os);

		if (m_AINodes.size() != 0) return;
		for (int i = 0; i < m_AINodes.size(); i++)
		{
			Load(archive, m_AINodes.at(i));
		}
	}	
}

template <class Archive>
void AINodeManager::Save(Archive& ar, AINode const& m)
{
	ar(m.m_Direction.x, m.m_Direction.y, m.m_Position.x, m.m_Position.y, m.m_Speed);
}

template <class Archive>
void AINodeManager::Load(Archive& ar, AINode const& m)
{
	ar(m.m_Direction.x, m.m_Direction.y, m.m_Position.x, m.m_Position.y, m.m_Speed);
}

void AINodeManager::SetNodeDepth(int a_notedepht)
{
	m_NodeDepth = a_notedepht; 
}

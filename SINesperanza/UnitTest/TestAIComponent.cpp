#include <gtest/gtest.h>
#include "Components/AIComponent.h"
#include "Systems/AINode.h"

TEST(AIComponent, GetMass )
{
	
	SIN::AIComponent aiComp;
	aiComp.SetMass(500);
	EXPECT_EQ(aiComp.GetMass(), 500);	
}

TEST(AIComponent, Speed)
{
	SIN::AIComponent aiComp;
	aiComp.SetCurrentSpeed(500);
	EXPECT_EQ(aiComp.GetCurrentSpeed(), 500);
	EXPECT_EQ(aiComp.GetMaxSpeed(), 200);	
}

TEST(AIComponent, Node)
{
	SIN::AIComponent aiComp;
	aiComp.SetCurrentNode(5);	
	EXPECT_EQ(aiComp.GetCurrentNode(), 5);
	EXPECT_EQ(aiComp.GetNodeReadSpeed(), 1);
}

TEST(AIComponent, Acceleration)
{
	SIN::AIComponent aiComp;
	aiComp.SetAcceleration(500);	
	EXPECT_EQ(aiComp.GetAcceleration(), 500);
}

TEST(AIComponent, PositionOnSpline)
{
	SIN::AIComponent aiComp;
	aiComp.SetPositionOnSpline(0.5f);
	EXPECT_EQ(aiComp.GetPositionOnSpline(), 0.5f);
}

TEST(AIComponent, AIState)
{
	SIN::AIComponent aiComp;
	aiComp.SetAIState(SIN::AIState::Accelerate);
	EXPECT_EQ(aiComp.GetAIState(), SIN::AIState::Accelerate);
}

TEST(AIComponent, Index)
{
	SIN::AIComponent aiComp;
	aiComp.SetAIIndex(145);
	EXPECT_EQ(aiComp.GetAIIndex(), 145);
}
#include <gtest/gtest.h>
#include "Components/TransformComponent.h"

TEST(TransformComponent, GetWorldLocation)
{
	SIN::TransformComponent trans;
	trans.SetWorldLocation(glm::vec2(50, 50));
	EXPECT_EQ(trans.GetWorldLocation(), glm::vec2(50, 50));
}

TEST(TransformComponent, GetDirection)
{
	SIN::TransformComponent trans;
	trans.SetDirection(glm::vec2(0,1));
	EXPECT_EQ(trans.GetDirection(), glm::vec2(0, 1));
}

TEST(TransformComponent, GetRightDirection)
{
	SIN::TransformComponent trans;
	trans.SetRightDirection(glm::vec2(1, 0));
	EXPECT_EQ(trans.GetRightDirection(), glm::vec2(1, 0));
}

TEST(TransformComponent, GetZRotation)
{
	SIN::TransformComponent trans;
	trans.SetZRotation(50.f);
	EXPECT_EQ(trans.GetZRotation(), 50.f);
}

TEST(TransformComponent, GetScale)
{
	SIN::TransformComponent trans;
	trans.SetScale(glm::vec2(12,30));
	EXPECT_EQ(trans.GetScale(), glm::vec2(12,30));
}

TEST(TransformComponent, GetTransformMatrix)
{
	SIN::TransformComponent trans;
	trans.SetZRotation(50.f);
	trans.SetWorldLocation(glm::vec2(12, 34));
	trans.SetScale(glm::vec2(12, 6));
	glm::mat4 temp = trans.GetTransformMatrix();
 	EXPECT_EQ(trans.GetTransformMatrix(), glm::mat4(
		7.71345139,9.19253349,0,0,
		-4.59626675,3.85672569,0,0,
		0,0,1,0,
		12,34,0,1));
}

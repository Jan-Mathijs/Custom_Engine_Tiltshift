#include <gtest/gtest.h>
#include "Components/SplineComponent.h"

TEST(SplineComponent, Linear)
{
	SIN::SplineComponent spline;
	spline.SetType(SIN::SplineType::Linear);

	EXPECT_EQ(spline.GetPoints().size(), 1);
	
	spline.AddPoint(glm::vec2(10, 0));

	EXPECT_EQ(spline.GetPoints().size(), 2);

	EXPECT_EQ(spline.GetPoint(0.5f).x, 5);

	EXPECT_EQ(spline.GetNextSegmentIndex(0.5f), 1);
}

TEST(SplineComponent, Quadratic)
{
	SIN::SplineComponent spline;
	spline.SetType(SIN::SplineType::Bezier);

	EXPECT_EQ(spline.GetPoints().size(), 1);

	spline.AddPoint(glm::vec2(10, 0));

	EXPECT_EQ(spline.GetPoints().size(), 3);

	EXPECT_EQ(spline.GetPoint(0.5f).x, 5);

	EXPECT_EQ(spline.GetNextSegmentIndex(0.51f), 2);

	EXPECT_EQ(spline.FindClosestPoint(glm::vec2(5, 0)).x, 5);

	
}

TEST(SplineComponent, Cubic)
{
	SIN::SplineComponent spline;
	spline.SetType(SIN::SplineType::CubicBezier);

	EXPECT_EQ(spline.GetPoints().size(), 1);

	spline.AddPoint(glm::vec2(10, 0));

	EXPECT_EQ(spline.GetPoints().size(), 4);

	EXPECT_TRUE(5 - spline.GetPoint(0.5f).x <= 0.1f);
	
	EXPECT_EQ(spline.GetNextSegmentIndex(0.75f), 3);

	EXPECT_EQ(spline.FindClosestPoint(glm::vec2(20, 0)).x, 10);
}
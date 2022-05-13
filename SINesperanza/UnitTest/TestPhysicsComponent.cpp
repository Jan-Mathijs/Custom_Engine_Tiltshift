#include <gtest/gtest.h>
//#include "Box2d/box2d.h"
#include "Components/PhysicsComponent.h"
//#include "Core/WorldManager.h"
/*


TEST(PhysicsComponent, GetLateralVelocityGLM)
{
	b2World world = b2World(b2Vec2(0.0f, 0.0f));
	//code strait from the AI to keep it the unit test correct
	PhysicsComponent physics;
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	physics.SetPhysicsBody(world.CreateBody(&bodyDef));
	physics.GetPhysicsBody()->SetAngularDamping(3);

	b2Vec2 vertices[8];
	vertices[0].Set(1.5f, 0.f);
	vertices[1].Set(3.f, 2.5f);
	vertices[2].Set(2.8f, 5.5f);
	vertices[3].Set(1.f, 10.f);
	vertices[4].Set(-1.f, 10.f);
	vertices[5].Set(-2.8f, 5.5f);
	vertices[6].Set(-3.f, 2.5f);
	vertices[7].Set(-1.5f, 0.f);

	b2PolygonShape polygonShape;
	polygonShape.Set(vertices, 8);
	b2FixtureDef fixture;
	fixture.shape = &polygonShape;
	fixture.density = 0.1f;
	physics.GetPhysicsBody()->CreateFixture(&fixture);

	EXPECT_EQ(physics.GetLateralVelocityGLM(), glm::vec2(5,5));
	
}*/

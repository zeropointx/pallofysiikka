#pragma once
#include "Ball.h"
#include "SFML\Graphics.hpp"
enum Direction
{
	NOHIT,
	LEFT,
	RIGHT,
	UP,
	DOWN

};
class Collision
{
	
public:
	static void Initialize(float width, float height);
	static float Collides(Ball *ballA, Ball *ballB,float deltatime);
	static int CollidesTerrain(sf::CircleShape *shape);
	static float calcPositiveAngle(float angle);
	static void calcSpeedAfterCollision(Ball *ballA, Ball *ballB, float deltaTime);
	static float _restitution;
private:
	static float Distance(const sf::Vector2f *from, const sf::Vector2f *to);
	static float _width;
	static float _height;

	static float _friction;
};


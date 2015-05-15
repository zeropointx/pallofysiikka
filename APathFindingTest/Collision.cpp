#include "Collision.h"
#include <iostream>
#define PI 3.14159265358
float Collision::_width = 0;
float Collision::_height= 0;
float Collision::_restitution = 0.4;
float Collision::_friction = 0.1;

//Check if balls collide, if they do returns angle of collision, else returns -1
float Collision::Collides(Ball *ballA, Ball *ballB,float deltatime)
{
	//Calculates speed 
	sf::CircleShape *shape = ballA->getShape();
	sf::CircleShape *collidesWith = ballB->getShape();
	float gravityY = (0.5 * 9.81  * pow(deltatime, 2));
	sf::Vector2f posAfterVel1 = shape->getPosition();
	posAfterVel1.y +=gravityY;
	posAfterVel1 += ballA->getVelocity();
	sf::Vector2f posAfterVel2 = collidesWith->getPosition();
	posAfterVel2.y += gravityY;
	posAfterVel2 += ballB->getVelocity();
	//Checks if balls collide
	if (shape->getRadius() + collidesWith->getRadius() >
		Distance(&posAfterVel1 , &posAfterVel2))
	{
		//Calculates angle between them
		float deltaY = collidesWith->getPosition().y -shape->getPosition().y;
		float deltaX = collidesWith->getPosition().x - shape->getPosition().x;
		float angleInDegrees = atan2(deltaY, deltaX) * 180 / PI;
		return calcPositiveAngle(angleInDegrees);
	}
	else
		return -1.0f;
}
//sets screen width and height
void Collision::Initialize(float width, float height)
{
	_width = width;
	_height = height;
}
//returns distance of two balls
float Collision::Distance(const sf::Vector2f *from, const sf::Vector2f *to)
{
	return sqrt(pow(to->x - from->x, 2) + pow(to->y - from->y, 2));
}

//returns enum for terrain collision
int Collision::CollidesTerrain(sf::CircleShape *shape)
{
	if (shape->getPosition().x + shape->getRadius() > _width)
		return Direction::RIGHT;
	if (shape->getPosition().x - shape->getRadius() < 0)
		return Direction::LEFT;
	if (shape->getPosition().y + shape->getRadius() > _height)
		return Direction::DOWN;
	if (shape->getPosition().y - shape->getRadius() < 0)
		return Direction::UP;
	else
		return Direction::NOHIT;

}
//returns always positive angle (0- 360)
float Collision::calcPositiveAngle(float angle)
{
	if (angle > 0.0f)
		return angle;
	else
	{
		return 360.0f + angle;
	}
}
//Calculates speed after two balls collide
void Collision::calcSpeedAfterCollision(Ball *ballA, Ball *ballB, float deltaTime)
{

	float Angle = Collides(ballA, ballB,deltaTime);
	
	float NormalSpeedBefore1 = ballA->getVelocity().x * cos((Angle * 180 / PI)) + ballA->getVelocity().y * sin((Angle * 180 / PI));
	float NormalSpeedBefore2 = ballB->getVelocity().x * cos((Angle * 180 / PI)) + ballB->getVelocity().y * sin((Angle * 180 / PI));

	float TangentSpeedBefore1 = ballA->getVelocity().x * -sin((Angle * 180 / PI)) + ballA->getVelocity().y * cos((Angle * 180 / PI));
	float TangentSpeedBefore2 = ballB->getVelocity().x * -sin((Angle * 180 / PI)) + ballB->getVelocity().y * cos((Angle * 180 / PI));

	float NormalSpeedAfter1 = ((ballA->getMass() - _restitution * ballB->getMass()) / (ballA->getMass() + ballB->getMass()))*NormalSpeedBefore1 + 
		  ((1 + _restitution) * ballB->getMass()) / (ballA->getMass() + ballB->getMass()) * NormalSpeedBefore2;
	 
	float NormalSpeedAfter2 = (((1 - _restitution)*ballA->getMass()) / (ballA->getMass() + ballB->getMass())) * NormalSpeedBefore1 
		+ ((ballB->getMass() - _restitution*ballA->getMass()) / (ballA->getMass() - ballB->getMass())) *NormalSpeedBefore2;


	float impulse = (ballA->getMass() * NormalSpeedAfter1 - ballA->getMass() * NormalSpeedBefore1) * _friction;

	float SpeedX1 = NormalSpeedAfter1 * cos((Angle * 180 / PI) + TangentSpeedBefore1 * (-sin(Angle * 180 / PI)));
	float SpeedX2 = NormalSpeedAfter2 * cos((Angle * 180 / PI) + TangentSpeedBefore2 * (-sin(Angle * 180 / PI)));


	float SpeedY1 = NormalSpeedAfter1 * sin((Angle * 180 / PI) + TangentSpeedBefore1 * cos((Angle * 180 / PI)));
	float SpeedY2 = NormalSpeedAfter2 * sin((Angle * 180 / PI) + TangentSpeedBefore2 * cos((Angle * 180 / PI)));
	sf::Vector2f speedbefore1 = ballA->getVelocity();
	sf::Vector2f speedbefore2 = ballB->getVelocity();
	if ((SpeedX1 + SpeedY1) > 7.0f)
	{
		SpeedX1 /= 10;
		SpeedY1 /= 10;
	}
	if ((SpeedX2 + SpeedY2) > 7.0f)
	{
		SpeedX2 /= 10;
		SpeedY2 /= 10;
	}
	sf::Vector2f objectA(SpeedX1, SpeedY1);
	sf::Vector2f objectB(SpeedX2, SpeedY2);


	ballA->setVelocity(objectA);

	ballB->setVelocity(objectB);
} 
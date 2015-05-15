#include "Ball.h"
#include <iostream>
#define acceleration 9.81f

Ball::Ball(float radius)
{
	_circleShape = sf::CircleShape(radius);
	_circleShape.setFillColor(sf::Color(100, 100, 100));
	_circleShape.setOrigin(sf::Vector2f(radius , radius));
	_velocity = sf::Vector2f(0.0f, 0.0f);
	_mass = radius;
}
//Updates ball's position by adding velocity and gravity
void Ball::update(float deltatime)
{
	sf::Vector2f newPosition;
	newPosition.x = _circleShape.getPosition().x+_velocity.x*deltatime;

	float gravityY= ( 0.5* acceleration  * pow(deltatime, 2));
	newPosition.y = _circleShape.getPosition().y + gravityY;
	newPosition += _velocity;
	_circleShape.setPosition(newPosition);
	_velocity.y += acceleration * deltatime;

}


Ball::~Ball()
{
}
void Ball::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(_circleShape, states);
}
void Ball::setVelocity(sf::Vector2f velocity)
{
	_velocity = velocity;
}
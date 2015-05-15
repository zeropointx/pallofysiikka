#pragma once
#include <SFML\Graphics.hpp>
class Ball : public sf::Drawable
{
public:
	sf::CircleShape _circleShape;
	Ball(float radius);
	~Ball();
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	void update(float deltatime);
	sf::CircleShape* getShape(){ return &_circleShape; }
	void setVelocity(sf::Vector2f velocity);
	sf::Vector2f getVelocity(){ return _velocity; }
	float getMass(){ return _mass; }
private:
	float _mass;
	sf::Vector2f _velocity;
};


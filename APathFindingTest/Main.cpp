#include <SFML\Graphics.hpp>
#include <SFML\System.hpp>
#include "Ball.h"
#include "Collision.h"
#include <iostream>
#include <vector>
#define PI 3.1459265358
bool wasCollided(Ball *a, Ball *b);
struct CollisionPair
{
	Ball *a;
	Ball *b;
};
//List of collided balls (so they don't collide more than once per frame)
std::vector<CollisionPair> collided;
int main()
{
	float width = 800;
	float height = 600;

	std::vector<Ball*> balls;
	sf::Clock deltaClock;
	sf::RenderWindow window(sf::VideoMode(width, height), "SFML works!");
	
	sf::Sprite *background = new sf::Sprite();
	sf::Texture cancer = sf::Texture();
	cancer.loadFromFile("dogedream.png");
	background->setTexture(cancer);
	background->setScale(width / cancer.getSize().x, height / cancer.getSize().y);
	sf::Texture *doge= new sf::Texture();
	doge->loadFromFile("doge.png");

	
	/*
	Creating balls
	*/
		for (int j = 0; j < 2; j++)
		{
			Ball *tempBall = new Ball(30 + 1 * j);
			tempBall->getShape()->setTexture(doge);
			tempBall->getShape()->setPosition(50 + j * 40, 50 + j * 40);
			tempBall->getShape()->setFillColor(sf::Color::White);
			balls.push_back(tempBall);
		}
	


		//Initializing
	Collision::Initialize(window.getSize().x, window.getSize().y);
	sf::Time dt = deltaClock.restart();
	window.setFramerateLimit(60);

	//Begin update
	while (window.isOpen())
	{
		dt = deltaClock.restart();
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}


		//Collision check for ball <-> ball
		for (int i = 0; i < balls.size(); i++)
		{
			for (int j = 0; j < balls.size(); j++)
			{
				if (balls[i] == balls[j])
					continue; 
				//Check if balls collide, if they do returns angle of collision, else returns -1
				if (Collision::Collides(balls[i], balls[j],dt.asSeconds()) != -1.0f)
				{
					//If ball already collided don't collide again
					if (wasCollided(balls[i], balls[j]))
						continue;
					//Add new speed after collision
					Collision::calcSpeedAfterCollision(balls[i], balls[j],dt.asSeconds());

					//Add collision to list so it won't collide again this frame
					CollisionPair tempCollision= CollisionPair();
					tempCollision.a = balls[i];
					tempCollision.b = balls[j];
					collided.push_back(tempCollision);
				}
			}
		}
		//clear list of collided balls
		collided.clear();
		
		//Left mouse resets first ball
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			for (int i = 0; i < 1; i++)
			{
				sf::Vector2f pos = (sf::Vector2f)sf::Mouse::getPosition(window);
				pos.x += i * 100;
				balls[i]->getShape()->setPosition(pos);
				balls[i]->setVelocity(sf::Vector2f(0.0f, 0.0f));
			}

		}
		//Quit window with Esc
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
			break;
		//Update balls
		for (int i = 0; i < balls.size(); i++)
		{
			balls[i]->update(dt.asSeconds());
		}

		//Terrain collision
		for (int i = 0; i < balls.size(); i++)
		{
			if (Collision::CollidesTerrain(balls[i]->getShape()) != Direction::NOHIT)
			{
				sf::Vector2f newVelocity;
				switch (Collision::CollidesTerrain(balls[i]->getShape()))
				{
				case Direction::DOWN:
					newVelocity = sf::Vector2f(balls[i]->getVelocity().x, -balls[i]->getVelocity().y);
					balls[i]->getShape()->setPosition(sf::Vector2f(balls[i]->getShape()->getPosition().x, 600 - balls[i]->getShape()->getRadius()));
					break;
				case Direction::UP:
					newVelocity = sf::Vector2f(balls[i]->getVelocity().x, -balls[i]->getVelocity().y);
					balls[i]->getShape()->setPosition(sf::Vector2f(balls[i]->getShape()->getPosition().x, 0 + balls[i]->getShape()->getRadius()));
					break;
				case Direction::LEFT:
					newVelocity = sf::Vector2f(-balls[i]->getVelocity().x, balls[i]->getVelocity().y);
					balls[i]->getShape()->setPosition(sf::Vector2f(0 + balls[i]->getShape()->getRadius(), balls[i]->getShape()->getPosition().y));
					break;
				case Direction::RIGHT:
					newVelocity = sf::Vector2f(-balls[i]->getVelocity().x, balls[i]->getVelocity().y);
					balls[i]->getShape()->setPosition(sf::Vector2f(800 - balls[i]->getShape()->getRadius(), balls[i]->getShape()->getPosition().y));
					break;
				}
				balls[i]->setVelocity(newVelocity*Collision::_restitution);

			}
		}
		//clear window and draw background
		window.clear(sf::Color(50,150,50));
		window.draw(*background);

		//Draw balls
		for (int i = 0; i < balls.size(); i++)
		{
			window.draw(*balls[i]);
		}
		window.display();


	}

	return 0;
}
/*
Returns true if ball was collided this frame
*/
bool wasCollided(Ball *a, Ball *b)
{
	for (int i = 0; i < collided.size(); i++)
	{
		if (collided[i].a == a && collided[i].b == b || collided[i].a == b && collided[i].b == a)
			return true;
	}
	return false;
}
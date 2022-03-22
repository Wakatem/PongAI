#pragma once
#include <SFML/Graphics.hpp>


class Bat
{
private:
	sf::Vector2f m_position;
	sf::RectangleShape m_Shape;
	float m_Speed = 2900.0f;
	bool m_MovingRight = false;
	bool m_MovingLeft = false;
	unsigned int startingX;
	unsigned int endingX;
	bool agentPlaying = false;
	bool positionUpdated = false;
public:
	Bat();
	Bat(std::string name);
	Bat(float startX, float startY);
	sf::FloatRect getPosition();
	sf::RectangleShape getShape();
	void moveLeft();
	void moveRight();
	void moveTo(int startingX);
	void stopLeft();
	void stopRight();
	void update(sf::Time dt, sf::RenderWindow& window, sf::Color color);
	void resetBat(float startX);
	float getSpeed();
};

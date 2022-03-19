
#include "..\headers\Bat.h"


Bat::Bat(){}

Bat::Bat(std::string name) {
	
}


Bat::Bat(float startX, float startY) {
	m_position.x = startX;
	m_position.y = startY;
	m_Shape.setSize(sf::Vector2f(150, 5));
	m_Shape.setPosition(m_position);
	startingX = startX;
}

sf::FloatRect Bat::getPosition()
{
	return m_Shape.getGlobalBounds();
}

sf::RectangleShape Bat::getShape()
{
	return m_Shape;
}

float Bat::getSpeed()
{
	return m_Speed;
}

void Bat::moveLeft()
{
	m_MovingLeft = true;
}

void Bat::stopLeft()
{
	m_MovingLeft = false;
}

void Bat::moveRight()
{
	m_MovingRight = true;
}

void Bat::stopRight()
{
	m_MovingRight = false;
}

void Bat::update(sf::Time dt, sf::RenderWindow& window, sf::Color color)
{
	m_Shape.setFillColor(color);
	int startingPoint = (int)this->getPosition().left;
	int endingPoint = (int)this->getPosition().left + (int)this->getPosition().width;


	
	if (startingPoint == startingX)
		positionUpdated = true;

	else{
		//go to right
		if (startingPoint < startingX)
		{
			calcPos = m_position.x + m_Speed * dt.asSeconds();
			if (calcPos + this->getPosition().width < window.getSize().x)
			{
				m_position.x = calcPos;
			}
		}

		//go to left
		if (startingX < startingPoint)
		{
			calcPos = m_position.x - m_Speed * dt.asSeconds();
			if (calcPos > 0)
			{
				m_position.x = calcPos;
			}
		}


	}

	m_Shape.setPosition(m_position);

}



void Bat::moveTo(int starting_x)
{

	startingX = starting_x;
	this->positionUpdated = false;
	
}

void Bat::resetBat(float startX)
{
	m_position.x = startX;
}

#include "..\headers\Bat.h"


Bat::Bat(float startX, float startY) {
	m_position.x = startX;
	m_position.y = startY;
	m_Shape.setSize(sf::Vector2f(100, 5));
	m_Shape.setPosition(m_position);
}

sf::FloatRect Bat::getPosition()
{
	return m_Shape.getGlobalBounds();
}

sf::RectangleShape Bat::getShape()
{
	return m_Shape;
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

	if (m_MovingLeft)
	{

		calcPos = m_position.x - m_Speed * dt.asSeconds();
		if (calcPos > 0)
		{
			m_position.x = calcPos;
		}

	}

	if (m_MovingRight)
	{
		calcPos = m_position.x + m_Speed * dt.asSeconds();
		if (calcPos + this->getPosition().width < window.getSize().x)
		{
			m_position.x = calcPos;
		}
	}

	m_Shape.setPosition(m_position);

}
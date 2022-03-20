
#include "..\headers\Ball.h"
#include <iostream>
using sf::Music;


Ball::Ball(){}

Ball::Ball(float startX, float startY) {
	c_position.x = startX;
	c_position.y = startY;
	circle.setRadius(3.0f);
	circle.setPointCount(2000);
	circle.setOutlineThickness(5);
	circle.setPosition(c_position);
	soundEffect = new Music();
	soundEffect->openFromFile(".\\resources\\hitSound.wav");
	soundEffect->setVolume(0);
}

FloatRect Ball::getPosition()
{
	return circle.getGlobalBounds();
}

CircleShape Ball::getShape()
{
	return circle;
}



void Ball::checkSides(Time dt, RenderWindow& window, Bat bat)
{
	//left side
	if ((this->getPosition().left + this->getPosition().width) < 0)
	{
		cameFromLeft = true; //ball hit left wall and will rebound
		toLeft = false;
		soundEffect->stop();
		soundEffect->play();
	}

	//right side
	else if ((this->getPosition().left + this->getPosition().width) > window.getSize().x)
	{
		cameFromLeft = false;
		toLeft = true;
		soundEffect->stop();
		soundEffect->play();
	}


	//top side
	if ((this->getPosition().top + this->getPosition().height) < 0)
	{
		cameFromTop = true;
		toUp = false;
		soundEffect->stop();
		soundEffect->play();
	}

	//Bat and Ball intersection
	else if (this->getPosition().intersects(bat.getPosition()) == true)
	{
		collisionStatus = 1;

	}

	//bottom side
	else if ((this->getPosition().top + this->getPosition().height) > window.getSize().y)
	{
		cameFromTop = false;
		toUp = true;
		soundEffect->stop();
		soundEffect->play();
		collisionStatus = -1;
	}




}



void Ball::move(Time dt, RenderWindow& window)
{
	//check if the ball hit the bat, fromwhere, and rebound it appropiately
	if (collisionStatus == 1)
	{
		//stop function is used to reset the music player position 
		soundEffect->stop();
		soundEffect->play();
		if (cameFromTop)
		{
			//rebound the ball to top if it hit the bat from the top
			toUp = true;
		}
		else
		{
			//rebound the ball to bottom if it hit the bat from the bottom
			toUp = false;
		}

		if (cameFromLeft)
		{
			toLeft = false;
		}
		else
		{
			//it came from right, so move left
			toLeft = true;
		}
	}

	if (toLeft)
	{
		c_position.x -= m_Speed * dt.asSeconds();
	}
	else
	{
		//toRight
		c_position.x += m_Speed * dt.asSeconds();
	}



	if (toUp)
	{
		c_position.y -= m_Speed * dt.asSeconds();
	}
	else
	{
		//toBottom
		c_position.y += m_Speed * dt.asSeconds();
	}



}



int Ball::update(Time dt, RenderWindow& window, Bat bat)
{

	checkSides(dt, window, bat);
	move(dt, window);
	circle.setPosition(c_position);

	// if ball hits the bottom wall
	if (collisionStatus == -1)
	{
		//reset
		collisionStatus = 0;
		return -1;
	}

	//if the ball hit the bat
	else if (collisionStatus == 1)
	{
		collisionStatus = 0;
		return 1;
	}

	return 0;
}


bool Ball::isBalltoUp()
{
	return toUp;
}
void Ball::resetBall(float startX, float startY)
{
	c_position.x = startX;
	c_position.y = startY;
	circle.setPosition(c_position);
	toLeft = true;
	toUp = false;
	cameFromLeft = false;
	cameFromTop = true;
	collisionStatus = 0;
}
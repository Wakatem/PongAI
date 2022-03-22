#pragma once
#include <SFML/Graphics.hpp>
#include "Bat.h"
#include <SFML/Audio/Music.hpp>

using namespace sf;
class Ball
{
private:
	Vector2f c_position;
	CircleShape circle;
	float m_Speed = 500.0f;
	bool toLeft = true;
	bool toUp = false;
	bool cameFromLeft = false;
	bool cameFromTop = true;
	short collisionStatus = 0;
public:
	Ball();
	Ball(float startX, float startY);
	FloatRect getPosition();
	CircleShape getShape();
	void checkSides(Time dt, RenderWindow& window, Bat bat); //check all four walls, including the bat
	void move(Time dt, RenderWindow& window);
	int update(Time dt, RenderWindow& window, Bat bat);
	bool isBalltoUp();
	bool isBalltoLeft();
	void resetBall(float startX, float startY);
	Music* soundEffect;
};

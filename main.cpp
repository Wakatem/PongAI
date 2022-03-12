
#include "Bat.h"
#include "Ball.h"
#include <sstream>
#include <cstdlib>
#include <string>
#include <windows.h>
#include <SFML/Graphics.hpp>
#include <SFML/Audio/Music.hpp>
#include <thread>
#include <chrono>
#include <vector>
#include <iostream>

using std::thread;
using std::string;
using std::vector;



int score;
bool addScore;
int lives;
bool gameover = false;
Text scoreHud, livesHud, controls, target, gameStatus, gameoverText;
Font font;
string gameStatuscontent;

void updateGameValues(RenderWindow& window, Clock& clock, Bat& bat, Ball& ball);
void updateGameWindow(RenderWindow& window, Bat& bat, Ball& ball, int& scoreTarget);




Color updateBatColor(int& score)
{

	if (score >= 1000 && score < 2000)
	{
		return Color::Green;
	}

	else if (score >= 2000 && score < 3000)
	{
		return Color::Blue;
	}

	else if (score >= 3000 && score < 4000)
	{
		return Color::Yellow;
	}

	else if (score >= 4000 && score < 5000)
	{
		return Color::Magenta;
	}

	else if (score >= 5000 && score < 6000)
	{
		return Color::Cyan;
	}

	else if (score >= 6000 && score < 7000)
	{
		return Color::Red;
	}

	else
	{
		return Color::White;
	}

}


void setScreenText(RenderWindow& window, int& scoreTarget)
{

	controls.setFont(font);
	controls.setCharacterSize(45);
	controls.setFillColor(Color::White);
	controls.setPosition(25, 3);
	controls.setString("'esc' :  Quit                       |      'R' :  Restart");

	target.setFont(font);
	target.setCharacterSize(45);
	target.setFillColor(Color::White);
	target.setPosition(window.getSize().x - 150, 3);
	target.setString("target  :  " + std::to_string(scoreTarget));

	scoreHud.setFont(font);
	scoreHud.setCharacterSize(75);
	scoreHud.setFillColor(Color::White);
	scoreHud.setPosition(25, 10);
	scoreHud.setString("Score: " + std::to_string(score));

	livesHud.setFont(font);
	livesHud.setCharacterSize(75);
	livesHud.setFillColor(Color::White);
	livesHud.setPosition(165, 10);
	livesHud.setString("     |   Lives: " + std::to_string(lives));
}



int main()
{
	vector<VideoMode> modes = VideoMode::getFullscreenModes();
	int i = 0;
	while ( (float)modes[i].width / modes[i].height != (float)16 / 9)	i++;

	VideoMode vm = vm.getFullscreenModes()[i];
	RenderWindow window(vm, "Pong Game", Style::Fullscreen);

	Clock clock;
	Music bgMusic;
	int scoreTarget = 10000;
	bgMusic.openFromFile("background_music.wav");
	bgMusic.setLoop(true);
	bgMusic.setVolume(30);
	bgMusic.play();


	//create and initialize
	Bat bat(window.getSize().x / 2, window.getSize().y - 80);
	Ball ball(window.getSize().x / 2, 10);
	font.loadFromFile("game_over.ttf");
	lives = 5;
	score = 0;
	addScore = false;

	setScreenText(window, scoreTarget);

	thread updateValues_t(updateGameValues, std::ref(window), std::ref(clock), std::ref(bat), std::ref(ball));


	while (window.isOpen())
	{

		//handle player input
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window.close();

		}

		if (Keyboard::isKeyPressed(Keyboard::Escape))
			window.close();


		if (Keyboard::isKeyPressed(Keyboard::R))
		{
			//game restarts
			gameover = false;
			ball.resetBall(window.getSize().x / 2, 10);
			score = 0;
			lives = 5;
		}

		if (Keyboard::isKeyPressed(Keyboard::Left))    bat.moveLeft();
		else                                           bat.stopLeft();


		if (Keyboard::isKeyPressed(Keyboard::Right))   bat.moveRight();
		else                                           bat.stopRight();


		updateGameWindow(window, bat, ball, scoreTarget);
	}


	updateValues_t.join();

	return 0;
}



void updateGameValues(RenderWindow& window, Clock& clock, Bat& bat, Ball& ball)
{
	while (window.isOpen())
	{
		Time dt = clock.restart();


		//update ball frames
		int value = ball.update(dt, window, bat);

		if (gameover == false)
		{
			//update bat frames
			bat.update(dt, window, updateBatColor(score));

			//if ball hits bottom wall
			if (value == -1)
			{
				lives -= 1;
				if (score >= 250)
				{
					score -= 250;
				}
				else
				{
					score = 0;
				}
			}
			else if (value == 1)
			{
				addScore = true;
			}



			//in order to avoid increasing score up to thousands with a single hit, we must check that the ball hit the bat and [[[ no longer intersects with it ]]]
			if (addScore == true && ball.getPosition().intersects(bat.getPosition()) == false)
			{
				score += 430;
				addScore = false;

			}
			std::stringstream scoreStream;
			scoreStream << "Score: " << score;
			scoreHud.setString(scoreStream.str());

			std::stringstream livesStream;
			livesStream << "     |   Lives: " << lives;
			livesHud.setString(livesStream.str());
		}

	}


}


void updateGameWindow(RenderWindow& window, Bat& bat, Ball& ball, int& scoreTarget)
{
	window.clear();
	if (lives > 0 && score < scoreTarget)
	{
		//nothing
	}
	else
	{
		gameStatus.setFont(font);
		gameStatus.setCharacterSize(300);
		gameStatus.setFillColor(Color::White);
		gameStatus.setPosition((window.getSize().x / 3) + 70, (window.getSize().y / 5) - 100);

		gameoverText.setFont(font);
		gameoverText.setCharacterSize(70);
		gameoverText.setFillColor(Color::White);
		gameoverText.setString("GAME OVER");

		if (score >= scoreTarget)
		{
			gameStatuscontent = "You Won!";
			gameoverText.setPosition(gameStatus.getPosition().x + 185, gameStatus.getPosition().y + 300);
			gameover = true;
		}
		else
		{
			gameStatuscontent = "You Lost";
			//to properly align it
			gameoverText.setPosition(gameStatus.getPosition().x + 160, gameStatus.getPosition().y + 300);
			gameover = true;
		}

		std::stringstream gameStatusStream;
		gameStatusStream << gameStatuscontent;
		gameStatus.setString(gameStatusStream.str());

		window.draw(gameStatus);
		window.draw(gameoverText);

	}

	window.draw(target);
	window.draw(controls);
	window.draw(scoreHud);
	window.draw(livesHud);
	window.draw(bat.getShape());
	window.draw(ball.getShape());
	window.display();
}

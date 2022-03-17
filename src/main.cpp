#include "..\headers\Bat.h"
#include "..\headers\Ball.h"
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
#include <pybind11/embed.h>

using std::thread;
using std::string;
using std::vector;
namespace py = pybind11;


RenderWindow window;

bool addScore;

Text scoreHud, livesHud, controls, target, gameStatus, gameoverText;
Font font;
string gameStatuscontent;


struct PongDetails 
{
	int lives = 5;
	int score = 0;
	int scoreTarget = 10000;
	float windowX;
	float windowY;
	float batWidth;
	float ballPos;	//ball boundraies 
	bool gameover = false;

}pg;


PYBIND11_EMBEDDED_MODULE(PongGame, m) {
	py::class_<PongDetails>(m, "PongDetails")
		.def(py::init<>())
		.def_readonly_static("lives", &pg.lives)
		.def_readonly_static("score", &pg.score)
		.def_readonly_static("scoreTarget", &pg.scoreTarget)
		.def_readonly_static("windowWidth", &pg.windowX)
		.def_readonly_static("windowHeight", &pg.windowY)
		.def_readonly_static("batwidth", &pg.batWidth)
		.def_readonly_static("ballPos", &pg.ballPos)
		.def_readonly_static("gameover", &pg.gameover);

}

void updateGameValues(RenderWindow& window, Clock& clock, Bat& bat, Ball& ball);
void updateGameWindow(RenderWindow& window, Bat& bat, Ball& ball, int& scoreTarget);
Color updateBatColor(int& score);
void setScreenText(RenderWindow& window, int& scoreTarget);


int main()
{

	//initialize Python interpreter
	py::scoped_interpreter guard{};

	vector<VideoMode> modes = VideoMode::getFullscreenModes();
	int i = 0;
	while ( (float)modes[i].width / modes[i].height != (float)16 / 9)	i++;

	VideoMode vm = vm.getFullscreenModes()[i];
	window.create(vm, "Pong Game", Style::Default);


	Clock clock;
	Music bgMusic;
	bgMusic.openFromFile(".\\resources\\background_music.wav");
	bgMusic.setLoop(true);
	bgMusic.setVolume(0);
	bgMusic.play();

	Bat bat(window.getSize().x / 2 - 100, window.getSize().y - 300);
	Ball ball(window.getSize().x / 2 - 100, window.getSize().y - 300);


	//Update pong details
	pg.windowX = window.getSize().x;
	pg.windowY = window.getSize().y;
	pg.batWidth = bat.getPosition().width;
	
	font.loadFromFile(".\\resources\\game_over.ttf");
	addScore = false;


	setScreenText(window, pg.scoreTarget);
	thread updateValues_t(updateGameValues, std::ref(window), std::ref(clock), std::ref(bat), std::ref(ball));

	//run python script here
	py::module::import("ai");


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
			pg.gameover = false;
			ball.resetBall(window.getSize().x / 2, 10);
			bat.resetBat(window.getSize().x / 2 - 100);
			pg.lives = 5;
			pg.score = 0;
		}

		if (Keyboard::isKeyPressed(Keyboard::Left))    bat.moveLeft();
		else                                           bat.stopLeft();


		if (Keyboard::isKeyPressed(Keyboard::Right))   bat.moveRight();
		else                                           bat.stopRight();

		updateGameWindow(window, bat, ball, pg.scoreTarget);
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
		pg.ballPos = ball.getPosition().left + ball.getPosition().width;

		if (pg.gameover == false)
		{
			//update bat frames
			bat.update(dt, window, updateBatColor(pg.score));

			//if ball hits bottom wall
			if (value == -1)
			{
				pg.lives -= 1;
				if (pg.score >= 250)
				{
					pg.score -= 250;
				}
				else
				{
					pg.score = 0;
				}
			}
			else if (value == 1)
			{
				addScore = true;
			}



			//in order to avoid increasing score up to thousands with a single hit, we must check that the ball hit the bat and [[[ no longer intersects with it ]]]
			if (addScore == true && ball.getPosition().intersects(bat.getPosition()) == false)
			{
				pg.score += 430;
				addScore = false;

			}
			std::stringstream scoreStream;
			scoreStream << "Score: " << pg.score;
			scoreHud.setString(scoreStream.str());

			std::stringstream livesStream;
			livesStream << "     |   Lives: " << pg.lives;
			livesHud.setString(livesStream.str());
		}

	}


}

void updateGameWindow(RenderWindow& window, Bat& bat, Ball& ball, int& scoreTarget)
{
	window.clear();
	if (pg.lives > 0 && pg.score < scoreTarget)
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

		if (pg.score >= scoreTarget)
		{
			gameStatuscontent = "You Won!";
			gameoverText.setPosition(gameStatus.getPosition().x + 185, gameStatus.getPosition().y + 300);
			pg.gameover = true;
		}
		else
		{
			gameStatuscontent = "You Lost";
			//to properly align it
			gameoverText.setPosition(gameStatus.getPosition().x + 160, gameStatus.getPosition().y + 300);
			pg.gameover = true;
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
	scoreHud.setString("Score: " + std::to_string(pg.score));

	livesHud.setFont(font);
	livesHud.setCharacterSize(75);
	livesHud.setFillColor(Color::White);
	livesHud.setPosition(165, 10);
	livesHud.setString("     |   Lives: " + std::to_string(pg.lives));
}

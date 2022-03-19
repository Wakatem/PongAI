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

Bat* bat;
Ball* ball;
RenderWindow window;



struct PongScreen
{
	Text scoreHud, livesHud, controls, target, gameStatus, gameoverText;
	Font font;
	string gameStatuscontent;
}ps;



struct PongDetails 
{
	int lives = 5;
	int score = 0;
	int scoreTarget = 10000;
	bool addScore;
	bool loseLive;
	float windowX;
	float windowY;
	float batWidth;
	float batY;					//y value of bat
	float ballPos_vertical;		//ball boundraies (for vertical comparison)
	float ballPos_horizontal;	//ball boundraies (for horizontal comparison)
	bool ball_toUp = false;
	bool gameover = false;

}pg;


void move(int startingX)
{
	bat->moveTo(startingX);
}

void resetGame()
{
	//game restarts
	pg.gameover = false;
	ball->resetBall(window.getSize().x / 2 / 2, 10);
	bat->resetBat(window.getSize().x / 2 - 100);
	pg.lives = 5;
	pg.score = 0;
}


PYBIND11_EMBEDDED_MODULE(PongGame, m) {
	py::class_<PongDetails>(m, "PongDetails")
		.def(py::init<>())
		.def_readonly_static("lives", &pg.lives)
		.def_readonly_static("score", &pg.score)
		.def_readonly_static("scoreTarget", &pg.scoreTarget)
		.def_readonly_static("windowWidth", &pg.windowX)
		.def_readonly_static("windowHeight", &pg.windowY)
		.def_readonly_static("batWidth", &pg.batWidth)
		.def_readonly_static("batY", &pg.batY)
		.def_readonly_static("ballPos_vertical", &pg.ballPos_vertical)
		.def_readonly_static("ballPos_horizontal", &pg.ballPos_horizontal)
		.def_readonly_static("ball_toUp", &pg.ball_toUp)
		.def_readonly_static("gameover", &pg.gameover);

	m.def("move", &move);
	m.def("resetGame", &resetGame);

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
	ps.controls.setFont(ps.font);
	ps.controls.setCharacterSize(45);
	ps.controls.setFillColor(Color::White);
	ps.controls.setPosition(25, 3);
	ps.controls.setString("'esc' :  Quit                       |      'R' :  Restart");

	ps.target.setFont(ps.font);
	ps.target.setCharacterSize(45);
	ps.target.setFillColor(Color::White);
	ps.target.setPosition(window.getSize().x - 150, 3);
	ps.target.setString("target  :  " + std::to_string(scoreTarget));

	ps.scoreHud.setFont(ps.font);
	ps.scoreHud.setCharacterSize(75);
	ps.scoreHud.setFillColor(Color::White);
	ps.scoreHud.setPosition(25, 10);
	ps.scoreHud.setString("Score: " + std::to_string(pg.score));

	ps.livesHud.setFont(ps.font);
	ps.livesHud.setCharacterSize(75);
	ps.livesHud.setFillColor(Color::White);
	ps.livesHud.setPosition(165, 10);
	ps.livesHud.setString("     |   Lives: " + std::to_string(pg.lives));
}


void showGameState()
{


	ps.gameStatus.setFont(ps.font);
	ps.gameStatus.setCharacterSize(300);
	ps.gameStatus.setFillColor(Color::White);
	ps.gameStatus.setPosition((window.getSize().x / 3) + 70, (window.getSize().y / 5) - 100);

	ps.gameoverText.setFont(ps.font);
	ps.gameoverText.setCharacterSize(70);
	ps.gameoverText.setFillColor(Color::White);
	ps.gameoverText.setString("GAME OVER");

	if (pg.score >= pg.scoreTarget)
	{
		ps.gameStatuscontent = "You Won!";
		ps.gameoverText.setPosition(ps.gameStatus.getPosition().x + 185, ps.gameStatus.getPosition().y + 300);
		pg.gameover = true;
	}
	else
	{
		ps.gameStatuscontent = "You Lost";
		//to properly align it
		ps.gameoverText.setPosition(ps.gameStatus.getPosition().x + 160, ps.gameStatus.getPosition().y + 300);
		pg.gameover = true;
	}

	std::stringstream gameStatusStream;
	gameStatusStream << ps.gameStatuscontent;
	ps.gameStatus.setString(gameStatusStream.str());



}



void updateGameValues(RenderWindow& window, Clock& clock, Bat& bat, Ball& ball)
{

	Time dt = clock.restart();

	//update ball frames
	int value = ball.update(dt, window, bat);
	pg.ballPos_horizontal = ball.getPosition().left + ball.getPosition().width;
	pg.ballPos_vertical = ball.getPosition().top + ball.getPosition().height;
	pg.ball_toUp = ball.isBalltoUp();

	if (pg.gameover == false)
	{
		//update bat frames
		bat.update(dt, window, updateBatColor(pg.score));

		//if ball hits bottom wall
		if (value == -1)
			pg.loseLive = true;
		else if (value == 1)
			pg.addScore = true;


		if (pg.loseLive == true && pg.ballPos_vertical < pg.windowY)
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

			pg.loseLive = false;
		}


		//in order to avoid increasing score up to thousands with a single hit, we must check that the ball hit the bat and [[[ no longer intersects with it ]]]
		if (pg.addScore == true && ball.getPosition().intersects(bat.getPosition()) == false)
		{
			pg.score += 430;
			pg.addScore = false;

		}
		std::stringstream scoreStream;
		scoreStream << "Score: " << pg.score;
		ps.scoreHud.setString(scoreStream.str());

		std::stringstream livesStream;
		livesStream << "     |   Lives: " << pg.lives;
		ps.livesHud.setString(livesStream.str());
	}




}


void updateGameValues(RenderWindow& window, Clock& clock, Bat& bat, Ball& ball);
void showGameState();
Color updateBatColor(int& score);
void setScreenText(RenderWindow& window, int& scoreTarget);
void runScript();


int main()
{
	
	vector<VideoMode> modes = VideoMode::getFullscreenModes();
	int i = 0;
	while ( (float)modes[i].width / modes[i].height != (float)16 / 9)	i++;

	VideoMode vm = vm.getFullscreenModes()[i];
	window.create(vm, "Pong Game", Style::None);
	//window.setFramerateLimit(60);

	Clock clock;
	Music bgMusic;
	bgMusic.openFromFile(".\\resources\\background_music.wav");
	bgMusic.setLoop(true);
	bgMusic.setVolume(0);
	bgMusic.play();

	bat = new Bat(window.getSize().x / 2 - 100, window.getSize().y - 300);
	ball = new Ball(window.getSize().x / 2 - 100, window.getSize().y - 300);


	//Update pong details
	pg.windowX = window.getSize().x;
	pg.windowY = window.getSize().y;
	pg.batWidth = bat->getPosition().width;
	pg.batY = bat->getPosition().top;

	ps.font.loadFromFile(".\\resources\\game_over.ttf");
	pg.addScore = false;
	pg.loseLive = false;
	setScreenText(window, pg.scoreTarget);


	//run python script here
	//thread runscript_t(runScript);


	while (window.isOpen())
	{
		window.clear();

		//handle player input
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window.close();

		}

		if (Keyboard::isKeyPressed(Keyboard::Escape))
		{
			pg.gameover = true;
			window.close();
		}

		if (Keyboard::isKeyPressed(Keyboard::R))
		{
			resetGame();
		}


		updateGameValues(window, clock, *bat, *ball);
		
		//when game ends
		if (pg.lives == 0 || pg.score > pg.scoreTarget)
		{
			showGameState();
			window.draw(ps.gameStatus);
			window.draw(ps.gameoverText);
		}


		window.draw(ps.target);
		window.draw(ps.controls);
		window.draw(ps.scoreHud);
		window.draw(ps.livesHud);
		window.draw(bat->getShape());
		window.draw(ball->getShape());
		window.display();
	}

	//runscript_t.join();

	delete bat;
	delete ball;
	return 0;
}


void runScript()
{
	//initialize Python interpreter
	py::scoped_interpreter guard{};
	py::module::import("Agent");
}






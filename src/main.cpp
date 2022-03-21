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

bool scriptRunning = false;
bool scriptStopped = true;
bool gameStarted = false;



struct PongScreen
{
	Text scoreHud, livesHud, controls, target, gameStatus, gameoverText, episodesHud, strategyHud, strategyResultHud, explorationRateHud;
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
	int ballPos_vertical;		//ball boundraies (for vertical comparison)
	int ballPos_horizontal;		//ball boundraies (for horizontal comparison)
	bool ball_toUp = false;
	bool ball_toLeft = true;
	bool ballIsBackUp;			
	bool gameover = false;
	int episodes;
	string strategy = "[Agent not online]";
	string strategyResult;
	float er;
	bool closeGame = false;
}pg;


void move(int startingX)
{
	bat->moveTo(startingX);
}

void updateIfBallIsBackUp(bool ballIsUp)
{
	pg.ballIsBackUp = ballIsUp;
}

void updateEpisodes(int ep, float er)
{
	pg.episodes = ep;
	std::stringstream epStream;
	epStream << "     |       Episodes:   " << pg.episodes;
	ps.episodesHud.setString(epStream.str());

	pg.er = er;
	std::stringstream ss;
	ss << "     |   Exploration Rate:   " + std::to_string(pg.er);
	ps.explorationRateHud.setString(ss.str());
}

void updateActionStrategy(string strategy)
{
	pg.strategy = strategy;
	std::stringstream stratStream;
	stratStream << "     |   Strategy:    " << pg.strategy;
	ps.strategyHud.setString(stratStream.str());
 
}

void updateReward(string action, int q)
{
	pg.strategyResult = "(" + action + ", " + std::to_string(q) + ")";
	std::stringstream ss;
	ss << "     |   Action,Reward:   " << pg.strategyResult;
	ps.strategyResultHud.setString(ss.str());
}

void resetGame()
{
	//game restarts
	pg.gameover = false;
	gameStarted = true;
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
		.def_readonly_static("ballIsBackUp", &pg.ballIsBackUp)
		.def_readonly_static("ball_toUp", &pg.ball_toUp)
		.def_readonly_static("ball_toLeft", &pg.ball_toLeft)
		.def_readonly_static("gameover", &pg.gameover)
		.def_readonly_static("closeGame", &pg.closeGame);

	m.def("move", &move);
	m.def("updateEpisodes", &updateEpisodes);
	m.def("updateActionStrategy", &updateActionStrategy);
	m.def("updateIfBallIsBackUp", &updateIfBallIsBackUp);
	m.def("updateReward", &updateReward);
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


void setScreenText(RenderWindow& window)
{
	ps.controls.setFont(ps.font);
	ps.controls.setCharacterSize(45);
	ps.controls.setFillColor(Color::White);
	ps.controls.setPosition(25, 3);
	ps.controls.setString("'esc' :  Quit                       |      'R' :  Restart                                                                          |      'P' : Wake Agent Up");

	ps.target.setFont(ps.font);
	ps.target.setCharacterSize(45);
	ps.target.setFillColor(Color::White);
	ps.target.setPosition(window.getSize().x - 150, 3);
	ps.target.setString("target  :  " + std::to_string(pg.scoreTarget));

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

	ps.episodesHud.setFont(ps.font);
	ps.episodesHud.setCharacterSize(75);
	ps.episodesHud.setFillColor(Color::White);
	ps.episodesHud.setPosition(315, 10);
	ps.episodesHud.setString("     |           Episodes:   " + std::to_string(pg.episodes));

	ps.strategyHud.setFont(ps.font);
	ps.strategyHud.setCharacterSize(75);
	ps.strategyHud.setFillColor(Color::White);
	ps.strategyHud.setPosition(585, 10);
	ps.strategyHud.setString("     |   Strategy:   " + pg.strategy);

	ps.strategyResultHud.setFont(ps.font);
	ps.strategyResultHud.setCharacterSize(75);
	ps.strategyResultHud.setFillColor(Color::White);
	ps.strategyResultHud.setPosition(1055, 10);
	ps.strategyResultHud.setString("     |   Action,Reward:        " + pg.strategyResult);

	ps.explorationRateHud.setFont(ps.font);
	ps.explorationRateHud.setCharacterSize(75);
	ps.explorationRateHud.setFillColor(Color::White);
	ps.explorationRateHud.setPosition(1435, 10);
	ps.explorationRateHud.setString("     |   Exploration Rate:   " + std::to_string(pg.er));
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




void updateGameValues(RenderWindow& window, Clock& clock, Bat& bat, Ball& ball);
void pythonThread();


int main()
{

	vector<VideoMode> modes = VideoMode::getFullscreenModes();
	int i = 0;
	while ( (float)modes[i].width / modes[i].height != (float)16 / 9)	i++;

	VideoMode vm = vm.getFullscreenModes()[i];
	window.create(vm, "Pong Game", Style::None | Style::Titlebar);
	//window.setFramerateLimit(60);

	Clock clock;
	Music bgMusic;
	bgMusic.openFromFile(".\\resources\\background_music.wav");
	bgMusic.setLoop(true);
	bgMusic.setVolume(25);
	bgMusic.play();

	ball = new Ball(window.getSize().x / 2 - 100, 10);
	bat = new Bat(window.getSize().x / 2 - 100, window.getSize().y - 300);


	//Update pong details
	pg.windowX = window.getSize().x;
	pg.windowY = window.getSize().y;
	pg.batWidth = bat->getPosition().width;
	pg.batY = bat->getPosition().top;

	ps.font.loadFromFile(".\\resources\\game_over.ttf");
	pg.addScore = false;
	pg.loseLive = false;
	setScreenText(window);


	//run python script here
	thread pythonThread_t(pythonThread);

	thread updateGameValues_t(updateGameValues, std::ref(window), std::ref(clock), std::ref(*bat), std::ref(*ball));


	while (window.isOpen())
	{
		window.clear();

		//handle player input
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
			{
				pg.gameover = true;
				pg.closeGame = true;
				window.close();
			}
		}

		if (Keyboard::isKeyPressed(Keyboard::Escape))
		{
			pg.gameover = true;
			pg.closeGame = true;
			window.close();
		}

		if (Keyboard::isKeyPressed(Keyboard::R))
		{
			resetGame();
		}

		if (Keyboard::isKeyPressed(Keyboard::P))
		{
			scriptRunning = true;
		}

		if (Keyboard::isKeyPressed(Keyboard::Left))    bat->moveLeft();
		else                                           bat->stopLeft();


		if (Keyboard::isKeyPressed(Keyboard::Right))   bat->moveRight();
		else                                           bat->stopRight();
		
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
		window.draw(ps.episodesHud);
		window.draw(ps.strategyHud);
		window.draw(ps.strategyResultHud);
		window.draw(ps.explorationRateHud);
		window.draw(bat->getShape());
		window.draw(ball->getShape());
		window.display();
	}

	pythonThread_t.join();
	updateGameValues_t.join();

	delete bat;
	delete ball;
	return 0;
}



void updateGameValues(RenderWindow& window, Clock& clock, Bat& bat, Ball& ball)
{

	while (window.isOpen())
	{
		
		Time dt = clock.restart();

		//update ball frames
		int value = ball.update(dt, window, bat);

		if (ball.getPosition().left + ball.getPosition().width < 0)
			pg.ballPos_horizontal = 0;

		else if (ball.getPosition().left + ball.getPosition().width > window.getSize().x)
			pg.ballPos_horizontal = (int)window.getSize().x;
		else
			pg.ballPos_horizontal = (int)(ball.getPosition().left + ball.getPosition().width);
		

		pg.ballPos_vertical = (int) (ball.getPosition().top + ball.getPosition().height);
		pg.ball_toUp = ball.isBalltoUp();
		pg.ball_toLeft = ball.isBalltoLeft();

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


}



void pythonThread()
{
	bool errorThrown = false;


	while(true)
	{
		if (!pg.closeGame)
		{
			if (scriptRunning)
			{
				pg.strategy = "{Starting...}";
				std::stringstream stratStream;
				stratStream << "     |   Strategy:    " << pg.strategy;
				ps.strategyHud.setString(stratStream.str());


				//run script
				py::scoped_interpreter guard{};
				//py::module::import("from scripts import Agent");
				py::exec("import sys");
				py::exec("sys.path.insert(0, './scripts')");
				py::exec("import Agent");
				scriptRunning = false;

			}
			else
			{
				pg.strategy = "[Agent not online]";
				std::stringstream stratStream;
				stratStream << "     |   Strategy:    " << pg.strategy;
				ps.strategyHud.setString(stratStream.str());
			}

		}

		else
			break;
	}



}






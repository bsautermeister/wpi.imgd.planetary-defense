/*******************************************************************************
 * @file        GameManager.cpp
 * @author      bsautermeister
 * @description The core game management unit, which handles the game loop.
 ******************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include "GameManager.h"
#include "WorldManager.h"
#include "GraphicsManager.h"
#include "InputManager.h"
#include "ResourceManager.h"
#include "ObjectList.h"
#include "LogManager.h"
#include "Clock.h"
#include "EventStep.h"

//prototypes
void doShutDown(int sig);

/**
 * Creates a game manager instance.
 */
GameManager::GameManager(void)
{
	_frameTime = DEFAULT_FRAME_TIME;
	_gameOver = false;
}

/**
 * Hides copy constructor.
 */
GameManager::GameManager(GameManager const &gm)
{
}

/**
 * Hides assignment operator.
 */
void GameManager::operator=(GameManager const &gm)
{
}

/**
 * Cleansup the game manager.
 */
GameManager::~GameManager(void)
{

}

/**
 * Gets the singleton game manager instance.
 * @return The singleton game manager instance.
 */
GameManager& GameManager::getInstance(void)
{
	static GameManager gameManager;
	return gameManager;
}

/**
 * Starts up all game manager services.
 * @return Returns 0 if start up was successfull, else negative.
 */
int GameManager::startUp(void)
{
	return startUp(false);
}

/**
 * Starts up all game manager services.
 * @param flush Configures log file flushing.
 * @return Returns 0 if start up was successfull, else negative.
 */
int GameManager::startUp(bool flush)
{
	return startUp(flush, time(NULL));
}

/**
 * Starts up all game manager services.
 * @param flush Configures log file flushing.
 * @param seed The seed value for the random numbers.
 * @return Returns 0 if start up was successfull, else negative.
 */
int GameManager::startUp(bool flush, time_t seed)
{
	LogManager &logManager = LogManager::getInstance();
	WorldManager &worldManager = WorldManager::getInstance();
	GraphicsManager &graphicsManager = GraphicsManager::getInstance();
	InputManager &inputManager = InputManager::getInstance();
	ResourceManager &resourceManager = ResourceManager::getInstance();

	// catch ctrl+c (SIGINT) and shutdown
	struct sigaction action;
	// set signal handler
	action.sa_handler = doShutDown;
	// clear signal set
	sigemptyset(&action.sa_mask);
	// no special modification to behavior
	action.sa_flags = 0;
	// enable the signal handler
	if (sigaction(SIGINT, &action, NULL))
	{
		perror("Registration of signal handerl failed.");
		return -1;
	}

	// set seed value
	srandom(seed);

	if (logManager.startUp(flush))
	{
		perror("LogManager could not be started");
		return -1;
	}
	else
	{
		logManager.writeLog(LOG_INFO,
			"GameManager::startUp()",
			"LogManager started\n");
	}

	if (worldManager.startUp())
	{
		logManager.writeLog(LOG_ERROR,
			"GameManager::startUp()",
			"WorldManager could not be started\n");
		return -1;
	}
	else
	{
		logManager.writeLog(LOG_INFO,
			"GameManager::startUp()",
			"WorldManager started\n");
	}

	if (graphicsManager.startUp())
	{
		logManager.writeLog(LOG_ERROR,
			"GameManager::startUp()",
			"GraphicsManager could not be started\n");
		return -1;
	}
	else
	{
		logManager.writeLog(LOG_INFO,
			"GameManager::startUp()",
			"GraphicsManager started\n");
	}

	// setup world and view bounds to terminal size
	worldManager.setWorldBoundary(
		Box(
			Position(),
			graphicsManager.getHorizontal(),
			graphicsManager.getVertical()));
	worldManager.setViewBoundary(
		Box(
			Position(),
			graphicsManager.getHorizontal(),
			graphicsManager.getVertical()));

	if (inputManager.startUp())
	{
		logManager.writeLog(LOG_ERROR,
			"GameManager::startUp()",
			"InputManager could not be started\n");
		return -1;
	}
	else
	{
		logManager.writeLog(LOG_INFO,
			"GameManager::startUp()",
			"InputManager started\n");
	}

	if (resourceManager.startUp())
	{
		logManager.writeLog(LOG_ERROR,
			"GameManager::startUp()",
			"ResourceManager could not be started\n");
		return -1;
	}
	else
	{
		logManager.writeLog(LOG_INFO,
			"GameManager::startUp()",
			"ResourceManager started\n");
	}

	_isStarted = true;

	logManager.writeLog(LOG_INFO,
			"GameManager::startUp()",
			"GameManager start up process completed\n");

	return 0;
}

/**
 * Shuts down all game manager services.
 */
void GameManager::shutDown(void)
{
	// verify manager is started
	if (!_isStarted)
		return;

	LogManager &logManager = LogManager::getInstance();
	WorldManager &worldManager = WorldManager::getInstance();
	GraphicsManager &graphicsManager = GraphicsManager::getInstance();
	InputManager &inputManager = InputManager::getInstance();
	ResourceManager &resourceManager = ResourceManager::getInstance();

	logManager.writeLog(LOG_INFO,
			"GameManager::shutDown()",
			"GameManager shut down process started\n");

	logManager.writeLog(LOG_INFO,
			"GameManager::shutDown()",
			"ResourceManager shutting down\n");
	resourceManager.shutDown();

	logManager.writeLog(LOG_INFO,
			"GameManager::shutDown()",
			"InputManager shutting down\n");
	inputManager.shutDown();

	logManager.writeLog(LOG_INFO,
			"GameManager::shutDown()",
			"GraphicsManager shutting down\n");
	graphicsManager.shutDown();

	logManager.writeLog(LOG_INFO,
			"GameManager::shutDown()",
			"WorldManager shutting down\n");
	worldManager.shutDown();

	logManager.writeLog(LOG_INFO,
			"GameManager::shutDown()",
			"LogManager shutting down\n");
	logManager.shutDown();

	_isStarted = false;
}

/**
 * Starts the blocking game loop.
 * @param frameTime The target frame time. Default is DEFAULT_FRAME_TIME.
 * @return The number of game loops updated and rendered.
 */
long int GameManager::run(int frameTime)
{
	LogManager &logManager = LogManager::getInstance();
	WorldManager &worldManager = WorldManager::getInstance();
	GraphicsManager &graphcisManager = GraphicsManager::getInstance();
	InputManager &inputManager = InputManager::getInstance();
	Clock clock;
	long int loopTime = frameTime;
	long int loopCounter = 0;
	long int targetTimeDiff;

	// set custom frame time
	_frameTime = frameTime;

	// reset game over flag
	_gameOver = false;

	while (!_gameOver)
	{
		float lastDelta = loopTime / 1000000.0f;
		clock.delta();
		++loopCounter;

		// 1 - GET INPUT
		inputManager.getInput();

		// 2 - UPDATE GAME SCENE
		// fire events
		EventStep eventStep(lastDelta);
		onEvent(&eventStep);

		// update the game world
		worldManager.update(lastDelta);

		// 3 - RENDER GAME SCENE TO BACK BUFFER
		worldManager.draw();

		// 4 - SWAP BACK BUFFER TO CURRENT BUFFER
		graphcisManager.swapBuffers();

		// 5 - MEASURE CURRENT LOOP TIME AND SLEEP TO HIT THE TARGET TIME
		loopTime = clock.split();
		targetTimeDiff = _frameTime - loopTime;
		// just sleep if target is not already expired
		if (targetTimeDiff > 0)
		{
			usleep(targetTimeDiff);
		}
		else
		{
			logManager.writeLog(LOG_WARNING,
				"GameManager::run()",
				"Target time not reached: %ld\n",
				targetTimeDiff);
		}
	}

	return loopCounter;
}

/**
 * Validates if event is handles by this manager.
 * @param eventType The event type name.
 * @return Returns TRUE if event is handled, else FALSE.
 */
 bool GameManager::isValid(string eventType)
 {
 	return eventType == STEP_EVENT;
 }

/**
 * Sets the game over flag to quit the game loop.
 * @param gameOver The new game over flag value.
 */
void GameManager::setGameOver(bool gameOver)
{
	_gameOver = gameOver;
}

/**
 * Gets the target frame time.
 * @return The taget frame time.
 */
int GameManager::getFrameTime(void)
{
	return _frameTime;
}

/**
 * Shuts down the game manager for CTRL+C signal handling.
 * @param sig The received signal.
 */
void doShutDown(int sig)
{
	LogManager &logManager = LogManager::getInstance();
	GameManager &gameManager = GameManager::getInstance();

	logManager.writeLog(LOG_INFO,
		"GameManager::doShutDown()",
		"Signal received: %d\n",
		sig);

	gameManager.shutDown();
	exit(sig);
}

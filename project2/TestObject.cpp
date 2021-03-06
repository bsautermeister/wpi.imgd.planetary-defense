/*******************************************************************************
 * @file        TestObject.cpp
 * @author      bsautermeister
 * @description Test object used by the simple unit test tool.
 ******************************************************************************/

#include <stdlib.h>
#include "TestObject.h"
#include "GameManager.h"
#include "GraphicsManager.h"
#include "WorldManager.h"
#include "EventStep.h"
#include "EventTest.h"
#include "EventKeyboard.h"
#include "EventMouse.h"
#include "EventOut.h"
#include "EventView.h"
#include "EventCollision.h"
#include "LogManager.h"

#define ENDLESS_TIME 999999999

/**
 * Creates a new test object instance.
 */
TestObject::TestObject(void)
{
	_id = random();
	setType(TYPE_TEST_OBJECT);
	_stepsToGameOver = ENDLESS_TIME;
	_stl = ENDLESS_TIME;

	LogManager &logManager = LogManager::getInstance();
	logManager.writeLog(LOG_DEBUG,
		"TestObject::TestObject()",
		"Test object created with id=%d\n",
		_id);
}

/**
 * Updates the test object.
 */
void TestObject::step(void)
{
	LogManager &logManager = LogManager::getInstance();

	--_stepsToGameOver;
	if (_stepsToGameOver == 0)
	{
		logManager.writeLog(LOG_DEBUG,
			"TestObject::step()",
			"Object with id=%d sets game over flag\n",
			_id);
		GameManager &gameManager = GameManager::getInstance();
		gameManager.setGameOver(true);
	}

	--_stl;
	if (_stl == 0)
	{
		logManager.writeLog(LOG_DEBUG,
			"TestObject::step()",
			"Object with id=%d marks itself for delete\n",
			_id);
		WorldManager &worldManager = WorldManager::getInstance();
		worldManager.markForDelete(this);
	}
}

/**
 * Handles the events.
 */
int TestObject::eventHandler(Event *p_event)
{
	LogManager &logManager = LogManager::getInstance();
	WorldManager &worldManager = WorldManager::getInstance();

	if (p_event->getType() == STEP_EVENT)
	{
		step();
		return 1;
	}

	if (p_event->getType() == TEST_EVENT)
	{
		logManager.writeLog(LOG_DEBUG,
			"TestObject::eventHandler()",
			"Object with id=%d got OUT\n",
			_id);

		// test event kills object to verify event was received
		worldManager.markForDelete(this);
		return 1;
	}

	if (p_event->getType() == KEYBOARD_EVENT)
	{
		EventKeyboard *p_keyboardEvent = static_cast<EventKeyboard *>(p_event);
		int key = p_keyboardEvent->getKey();
		logManager.writeLog(LOG_DEBUG,
			"TestObject::eventHandler()",
			"Object with id=%d got key event: %d\n",
			_id,
			key);

		if (key == UP_KEY || key == ENTER_KEY)
		{
			GameManager &gameManager = GameManager::getInstance();
			gameManager.setGameOver(true);
			return 1;
		}
		else if (key == SPACE_KEY)
		{
			worldManager.markForDelete(this);
			return 1;
		}
	}

	if (p_event->getType() == MOUSE_EVENT)
	{
		EventMouse *p_mouseEvent = static_cast<EventMouse *>(p_event);
		
		logManager.writeLog(LOG_DEBUG,
			"TestObject::eventHandler()",
			"Object with id=%d got mouse event in: (%d, %d)\n",
			_id,
			p_mouseEvent->getPosition().getX(),
			p_mouseEvent->getPosition().getY());

		if (p_mouseEvent->getPosition() == getPosition())
		{
			worldManager.markForDelete(this);
		}
		return 1;
	}

	if (p_event->getType() == OUT_EVENT)
	{
		logManager.writeLog(LOG_DEBUG,
			"TestObject::eventHandler()",
			"Object with id=%d got OUT\n",
			_id);

		worldManager.markForDelete(this);

		// notify view
		EventView ev("OutCounter", 1, true);
		worldManager.onEvent(&ev);

		return 1;
	}

	return 0;
}

/**
 * Renders the test game object.
 */
void TestObject::draw(void)
{
	GraphicsManager &graphicsManager = GraphicsManager::getInstance();
	int color;

	if (getSolidness() == HARD)
		color = COLOR_RED;
	else if (getSolidness() == SOFT)
		color = COLOR_YELLOW;
	else
		color = COLOR_GREEN;

	graphicsManager.drawChar(getPosition(), 'X', color);
}

/**
 * Sets the sets until the objects sets the game over flag.
 */
void TestObject::setStepsToGameOver(int value)
{
	_stepsToGameOver = value;
}

/**
 * Sets the steps to live.
 */
void TestObject::setSTL(int value)
{
	_stl = value;
}

/**
 * Gets the test objects id for identificatoin
 * @return The objects unique id.
 */
int TestObject::getId(void)
{
	return _id;
}

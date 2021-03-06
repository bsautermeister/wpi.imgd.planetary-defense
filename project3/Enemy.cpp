/*******************************************************************************
 * @file        Enemy.cpp
 * @author      kcbryant
 * @description Enemy base class Functions
 ******************************************************************************/

#include <stdlib.h>
#include "EventStep.h"
#include "EventView.h"
#include "GraphicsManager.h"
#include "LogManager.h"
#include "ResourceManager.h"
#include "WorldManager.h"
#include "Enemy.h"
#include "TowerDefenseController.h"
#include "EventEnemyInvasion.h"
#include "EventEnemyKilled.h"
#include "EventDetonation.h"
#include "EventInfo.h"
#include "EventPlayerKilled.h"
#include "Spawner.h"
#include "ExplosionSmall.h"
#include "Player.h"

// initialize the static class variables
bool Enemy::_showInfo = false;

/**
 * Creates a new enemy object instance with speed and health set
 * @param spriteName The name of the sprite asset.
 * @param health The enemies initial health.
 * @param speed The enemies speed.
 * @param killCredits The value the player gains for killing this enemy.
 */
Enemy::Enemy(string spriteName, int health, float speed, int killCredits)
{	
	LogManager &logManager = LogManager::getInstance();
  	ResourceManager &resourceManager = ResourceManager::getInstance();

  	setType(TYPE_ENEMY);
  	_id = abs(random());
  	_health = health;
  	_initHealth = health;
  	_speed = speed;
  	_killCredits = killCredits;
  	_targetReached = false;
  	_hasEndEventSent = false;

  	setSolidness(SOFT);

  	Sprite *p_tempSprite = resourceManager.getSprite(spriteName);
  	if (!p_tempSprite)
  	{
  		logManager.writeLog(LOG_ERROR,
  			"Enemy::Enemy()",
  			"Loading sprite failed.\n");
  	}
  	else
  	{
  		setSprite(p_tempSprite);
    	setSpriteSlowdown(4);	
  	}

	_pathIndex = 0;
	TowerDefenseController* towerDefenseController = TowerDefenseController::getInstance();
	if (towerDefenseController->getPathPositionsCount() != 0)
	{
		_currentTarget = towerDefenseController->getPathPosition(_pathIndex);
		setPosition(_currentTarget);
	}
	else
	{
		_currentTarget = Position(0, 0);
	}

	// register for events
	registerInterest(STEP_EVENT);
	registerInterest(DETONATION_EVENT);
	registerInterest(PLAYER_KILLED_EVENT);
}

/**
 * Cleans up the enemy object.
 */
Enemy::~Enemy(void)
{
}


/**
 * Handle events
 * @param p_event The event.
 * @return Returns 1 if event was handled, else 0.
 */
int Enemy::eventHandler(Event *p_event)
{
	LogManager &logManager = LogManager::getInstance();

	if (p_event->getType() == STEP_EVENT)
	{	
		Player *p_player = Player::getInstance();
		if (p_player->getPause() == false){
			
			if (getSpriteSlowdown() == 0)
			{
				setSpriteSlowdown(4);
			}
	  		// check target reached
			if (getPosition() == _currentTarget)
			{
				nextTarget();
			}

			if (getVelocityX() == 0 && getVelocityY() == 0)
			{
				setVelocityX(_lastX);
				setVelocityY(_lastY);
			}

		 	_lastX = getVelocityX();
	  		_lastY = getVelocityY();

	  		return 1;
  		}

	  	else
	  	{	
	  		setSpriteSlowdown(0);
	  		setVelocityX(0);
	  		setVelocityY(0);
	  	}
  		return 0;

  	}


	if (p_event->getType() == PLAYER_KILLED_EVENT)
	{
		// simply remove itself without bling bling
		WorldManager &world_manager = WorldManager::getInstance();
		world_manager.markForDelete(this);
	}

	if (p_event->getType() == DETONATION_EVENT)
	{
		EventDetonation *p_eventDetonation = static_cast<EventDetonation *>(p_event);
		float dist = distance(p_eventDetonation->getCircle().getCenter(), getPosition());

  		// check if in range
		if (dist < p_eventDetonation->getCircle().getRadius())
		{
			// ensure distance is not zero
			if (dist == 0)
				dist = 1;

			// add linear decreasing splash damage
			float rangeDamage = p_eventDetonation->getDamage() / dist;

			logManager.writeLog(LOG_DEBUG,
				"Enemy::eventHandler()",
				"rangeDamage=%f.\n",
				rangeDamage);

			addDamage((int)rangeDamage);

			logManager.writeLog(LOG_DEBUG,
				"Enemy::eventHandler()",
				"After add damage.\n");
		}

  		return 1;
	}

	return 0;
}

/**
 * Renders the enemys sprite frame with an optional info overlay.
 */
void Enemy::draw(void)
{
	Object::draw();

	if (_showInfo)
	{
		Position topCenter(getPosition().getX(),
			getPosition().getY() - getBox().getVertical() / 2 - 1);

		GraphicsManager &graphcisManager = GraphicsManager::getInstance();
		graphcisManager.drawStringFormat(topCenter,
			CENTER_JUSTIFIED,
			"%d%%",
			(int)(100.0f * _health / _initHealth));
	}
	
}

/**
 * Changes the target to the next path position.
 * @return Returns 0 if ok and -1 if there is no target left.
 */
int Enemy::nextTarget(void)
{
	TowerDefenseController* towerDefenseController = TowerDefenseController::getInstance();
	LogManager &logManager = LogManager::getInstance();

	++_pathIndex;
	
	// verify target reached
	if (_pathIndex >= towerDefenseController->getPathPositionsCount())
	{
		logManager.writeLog(LOG_DEBUG,
			"Enemy::nextTarget()",
			"Final target reached.\n");

		_targetReached = true;

		// delete itself
		killSelf();
		
		return -1;
	}

	_currentTarget = towerDefenseController->getPathPosition(_pathIndex);

	logManager.writeLog(LOG_DEBUG,
			"Enemy::nextTarget()",
			"Next target id x=%d, y=%d.\n",
			_currentTarget.getX(),
			_currentTarget.getY());

	// adjust velocity/direction
	int currentX = getPosition().getX();
	int currentY = getPosition().getY();
	int nextX = _currentTarget.getX();
	int nextY = _currentTarget.getY();

	if (nextY > currentY)
	{
		setVelocityX(0);
		setVelocityY(_speed);
	}
	else if (nextY < currentY)
	{
		setVelocityX(0);
		setVelocityY(-_speed);
	}
	else if (nextX > currentX)
	{
		setVelocityX(_speed);
		setVelocityY(0);
	}
	else if (nextX < currentX)
	{
		setVelocityX(-_speed);
		setVelocityY(0);
	}
	else
	{
		setVelocityX(0);
		setVelocityY(0);
		logManager.writeLog(LOG_DEBUG,
			"Enemy::nextTarget()",
			"Next target unknown.\n");
	}

	return 0;
}

/**
 * Adds damage to the enemy and deletes it, if destroyed.
 * @param damage The damage to add.
 */
void Enemy::addDamage(int damage)
{
	_health -= damage;

	if (_health <= 0)
	{	
		killSelf();
	}
}

/**
 * Gets the current target position.
 * @return The current target position.
 */
Position Enemy::getCurrentTarget(void)
{
	return _currentTarget;
}

/**
 * Gets the speed index of the Enemy.
 * @return float speed index of the enemy.
 */
float Enemy::getSpeed(void)
{
	return _speed;
}

/**
 * Gets the health of the Enemy.
 * @return int the health of the enemy.
 */
int Enemy::getHealth(void)
{
	return _health;
}

/**
 * Gets the credits the player gains for killing this enemy.
 * @return The credits the player gains for killing this enemy.
 */
int Enemy::getKillCredits(void)
{
	return _killCredits;
}

/**
 * Removes the enemy from spawner and world
 */
void Enemy::killSelf(void)
{
	WorldManager &worldManager = WorldManager::getInstance();
	
	if (!_hasEndEventSent)
	{
		if (_targetReached)
		{
			EventEnemyInvasion eventInvasion;
			worldManager.onEvent(&eventInvasion);
		}
		else 
		{
			EventEnemyKilled event(getPosition(), _killCredits);
			worldManager.onEvent(&event);

			new ExplosionSmall(getPosition());
		}

		// remove itself
		worldManager.markForDelete(this);
		_hasEndEventSent = true;
	}
}

/**
 * Gets the id for identification.
 * @return The enemy id.
 */
int Enemy::getId(void)
{
	return _id;
}

/**
 * Sets whether the info overlay shoud be displayed.
 * @param value If the overlay should be displayed or not.
 */
void Enemy::setShowInfo(bool value)
{
	Enemy::_showInfo = value;
}

/**
 * Indicates whether the info overlay shoud be displayed.
 * @Return Returns TRUE if the info is desplayed, else FALSE.
 */
bool Enemy::getShowInfo(void)
{
	return Enemy::_showInfo;
}

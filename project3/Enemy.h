/*******************************************************************************
 * @file        Enemy.h
 * @author      kcbryant
 * @description Base class for Enemies
 ******************************************************************************/
#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "Object.h"
#include "Position.h"

#define TYPE_ENEMY "enemy"

/*
* Base Class for all enemy units
*/
class Enemy : public Object
{
 private:
 	/**
 	 * The ID to identify the enemy.
 	 */
 	int _id;
 	/**
 	 * Amount of health the enemy has.
 	 */
 	int _health;

 	/**
 	 * The initial health.
 	 */
 	int _initHealth;

 	/**
 	 * Speed of the enemy.
 	 */
 	float _speed;

 	/**
 	 * Last x speed of enemy
 	 */
 	float _lastX;

 	/**
 	 * Last x speed of enemy
 	 */
 	float _lastY;

 	/**
 	 * The value the player gains for killing this enemy.
 	 */
 	int _killCredits;

 	/**
 	 * Target for enemy to approach.
 	 */
 	Position _currentTarget;

 	/**
 	 * Indicates whether the target was reached or not.
 	 */
 	bool _targetReached;

 	/**
 	 * Path position index.
 	 */
 	int _pathIndex;

 	/**
 	 * Indicates whether dditional enemy info (hud) is displayed.
 	 */
 	static bool _showInfo;

 	/**
 	 * To prevent mulitple kill events.
 	 */
 	bool _hasEndEventSent;

 public:
	/**
	 * Creates a new enemy object instance with speed and health set
	 * @param spriteName The name of the sprite asset.
	 * @param health The enemies initial health.
	 * @param speed The enemies speed.
	 * @param killCredits The value the player gains for killing this enemy.
	 */
	Enemy(string spriteName, int health, float speed, int killCredits);

	/**
	 * Cleans up the enemy object.
	 */
	virtual ~Enemy(void);

	/**
	 * Handle events
	 * @param p_event The event.
	 * @return Returns 1 if event was handled, else 0.
	 */
	virtual int eventHandler(Event *p_event);

	/**
	 * Renders the enemys sprite frame with an optional info overlay.
	 */
	virtual void draw(void);

	/**
	 * Changes the target to the next path position.
	 * @return Returns 0 if ok and -1 if there is no target left.
	 */
	int nextTarget(void);

	/**
	 * Adds damage to the enemy and deletes it, if destroyed.
	 * @param damage The damage to add.
	 */
	void addDamage(int damage);

	/**
	 * Gets the current target position.
	 * @return The current target position.
	 */
	Position getCurrentTarget(void);

	/** 
	 * Gets the speed index of the Enemy.
	 * @return float speed of the enemy.
	 */
	float getSpeed(void);

	/**
	 * Gets the current health of the Enemy.
	 * @return int the health of the enemy.
	 */
	int getHealth(void);

	/**
	 * Gets the credits the player gains for killing this enemy.
	 * @return The credits the player gains for killing this enemy.
	 */
	int getKillCredits(void);

	/**
	 * Removes the enemy from spawner and world
	 */
	void killSelf(void);

	/**
	 * Gets the id for identification.
	 * @return The enemy id.
	 */
	int getId(void);

	/**
	 * Sets whether the info overlay shoud be displayed.
	 * @param value If the overlay should be displayed or not.
	 */
	static void setShowInfo(bool value);

	/**
	 * Indicates whether the info overlay shoud be displayed.
	 * @Return Returns TRUE if the info is desplayed, else FALSE.
	 */
	static bool getShowInfo(void);
};

#endif

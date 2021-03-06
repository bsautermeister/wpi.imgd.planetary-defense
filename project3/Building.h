/*******************************************************************************
 * @file        Building.h
 * @author      bsautermeister
 * @description The base class for all buildings, no matter what kind of.
 ******************************************************************************/
#ifndef __BUILDING_H__
#define __BUILDING_H__

#include <string>
#include "Object.h"
#include "Event.h"

using std::string;

#define TYPE_BUILDING "building"
#define SELL_FACTOR 0.5f
#define MAX_UPGRADE_LEVEL 5
#define LEVEL_BLINK_INTERVALL 30

/*
* Represents a general building.
*/
 class Building : public Object
 {
 private:
 	/**
 	 * The name of the building.
 	 */
 	string _name;

 	/**
 	 * The always positive price of the building.
 	 */
 	int _price;

 	/**
 	 * The energy requirement or value of the building.
 	 * @note The value can be positive and negative.
 	 */
 	int _energy;

 	/**
 	 * The upgrade level of the building.
 	 */
 	int _level;

 	/**
 	 * The total payed price with upgrades.
 	 */
 	int _totalPrice;

 	/**
 	 * The total energy value with upgrades.
 	 */
 	int _totalEnergy;

 	/**
 	 * For blinking level info.
 	 */
 	int _levelBlinkCounter;

 public:
 	/**
 	 * Creates a new building object and notifies the player.
 	 * @param name The name.
 	 * @param spriteName The name of the sprite.
 	 * @param price The price.
 	 * @param energy The energy price or value.
 	 */
 	Building(string name, string spriteName, int price, int energy);

 	/**
 	 * Cleans the building object and nodifies the player.
 	 */
 	virtual ~Building(void);

 	/**
	 * Handle events
	 * @param p_event The event.
	 * @return Returns 1 if event was handled, else 0.
	 */
	virtual int eventHandler(Event *p_event);

	/**
	 * Renders additional upgrade information.
	 */
	virtual void draw(void);

 	/**
 	 * Gets the buildings name.
 	 * @return The building name.
 	 */
 	string getName(void);

 	/**
 	 * Gets the building price.
 	 * @return The building price.
 	 */
 	int getPrice(void);

 	/**
	 * Gets the building selling price.
	 * @return The building selling price.
	 */
	int getSellingPrice(void);

	/**
 	 * Gets the building upgrade  price.
 	 * @return The building upgrade price.
 	 */
 	int getUpgradePrice(void);

 	/**
 	 * Gets the buildings energy requirement or value.
 	 * @return The energy requirement or value.
 	 */
 	int getEnergy(void);

 	/**
 	 * Gets the buildings energy for selling.
 	 * @return The energy for selling.
 	 */
 	int getSellingEnergy(void);

 	/**
 	 * Gets the buildings upgrade energy requirement or value.
 	 * @return The energy for upgrading.
 	 */
 	int getUpgradeEnergy(void);

 	/**
 	 * Indicates whether the building is able to upgrade.
 	 * @note Default is FALSE.
 	 * @param credits The players credits.
 	 * @param energy The players energy.
 	 * @return Returns if the building is able to upgrade.
 	 */
 	bool canUpgrade(int credits, int energy);

 	/**
 	 * Upgrades the building.
 	 */
 	void upgrade(void);

 	/**
 	 * Gets the upgrade level of the building.
 	 * @return The upgrade level.
 	 */
 	int getLevel(void);
};

#endif


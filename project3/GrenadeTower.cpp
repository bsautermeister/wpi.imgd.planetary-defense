/*******************************************************************************
 * @file        GrenadeTower.h
 * @author      bsautermeister
 * @description The grenade tower, which fires some splash damage grenades.
 ******************************************************************************/

#include "GrenadeTower.h"
#include "FragmentationGrenade.h"

/**
 * Creates a new granade tower object.
 */
GrenadeTower::GrenadeTower(void)
	: Tower(TOWER_GRENADE, "grenadetower",
		INIT_PRICE_GRENADE, INIT_ENERGY_GRENADE,
		60, 15, 20, true)
{

}

/**
 * Fires a granade at the enemy object.
 * @param p_object The enemy object. 
 */
void GrenadeTower::fire(Object *p_object)
{
	new FragmentationGrenade(getPosition(),
		p_object->getPosition(),
		getFirePower());
}

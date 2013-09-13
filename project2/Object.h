/*******************************************************************************
 * @file        Object.h
 * @author      bsautermeister
 * @description The bass class for all game objects representing an object in
 *              the game world.
 ******************************************************************************/

#ifndef __OBJECT_H__
#define __OBJECT_H__

#include <string>
#include "Position.h"
#include "Event.h"

using std::string;

#define TYPE_OBJECT "Object"
#define MAX_OBJ_EVENTS 64
#define MAX_ALTITUDE 4
#define MIN_ALTITUDE 0

/**
 * The game objects possible solidnes states
 */
enum Solidness
{
	HARD,     // game objects cause collision and impede
	SOFT,     // game objects caues collision without impede
	SPECTRAL  // game objects don't collide or impede
};

/**
 * Represents a game object in the game world.
 */
class Object
{
private:
	/**
	 * The type identifier of the object.
	 */
	string _type;

	/**
	 * The objects position in the game world.
	 */
	Position _position;

	/**
	 * The horizontal speed in spaces per step.
	 */
	float _velocityX;

	/**
	 * Countdown to horizontal movement.
	 */
	float _velocityXCountdown;

	/**
	 * The vertical speed in spaces per step.
	 */
	float _velocityY;

	/**
	 * Countdown to vertical movement.
	 */
	float _velocityYCountdown;

	/**
	 * The number of registerd events.
	 */
	int _eventCount;

	/**
	 * The registerd event types.
	 */
	string _eventTypes[MAX_OBJ_EVENTS];

	/**
	 * The objects altitude/depth (0=low,...,4=high).
	 */
	int _altitude;

	/**
	 * The game objects solidness.
	 */
	Solidness _solidness;

	/**
	 * Indicates whether move onto soft objects is blocked.
	 */
	bool _noSoft;

public:
	/**
	* Creates a game object instance.
	* NOTE: Game objects MUST be created on heap via 'new' because they will be 
	*       deleted via 'delete' in the world manager.
	*/
	Object(void);

	/**
	 * Cleans up the game object.
	 */
	virtual ~Object(void);

	/**
	 * Handles events, while default is to ignore all events.
	 * @param p_event Points to the current event to handle.
	 * @return Return 0 if ignored, else 1 if event was handled.
	 */
	virtual int eventHandler(Event *p_event);

	/**
	 * Registers its interest for an event. Keeps track of manager and event.
	 * @param eventType The event type name.
	 * @return Returns 0 if ok, else -1.
	 */
	int registerInterest(string eventType);

	/**
	 * Unregisters its interest for an event.
	 * @note doesn't check if game object has already been registered.
	 * @param eventType The event type name.
	 * @return Returns 0 if ok, else -1.
	 */
	int unregisterInterest(string eventType);

	/**
	 * Renders the game object.
	 */
	virtual void draw(void);

	/**
	 * Gets the game objects type.
	 * @return The game object type.
	 */
	string getType(void);

	/**
	 * Sets the game objects type.
	 * @param type The new game object type.
	 */
	void setType(string type);

	/**
	 * Gets the game objects position in the game world.
	 * @return The game object position.
	 */
	Position getPosition(void);

	/**
	 * Sets the game objects position in the game world.
	 * @param position The new game objects position.
	 */
	void setPosition(Position position);

	/**
	 * Gets the x velocity.
	 * @return The x velocity.
	 */
	float getVelocityX(void);

	/**
	 * Sets the x velocity.
	 * @param velocity The x velocity.
	 */
	void setVelocityX(float velocity);

	/**
	 * Gets the y velocity.
	 * @return The y velocity.
	 */
	float getVelocityY(void);

	/**
	 * Sets the y velocity.
	 * @param velocity The y velocity.
	 */
	void setVelocityY(float velocity);

	/**
	 * Gets the horizontal direction perfomed in one step.
	 * @return The horizontal movement distance for this step.
	 */
	int getVelocityXStep(void);

	/**
	 * Gets the vertical direction perfomed in one step.
	 * @return The vertical movement distance for this step.
	 */
	int getVelocityYStep(void);

	/**
	 * Sets the verifies the altitude/depth of the object.
	 * @param value The new altitude value.
	 * @return Returns 0 if ok, else -1.
	 */
	int setAltitude(int value);

	/**
	 * Gets the objects altitude/depth.
	 * @return The objects altitude.
	 */
	int getAltitude(void);

	/**
	 * Checks whether the object is solid or not.
	 * @return Returns TRUE if the game object is solid, else FALSE.
	 */
	bool isSolid(void);

	/**
	 * Sets the solidness of the game object.
	 * @param value The new solidness value.
	 * @return Returns 0 if ok, else -1.
	 */
	int setSolidness(Solidness value);

	/**
	 * Gets the game objects solidness.
	 * @return The solidness.
	 */
	Solidness getSolidness(void);

	/**
	 * Gets the no soft value.
	 * @return The no soft value.
	 */
	bool getNoSoft(void);

	/**
	 * Sets the no soft value.
	 * @param The no soft value.
	 */
	void setNoSoft(bool value);
};

#endif

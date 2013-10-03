/*******************************************************************************
 * @file        MapObject.h
 * @author      bsautermeister
 * @description Manages the map and the cursor. Also, it is implemented as a
 *              singleton, so e.g. enemies can get map information using this
 *              manager class.
 ******************************************************************************/

#ifndef __MAP_OBJECT_H__
#define __MAP_OBJECT_H__

#include <string>
#include "MapData.h"
#include "Sprite.h"
#include "Object.h"
#include "VirtualCursor.h"

using std::string;

#define TYPE_MAP_OBJECT "map_object"

/**
 * Manages 2D maps and allows virtual cursor navigation.
 */
class MapObject : public Object
{
private:
	/**
	 * The singleton instance variable. Implemented this way with an 
	 * instance variable instead of a local static variable, because 
	 * each object must be created with 'new'.
	 */
	//static MapObject *_p_instance;

	/**
	 * The map data of the current map.
	 */
	MapData *_p_currentMapData;

	/**
	 * The virtual cursor on the map
	 */
	VirtualCursor *_p_cursor;
	
	
public:
	/**
	 * Creates a map object instance.
	 */
	MapObject(void);
	/**
	 * Cleans up the map manager allocated resources.
	 */
	~MapObject(void);

	/**
	 * Gets the singleton map object instance.
	 * @note Singleton required, because other objects like enemies should have
	 *       access to the map object properties.
	 * @return The singleton map object instance.
	 */
	//static MapObject* getInstance(void);

	/**
	 * Handles all events.
	 * @param p_event Points to the current event to handle.
	 * @return Return 0 if ignored, else 1 if event was handled.
	 */
	virtual int eventHandler(Event *p_event);

	/**
	 * Renders the map objects sprite frame. Drawing accounts for: centering,
	 * slowdown, advancing sprite frame.
	 */
	virtual void draw(void);

	/**
	 * Loads a new map from the resource manager.
	 * @note The map must be loaded in resource manager previously.
	 * @param mapLabel The label name of the map to load.
	 * @return Returns 0 if ok, else -1.
	 */
	int loadMap(string mapLabel);

	/**
	 * Gets the maps width in cells.
	 * @return The maps width in cells.
	 */
	int getCellsHorizontal(void);

	/**
	 * Gets the maps height in cells.
	 * @return The maps height in cells.
	 */
	int getCellsVertical(void);

	/**
	 * Gets the maps cell width.
	 * @return The maps cell width.
	 */
	int getCellWidth(void);

	/**
	 * Gets the maps cell height.
	 * @return The maps cell height.
	 */
	int getCellHeight(void);

	/**
	 * Gets the maps background.
	 * @return The maps background.
	 */
	Sprite *getBackground(void);

	/**
	 * Checks whether the cell at the given position is passable.
	 * @param cellPosition The cell position.
	 * @return Returns TRUE if passable, else FALSE.
	 */
	bool isPassable(Position cellPosition);

	/**
	 * Gets the maps enemy's path position at the given index.
	 * @param index The index of the path.
	 * @return The position of the enemy path of the map, or (-1, -1) if out of bounds.
	 */
	Position getPathPosition(int index);

	/**
	 * Gets the number of enemy's path positions.
	 * @return The amount path positions of the enemy path, or -1 if error.
	 */
	int getPathPositionsCount();
};

#endif

//
// GameStart.cpp
//

// engine includes
#include "EventKeyboard.h"
#include "EventStep.h"
#include "GameManager.h"
#include "GraphicsManager.h"	// for COLOR_YELLOW
#include "LogManager.h"
#include "ResourceManager.h"
#include "WorldManager.h"
#include "Box.h"
#include "EventStep.h"

// game includes
#include "GameStart.h"

GameStart::GameStart() {
  setType("GameStart");

  _counter = 0;
  // link to "message" sprite
  ResourceManager &resourceManager = ResourceManager::getInstance();
  Sprite *p_temp_sprite = resourceManager.getSprite("gamestart");
  setSprite(p_temp_sprite);
  setSpriteSlowdown(0);		  

  // put in center of screen
  WorldManager &world_manager = WorldManager::getInstance();
  Box view = world_manager.getViewBoundary();
  Position position(view.getHorizontal() / 2 , view.getVertical() / 2);
  setPosition(position);
  setSpriteIndex(0);

  // register for "keyboard" event
  registerInterest(KEYBOARD_EVENT);
  registerInterest(STEP_EVENT);
}

// handle event
// return 0 if ignored, else 1
int GameStart::eventHandler(Event *p_e) {
   GameManager &game_manager = GameManager::getInstance();

  if (p_e->getType() == STEP_EVENT)
  {
      if (_counter > 25 && _counter < 40)
      {
        setSpriteIndex(1);
      }

      else if (_counter > 41) 
      {
        setSpriteIndex(2);
        _counter = -25;
      }

      ++_counter;
      
  }

  // keyboard
  if (p_e->getType() == KEYBOARD_EVENT) {
    EventKeyboard *p_keyboard_event = (EventKeyboard *) p_e;
    switch (p_keyboard_event->getKey()) {
    case 'p': 			// play
      start();
      break;
    case 'q':			// quit
      //game_manager.setGameOver();
      break;
    default:
      break;
    }
    return 1;
  }

  // if we get here, we have ignored this event
  return 0;
}

void GameStart::start() {}

// override default draw so as not to display "value"
void GameStart::draw() {
  Object::draw();
}
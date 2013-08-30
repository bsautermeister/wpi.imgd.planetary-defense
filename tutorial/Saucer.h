/* Saucer.h */

#ifndef SAUCER_H
#define SAUCER_H

#include "Object.h"
#include "EventCollision.h"

class Saucer : public Object
{
protected:
	int eventHandler(Event *p_event);
	void out(void);
	void moveToStart(void);
	void hit(EventCollision *p_collisionEvent);

public:
	Saucer(void);
	~Saucer(void);
};

#endif


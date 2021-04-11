#ifndef AGENT_H
#define AGENT_H

#include "pvector.h"

class agent{
public:
  agent(float x, float y);
  agent &setVelocity(float x, float y);
  agent &setPosition(float x, float y);
  pvector *getPosition();
  pvector *getVelocity();
private:
  pvector *position;
  pvector *velocity;  
};

#endif
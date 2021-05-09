#pragma once

#include "flowField.h"
#include <vector>
#include "graphics.h"

#define CIRCLE_DISTANCE 0.1  //Wander behavior
#define CIRCLE_RADIUS   0.4

#define FOLLOW_MOUSE   1 
#define STAY_IN_FIELD  2
#define IN_FLOW_FIELD  3
#define STAY_IN_PATH   4
#define STAY_IN_PATH_2 5
#define FLOCK          6
#define WANDER         7
#define FLEE           8
#define PURSUIT        9
/*
#define PURSUIT        8
#define EVADE          8
#define HIDE           9
#define PATH_LOOP      10
#define OBSTACLE_AVOIDANCE 12
#define WALL_FOLLOWER      13
#define OFFSET_PURSUIT     14
#define LEADER_FOLLOWER    15

*/

using namespace  std;

class agent;
class path;
class graphics;

class steeringBehavior{
public:   
   pvector stayInArea(agent &agent, int turnPoint);
   pvector inFlowField(agent &agent, flowField &flow);
   pvector stayInPath(agent &agent, path &path);   
   pvector stayInPath_2(agent &agent, path &path, graphics view);
   pvector seek(agent &agent);
   pvector separation(vector<agent> agents, agent &agent);
   pvector cohesion(vector<agent> boids, agent &agent);
   pvector align(vector<agent> boids, agent &agent);   
   pvector wander(agent &agent);
   pvector flee(agent &agent, graphics &view);
   void setAngle(pvector &p, float angle);
};
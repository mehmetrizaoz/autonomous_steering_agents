/**
 * @file   flock.cpp
 * @author Mehmet Rıza Öz - mehmetrizaoz@gmail.com
 * @brief  flock class implementation
 * @date   15.05.2021
*/

#include "scenario.h"
#include "flock.h"
#include <iostream>
#include <GL/glut.h>

using namespace std;

void flock::loop()
{
    for(auto it = agents.begin(); it < agents.end(); it++){
        view.forceInScreen((*it));
         
        pvector sep = behavior.separation(agents, *it, 6);
        sep.mul(1);         
        pvector ali = behavior.align(agents, *it, 20);
        ali.mul(4);    
        pvector coh = behavior.cohesion(agents, *it, 20);
        coh.mul(0.1);

        (*it).force = sep + ali + coh;
    }
    refresh();
}

flock::flock()
{
    int agentCount = 40;
    float maxForce = 0.5;
    float maxSpeed = 0.9;
    name = "flocking agents";    
    
    createRandomAgents(agentCount, maxForce, maxSpeed);    
    callback = reinterpret_cast <void(*)()> ( (void *)(&loop) );
}
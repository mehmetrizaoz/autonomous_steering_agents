/**
 * @file   steeringBehavior.cpp
 * @author Mehmet Rıza Öz - mehmetrizaoz@gmail.com
 * @brief  implementation of autonomous steering behaviors
 * @date   15.05.2021
*/

#include "steeringBehavior.h"
#include "pvector.h"
#include "agent.h"
#include "path.h"
#include "point.h"
#include <vector>
#include "graphics.h"
#include "math.h"
#include "obstacle.h"
#include <iostream>
#include <GL/glut.h>

using namespace std;

void steeringBehavior::setAngle(pvector &p, float angle)
{
   p.x = cos ( angle * PI / 180.0 );
   p.y = sin ( angle * PI / 180.0 );
}

pvector steeringBehavior::flee(agent &agent, graphics &view, point p)
{
   int radius = 15;

   pvector dist = agent.getTarget() - p;
   //view.drawPoint(agent.targetPoint);
   
   if(dist.magnitude() < radius){
      agent.arrive = false;
      agent.desiredVelocity = agent.position - p;      
   }
   else{
      agent.arrive = true;
      agent.desiredVelocity = agent.getTarget() - agent.position;
   }   
   agent.steering = agent.desiredVelocity - agent.getVelocity();
   return agent.steering;
}

pvector steeringBehavior::evade(vector<agent> boids, agent &evader, graphics view, string name)
{
   agent target;
   for(auto it = boids.begin(); it < boids.end(); it++){
      if((*it).getName() == name){
         target = *it;
      }
   }

   point p = point(evader.position.x + 2, evader.position.y - 2);   
   view.drawText(evader.getName(), p);
   p = point(target.position.x + 2, target.position.y - 2);
   view.drawText(target.getName(), p);
   
   pvector targetVel = target.getVelocity();
   targetVel.mul(5);//TODO: magic number

   point futurePos = target.position + targetVel;
   //view.drawPoint(futurePos);
   
   pvector dist = evader.position - futurePos;
   dist.normalize().mul( 1 / dist.magnitude() );
   
   evader.setTarget(evader.position + dist);
   return flee(evader, view, futurePos);
}

pvector steeringBehavior::pursuit(vector<agent> boids, agent &pursuer, graphics view, string name)
{
   agent target;
   for(auto it = boids.begin(); it < boids.end(); it++){
      if((*it).getName() == name){
         target = *it;
      }
   }

   point p = point(target.position.x + 2, target.position.y - 2);   
   view.drawText(target.getName(), p);
   
   p = point(pursuer.position.x + 2, pursuer.position.y - 2);
   view.drawText(pursuer.getName(), p);
   
   float dist = (target.position - pursuer.position).magnitude();
   float t = dist / target.maxSpeed;

   pvector targetVel = target.getVelocity();
   targetVel.mul(t);
   point futurePos = target.position + targetVel;
   pursuer.setTarget(futurePos);
   return seek(pursuer);
}

pvector steeringBehavior::wander(agent &agent)
{
   pvector circleCenter = agent.getVelocity();
   circleCenter.normalize().mul(CIRCLE_DISTANCE + CIRCLE_RADIUS);
      
   int wanderAngle = (rand() % 360);
   pvector displacement {0, 1};
   setAngle(displacement, wanderAngle);
   displacement.mul(CIRCLE_RADIUS);
   
   agent.desiredVelocity = displacement + circleCenter;
   agent.steering = agent.desiredVelocity - agent.getVelocity();
   
   //move it to the center when it is out of screen
   if(agent.position.x > WIDTH || agent.position.x < -WIDTH ||
      agent.position.y > HEIGHT || agent.position.y < -HEIGHT)
      agent.position = point(0,0);

   return agent.steering;
}

pvector steeringBehavior::align(vector<agent> boids, agent &agent, float radius)
{
   pvector sum {0,0};
   int count = 0;
   for(auto it = boids.begin(); it < boids.end(); it++){
      float d = (agent.position - (*it).position).magnitude();
      if( (d >0) && (d < radius) ){
         sum += (*it).getVelocity();
         count++;
      }
   }
   if(count>0){
      sum.div(count);
      sum.normalize().mul(agent.maxSpeed);
      agent.steering = sum - agent.getVelocity();
      return agent.steering;
   }
   return pvector(0,0);
}

pvector steeringBehavior::cohesion(vector<agent> boids, agent &agent, float radius)
{
   point sum {0,0};
   int count = 0;
   for(auto it = boids.begin(); it < boids.end(); it++){
      float d = (agent.position - (*it).position).magnitude();
      if( (d >0) && (d < radius) ){
         sum = sum + (*it).position;
         count++;
      }
   }
   if(count>0){
      sum.div(count);
      agent.setTarget(sum);
      return seek(agent);
   }
   return pvector(0,0);
}

pvector steeringBehavior::separation(vector<agent> agents, agent &agent, float radius)
{
   pvector sum = pvector(0,0);
   int count = 0;
   for(auto it = agents.begin(); it < agents.end(); it++){      
      float d = (agent.position - (*it).position).magnitude();
      if( (d > 0) && (d < radius) ){
         pvector diff = agent.position - (*it).position;
         diff.normalize().div(d);
         sum = sum + diff;
         count++;
      }
   }
   if(count > 0){
      sum.div(count);      
      sum.normalize().mul(agent.maxSpeed);
      agent.steering = sum - agent.getVelocity();
      return agent.steering;
   }
   return pvector(0,0);
}

pvector steeringBehavior::avoid(vector<obstacle> obstacles, agent &agent)
{
   float dynamic_length = agent.getVelocity().magnitude() / agent.maxSpeed;
   pvector vel = agent.getVelocity();
   vel.normalize().mul(dynamic_length);
   pvector ahead  = vel + agent.position;
   vel.mul(6);
   pvector ahead2 = vel + agent.position;
   //view.drawPoint(ahead, RED);                        
   //view.drawPoint(ahead2, BLUE);  

   for(auto it = obstacles.begin(); it < obstacles.end(); it++){
      float dist  = (ahead  - (*it).getCenter()).magnitude();
      float dist2 = (ahead2 - (*it).getCenter()).magnitude();   
      if(dist < (*it).getRadius() + 2 || dist2 < (*it).getRadius() + 2){
         pvector avoidance = ahead - (*it).getCenter();
         avoidance.normalize().mul(20);
         /*a = point(avoidance.x, avoidance.y);
         view.drawLine(agent.position, agent.position + a, color(0,1,0));*/                  
         return avoidance;      
      }
   }
   return pvector(0,0);
}

pvector steeringBehavior::seek(agent &agent)
{
   agent.desiredVelocity = agent.getTarget() - agent.position;
   agent.steering = agent.desiredVelocity - agent.getVelocity();
   return agent.steering;
}

pvector steeringBehavior::stayInPath(agent &agent, path &path, graphics view)
{
   float worldRecord = 1000000;
   point normalPoint, predictedPos, start, end;
   pvector distance;
   for(auto it = path.points.begin(); it < path.points.end()-1; it++){
      start = point((*it).x, (*it).y);
      end   = point((*(it+1)).x, (*(it+1)).y);
      predictedPos = agent.position + agent.getVelocity();
      normalPoint.getNormalPoint(predictedPos, start, end);
      if (normalPoint.x < start.x || normalPoint.x > end.x){
         normalPoint = end;
      }
      distance = predictedPos - normalPoint;
      if (distance.magnitude() < worldRecord){
         worldRecord = distance.magnitude();
         agent.setTarget(end);
      }
      view.drawPoint(agent.getTarget(), RED);
   }
   return seek(agent);
}

pvector steeringBehavior::inFlowField(agent &agent, flowField &flow)
{
    //pos_x, pos_y must be non negative integer
    int pos_x = abs((int)agent.position.x) % WIDTH;
    int pos_y = abs((int)agent.position.y) % HEIGHT;
    //TODO: modification required for non uniform fields
    return flow.getField(pos_x, pos_y);
}

pvector steeringBehavior::stayInArea(agent &agent, int turnPoint)
{
   if(agent.position.x >= turnPoint){
      agent.desiredVelocity = pvector( -agent.maxSpeed, agent.getVelocity().y );
      agent.steering = agent.desiredVelocity - agent.getVelocity();
      agent.steering.mul(5);
      return agent.steering;
   }
   else if(agent.position.x <= -turnPoint){
      agent.desiredVelocity = pvector( agent.maxSpeed, agent.getVelocity().y );
      agent.steering = agent.desiredVelocity - agent.getVelocity();
      agent.steering.mul(5);
      return agent.steering;
   }
   else if(agent.position.y >= turnPoint){
      agent.desiredVelocity = pvector( agent.getVelocity().x, -agent.maxSpeed );
      agent.steering = agent.desiredVelocity - agent.getVelocity();
      agent.steering.mul(5);
      return agent.steering;
   }
   else if(agent.position.y <= -turnPoint){
      agent.desiredVelocity = pvector( agent.getVelocity().x, agent.maxSpeed );
      agent.steering = agent.desiredVelocity - agent.getVelocity();
      agent.steering.mul(5);
      return agent.steering;
   }
   return pvector(0,0);
}

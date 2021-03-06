/**
 * @file   pvector.cpp
 * @author Mehmet Rıza Öz - mehmetrizaoz@gmail.com
 * @brief  pvector class implementation
 * @date   15.05.2021
*/

#include "pvector.h"
#include "math.h"
#include "point.h"
#include <iostream>
#include <string>

using namespace std;

float pvector::getAngle()
{
   float angle;
   angle = atan2 (this->y, this->x) * 180 / PI;
   return angle;
}

float pvector::angleBetween(pvector v)
{
   float angle = this->dotProduct(v) / (this->magnitude() * v.magnitude());
   angle = acos(angle)  * 180 / PI;
   return angle;
}

float pvector::dotProduct(pvector v)
{
   return ((x * v.x) + (y * v.y));
}

pvector::pvector()
{
   x = 0;
   y = 0;
}

pvector::pvector(float x, float y)
{
   this->x = x;
   this->y = y;
}

void pvector::div(float i)
{
   x = x / i;
   y = y / i;
}

void pvector::mul(float i)
{
   x = x * i;
   y = y * i;
}

void pvector::add(pvector p)
{
   x = x + p.x;
   y = y + p.y;
}

float pvector::magnitude()
{
   return sqrt((this->x * this->x) + (this->y * this->y));
}

pvector &pvector::normalize()
{
   float magnitude = this->magnitude();
   if(magnitude != 0){
      this->x = this->x / magnitude;
      this->y = this->y / magnitude;
   }
   else{
      this->x = 0;
      this->y = 0;
   }
   return *this;  
}

void pvector::limit(float limit)
{
   this->normalize();
   this->mul(limit);
}

pvector pvector::operator + (pvector const &obj)
{
   pvector res;
   res.x = x + obj.x;
   res.y = y + obj.y;
   return res;
}

pvector pvector::operator += (pvector const &obj)
{
   x = x + obj.x;
   y = y + obj.y;
   return *this;
}

bool pvector::operator == (pvector const &obj)
{
   if(x == obj.x && y == obj.y)
      return true;
   return false;
}

pvector pvector::operator + (point const &obj)
{
   pvector res;
   res.x = x + obj.x;
   res.y = y + obj.y;
   return res;
}

pvector pvector::operator - (point const &obj)
{
   pvector res;
   res.x = x - obj.x;
   res.y = y - obj.y;
   return res;
}

void pvector::print(const string &s)
{
   cout << s << " " << x << " " << y << endl;
}

pvector pvector::operator - (pvector const &obj)
{
   pvector res;
   res.x = x - obj.x;
   res.y = y - obj.y;
   return res;
}

#pragma once

#include "scenario.h"
#include <vector>

using namespace std;

class prison: public scenario{
public:
   prison();
   static void loop();  
   void initGL(int* argc, char** argv) override;
};

#pragma once

#include "scenario.h"
#include <vector>

using namespace std;

class flee: public scenario{
public:
   flee();
   static void loop();  
   void initGL(int* argc, char** argv) override;
};

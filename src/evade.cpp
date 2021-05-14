#include "scenario.h"
#include "evade.h"
#include <iostream>
#include <GL/glut.h>

using namespace std;

void evade::loop(){
    for(auto it = agents.begin(); it < agents.end(); it++){
         if((*it).name == "lion"){
            (*it).targetPoint = view.getMousePosition();
            (*it).force  = behavior.seek(*it);
            (*it).arrive = true;
         }
         else{//gazelle
            (*it).force  = behavior.evade(agents, *it, view);
         }
    }
            
    refresh();
}

evade::evade(){    
    name = "evading";
    createAgent(STATIC, nullptr, nullptr, nullptr);
    callback = reinterpret_cast <void(*)()> ( (void *)(&loop) );
}
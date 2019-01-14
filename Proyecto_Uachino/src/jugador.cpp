#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <string>
#include <GL/glew.h> // include GLEW and new version of GL on Windows
#include <glm/glm.hpp>
#include "maths_funcs.h"
#include "malla.h"
#include "jugador.h"


jugador::jugador(){
	vida = 100;
	//velocityY = 0.0f;
	//velocityZ = -0.05f;

}
void jugador::move_R(float k){
	//this->pos += glm::vec3(0,k,0);
}
/*void jugador::jump(float time){
    float gravity = -9.8f;
    if(pos.y >= 0.0f){
        pos.y += velocityY * time;    
        velocityY += gravity * time;        // Apply gravity to vertical velocity
    }
    
}*/


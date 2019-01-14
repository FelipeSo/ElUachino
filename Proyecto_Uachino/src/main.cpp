/******************************************************************************\
| OpenGL 4 Example Code.                                                       |
| Accompanies written series "Anton's OpenGL 4 Tutorials"                      |
| Email: anton at antongerdelan dot net                                        |
| First version 27 Jan 2014                                                    |
| Copyright Dr Anton Gerdelan, Trinity College Dublin, Ireland.                |
| See individual libraries for separate legal notices                          |
|******************************************************************************|
| This demo uses the Assimp library to load a mesh from a file, and supports   |
| many formats. The library is VERY big and complex. It's much easier to write |
| a simple Wavefront .obj loader. I have code for this in other demos. However,|
| Assimp will load animated meshes, which will we need to use later, so this   |
| demo is a starting point before doing skinning animation                     |
\******************************************************************************/
#include <assimp/cimport.h> // C importer
#include <assimp/scene.h> // collects data
#include <assimp/postprocess.h> // various extra operations
#include <GL/glew.h> // include GLEW and new version of GL on Windows
#include <GLFW/glfw3.h> // GLFW helper library
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include "maths_funcs.h"
#include "gl_utils.h"
#include "skybox.h"
#include "malla.h"
#include "Camera.h"
#include "stb_image.c" // Sean Barrett's image loader - http://nothings.org/
#include "image.h"


#define _USE_MATH_DEFINES
#include <math.h>


#define GL_LOG_FILE "log/gl.log"
#define VERTEX_SHADER_FILE "shaders/test_vs.glsl"
#define FRAGMENT_SHADER_FILE "shaders/test_fs.glsl"

#define SKYBOX_VERTEX_SHADER_FILE "shaders/sky_vert.glsl"
#define SKYBOX_FRAGMENT_SHADER_FILE "shaders/sky_frag.glsl"
#define ONE_DEG_IN_RAD (2.0 * M_PI) / 360.0

int g_gl_width = 800;
int g_gl_height = 600;
GLFWwindow* g_window = NULL;


void init();
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window, malla *personaje);


// camera
Camera cam;


// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;
float mult = 0.0;
float moveSpeed = 0.0f;

int main(){
	init();

	/*-------------------------------CREATE SHADERS-------------------------------*/
	GLuint shader_programme = create_programme_from_files (VERTEX_SHADER_FILE, FRAGMENT_SHADER_FILE);
	GLuint skybox_shader = create_programme_from_files (SKYBOX_VERTEX_SHADER_FILE, SKYBOX_FRAGMENT_SHADER_FILE);
	skybox sky = skybox(shader_programme, skybox_shader);

	int model_mat_location = glGetUniformLocation (shader_programme, "model");
	glm::mat4 projection = cam.projection();
	glm::mat4 view = cam.view();

	cam.promat(shader_programme);
	cam.viewmat(shader_programme);
	cam.setPosition(glm::vec3(0.0f, 1.0f, 0.0f));


	malla *personaje = new malla((char*)"mallas/persona_corriendo.obj", shader_programme, GL_TRIANGLES);
	personaje->set_pos(glm::vec3(0.0f, 0.0f, -5.0f));
	personaje->load_texture("texturas/corazon.jpg");

	malla *piso = new malla((char*)"mallas/obj/piso.obj", shader_programme, GL_TRIANGLES);
	piso->set_pos(glm::vec3(0.0f, 0.0f, 0.0f));
	piso->load_texture("texturas/pis.jpg");

	malla *pared1 = new malla((char*)"mallas/obj/pared.obj", shader_programme, GL_TRIANGLES);
	pared1->set_pos(glm::vec3(3.25f, 0.0f, 0.0f));
	pared1->load_texture("texturas/pared.jpg");

	malla *pared2 = new malla((char*)"mallas/obj/pared.obj", shader_programme, GL_TRIANGLES);
	pared2->set_pos(glm::vec3(-3.25f, 0.0f, 0.0f));
	pared2->load_texture("texturas/pared.jpg");

	malla *techo = new malla((char*)"mallas/obj/piso.obj", shader_programme, GL_TRIANGLES);
	techo->set_pos(glm::vec3(0.0f, 3.0f, 0.0f));
	techo->load_texture("texturas/techo.jpg");

	for(int i=0; i<10; i++){

	}
	malla *prueba = new malla((char*)"mallas/texto_inicio.obj", shader_programme, GL_TRIANGLES);
	//prueba->set_pos(glm::vec3(0.0f, 0.0f, 0.0f));
	prueba->load_texture("texturas/prueba.jpg");

	

	// render loop
	float dx = 0.0f;
	int mult = 1;
	// -----------
	while (!glfwWindowShouldClose(g_window)){
		// per-frame time logic
		// --------------------
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// input
		// -----
		processInput(g_window, personaje);
		personaje->set_pos(personaje->get_pos());

		// render
		// ------
		//glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glViewport (0, 0, g_gl_width, g_gl_height);

		
		sky.render_skybox(skybox_shader);
		// activate shader
		glUseProgram (shader_programme);
		
		cam.promat(shader_programme);
		cam.viewmat(shader_programme);
		if(dx>2.5f || dx<-2.5f){
			mult = mult*-1;
		}
		if(fmod(currentFrame, 2)<=1.2f && moveSpeed >1.0f){
			personaje->reflect_x();	
		}
		prueba->set_pos(glm::vec3(dx, 2.0f, -4.0f));
		// camera/view transformation+
		
		personaje->updatepos(deltaTime);
		//personaje->drawHealth(100);
		personaje->render_indices();
        piso->render_indices();
        pared1->render_indices();
        pared2->render_indices();
        techo->render_indices();
        prueba->render_indices();

        dx += 0.02f*mult;
		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(g_window);
		glfwPollEvents();
		sky.update_skybox(skybox_shader, shader_programme, cam.position());

	}
	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window, malla* personaje){
	//cam.lookAt(personaje->get_pos());
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if((glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) & (personaje->get_pos().y <= 0.0f)){ 
    	personaje->jump();
    }
    if((glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) & (personaje->get_pos().x>-2.5f)){
        personaje->move_R(-0.1f);
    }
    else if((glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) & (personaje->get_pos().x<2.5f)){
        personaje->move_R(0.1f);
    }

    //CAMERA INPUT
	
    if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS){
    	moveSpeed = 5.0f;
    	personaje->setvelZ(-5.0f);
    }
    cam.offsetPosition(deltaTime *  cam.forward() * moveSpeed); //hace avanzar a la camara automaticamente hacia adelante
    if(glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS){
        cam.offsetPosition(deltaTime * moveSpeed * -cam.forward());
    } 
    else if(glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS){
        cam.offsetPosition(deltaTime * moveSpeed * cam.forward());
    }
    if(glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS){
        cam.setPosition(glm::vec3(0.0f, 1.0f, 0.0f));
        personaje->set_pos(glm::vec3(0.0f, 0.0f, -5.0f));
        moveSpeed = 0.0f;
        personaje->setvelZ(0.0f);
        //personaje->updatepos(deltaTime);
    }
    if((glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) & cam.position().x>-2.5f){
        cam.offsetPosition(deltaTime * moveSpeed * -cam.right());
    }
    else if((glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) & cam.position().x<2.5f){
        cam.offsetPosition(deltaTime * moveSpeed * cam.right());
    }
    if(glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS){
        cam.offsetPosition(deltaTime * moveSpeed * -glm::vec3(0,1,0));
    } 
    else if(glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS){
        cam.offsetPosition(deltaTime * moveSpeed * glm::vec3(0,1,0));
    }


}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

void init(){
	restart_gl_log ();
	start_gl ();
	glEnable (GL_DEPTH_TEST); // enable depth-testing
	glDepthFunc (GL_LESS); // depth-testing interprets a smaller value as "closer"
	glEnable (GL_CULL_FACE); // cull face
	glCullFace (GL_BACK); // cull back face
	glFrontFace (GL_CCW); // set counter-clock-wise vertex order to mean the front
	glClearColor (0.2, 0.2, 0.2, 1.0); // grey background to help spot mistakes
	glViewport (0, 0, g_gl_width, g_gl_height);

	glfwSetFramebufferSizeCallback(g_window, framebuffer_size_callback);
	glfwSetWindowTitle (g_window, "El UACHino");

}
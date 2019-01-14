#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string>
#include <GL/glew.h> // include GLEW and new version of GL on Windows
#include <glm/glm.hpp>
#include <assimp/cimport.h> // C importer
#include <assimp/scene.h> // collects data
#include <assimp/postprocess.h> // various extra operations
#include "stb_image.h"
#include "maths_funcs.h"
#include "gl_utils.h"
#include "skybox.h"

#define PI 3.14159265359

using namespace std;

skybox::skybox(GLuint shader_programme, GLuint _skybox_shader){
	float points[] = {
		-10.0f, 10.0f,	-10.0f, -10.0f, -10.0f, -10.0f, 10.0f,	-10.0f, -10.0f,
		10.0f,	-10.0f, -10.0f, 10.0f,	10.0f,	-10.0f, -10.0f, 10.0f,	-10.0f,

		-10.0f, -10.0f, 10.0f,	-10.0f, -10.0f, -10.0f, -10.0f, 10.0f,	-10.0f,
		-10.0f, 10.0f,	-10.0f, -10.0f, 10.0f,	10.0f,	-10.0f, -10.0f, 10.0f,

		10.0f,	-10.0f, -10.0f, 10.0f,	-10.0f, 10.0f,	10.0f,	10.0f,	10.0f,
		10.0f,	10.0f,	10.0f,	10.0f,	10.0f,	-10.0f, 10.0f,	-10.0f, -10.0f,

		-10.0f, -10.0f, 10.0f,	-10.0f, 10.0f,	10.0f,	10.0f,	10.0f,	10.0f,
		10.0f,	10.0f,	10.0f,	10.0f,	-10.0f, 10.0f,	-10.0f, -10.0f, 10.0f,

		-10.0f, 10.0f,	-10.0f, 10.0f,	10.0f,	-10.0f, 10.0f,	10.0f,	10.0f,
		10.0f,	10.0f,	10.0f,	-10.0f, 10.0f,	10.0f,	-10.0f, 10.0f,	-10.0f,

		-10.0f, -10.0f, -10.0f, -10.0f, -10.0f, 10.0f,	10.0f,	-10.0f, -10.0f,
		10.0f,	-10.0f, -10.0f, -10.0f, -10.0f, 10.0f,	10.0f,	-10.0f, 10.0f
	};
	this->skybox_shader = _skybox_shader;
	glGenBuffers( 1, &vbosky );
	glBindBuffer( GL_ARRAY_BUFFER, vbosky );
	glBufferData( GL_ARRAY_BUFFER, 3 * 36 * sizeof( GLfloat ), &points,
								GL_STATIC_DRAW );

	glGenVertexArrays( 1, &vaosky );
	glBindVertexArray( vaosky );
	glEnableVertexAttribArray( 0 );
	glBindBuffer( GL_ARRAY_BUFFER, vbosky );
	glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, NULL );


	mat4 pers_mat = perspective(67.0f, (float)g_gl_width / (float)g_gl_height, 0.1f, 100.0f);
    mat4 ortho_mat = orthographic(-100, 100, 2.5, -2.5, -4, 4);
    // para gameplay con camara
    mat4 proj = pers_mat;

    float cam_speed = 4.0f; // 1 unit per second
	float cam_yaw_speed = 70.0f;
	float cam_yaw = 0.0f;
	mat4 T = translate (identity_mat4 (), vec3 (0.0f, 0.0f, -3.0f));
	mat4 R = rotate_y_deg (identity_mat4 (), -cam_yaw);
	mat4 view_mat = R * T;
	
	int view_mat_location = glGetUniformLocation (shader_programme, "view");
	glUseProgram (shader_programme);
	glUniformMatrix4fv (view_mat_location, 1, GL_FALSE, view_mat.m);
	int proj_mat_location = glGetUniformLocation (shader_programme, "proj");
	glUseProgram (shader_programme);
	glUniformMatrix4fv (proj_mat_location, 1, GL_FALSE, proj.m);
// aqui iba el create skybox_shader
	glUseProgram (skybox_shader);

	int view_skybox = glGetUniformLocation (skybox_shader, "view");
	int proj_skybox = glGetUniformLocation (skybox_shader, "proj");
	glUniformMatrix4fv (view_skybox, 1, GL_FALSE, view_mat.m);
	glUniformMatrix4fv (proj_skybox, 1, GL_FALSE, proj.m);

	glActiveTexture( GL_TEXTURE0 );
	glGenTextures( 1, &cube_map_texture );

	 load_cube_map_side( cube_map_texture, GL_TEXTURE_CUBE_MAP_POSITIVE_Z, "texturas/city_bk.tga");
	 load_cube_map_side( cube_map_texture, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, "texturas/city_ft.tga");
	 load_cube_map_side( cube_map_texture, GL_TEXTURE_CUBE_MAP_POSITIVE_Y, "texturas/city_up.tga");
	 load_cube_map_side( cube_map_texture, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, "texturas/city_dn.tga");
	 load_cube_map_side( cube_map_texture, GL_TEXTURE_CUBE_MAP_NEGATIVE_X, "texturas/city_lf.tga");
	 load_cube_map_side( cube_map_texture, GL_TEXTURE_CUBE_MAP_POSITIVE_X, "texturas/city_rt.tga");

	glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE );
	glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
//end wea
}
bool skybox::load_cube_map_side( GLuint texture, GLenum side_target, const char *file_name ) {
	
	glBindTexture( GL_TEXTURE_CUBE_MAP, texture );

	int x, y, n;
	int force_channels = 4;
	unsigned char *image_data = stbi_load( file_name, &x, &y, &n, force_channels );
	if ( !image_data ) {
		fprintf( stderr, "ERROR: could not logad %s\n", file_name );
		return false;
	}
	// non-power-of-2 dimensions check
	if ( ( x & ( x - 1 ) ) != 0 || ( y & ( y - 1 ) ) != 0 ) {
		fprintf( stderr, "WARNING: image %s is not power-of-2 dimensions\n", file_name );
	}

	// copy image data into 'target' side of cube map
	glTexImage2D( side_target, 0, GL_RGBA, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data );
	free( image_data );
	return true;
}

void skybox::render_skybox(GLuint skybox_shader){
	glUseProgram (skybox_shader);
	glDepthMask( GL_FALSE );
	glUseProgram( skybox_shader );
	glActiveTexture( GL_TEXTURE0 );
	glBindTexture( GL_TEXTURE_CUBE_MAP, cube_map_texture );
	glBindVertexArray( vaosky );
	glDrawArrays( GL_TRIANGLES, 0, 36 );
	glDepthMask( GL_TRUE );

}
void skybox::update_skybox(GLuint skybox_shader, GLuint shader_programme, glm::vec3 c_pos){	
	mat4 T = translate (identity_mat4 (), vec3 (c_pos.x, c_pos.y, c_pos.z));
    mat4 R = rotate_y_deg(identity_mat4 (), 0.0f); // 
	mat4 view_mat = R * T;
	int view_mat_location = glGetUniformLocation (shader_programme, "view");
	int view_skybox = glGetUniformLocation (skybox_shader, "view");
	glUniformMatrix4fv(view_mat_location, 1, GL_FALSE, view_mat.m);
	glUseProgram (skybox_shader);
	glUniformMatrix4fv(view_skybox, 1, GL_FALSE, R.m);
}
#ifndef SKYBOX_H
#define SKYBOX_H

using namespace std;

class skybox{
	private:
		GLuint vaosky, vbosky, cube_map_texture;


	protected:
		GLuint skybox_shader;



	public:
		skybox(GLuint shader_programme, GLuint _skybox_shader);
		// funcion de carga del skybox
		bool load_cube_map_side( GLuint texture, GLenum side_target, const char *file_name );

		// funcion de renderizado del skybox
		void render_skybox(GLuint skybox_shader);
		// update del skybox en el loop principal
		void update_skybox(GLuint skybox_shader, GLuint shader_programme, glm::vec3 c_pos);
};

#endif

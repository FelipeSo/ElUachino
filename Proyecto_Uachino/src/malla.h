#ifndef MALLA_H
#define MALLA_H


#define QUAD_FACTOR     4

using namespace std;

class malla{
    private:
        char* filename;
        GLuint nvertices;
        GLuint nfaces;
        mat4 M;
        glm::vec3 pos;
        GLuint FTYPE;
        GLuint FFACTOR;
        GLuint vao, vbo, nbo, tbo, ebo;
        GLuint matloc;
        float velocityY, velocityZ;

        protected:
        GLuint tex;
        GLuint shaderprog;


    public:
        malla(const char *filename, GLuint shadprog, GLuint face_type);
        malla();
//functions
        void render_vertices_points(GLfloat point_size);
        void render_vertices();
        void render_indices();
        bool load_mesh(const char* file_name, GLuint face_type);
        bool load_texture(const char* file_name);
        void makevao(GLfloat *vertices, GLfloat* normals, GLfloat* texcoords, GLuint *indices);
        void mtranslate(vec3 d);
        void mrotatey(float d);
        void reset_matrix();
        void print_matrix();
        void move_R(float k);
        void jump();
        void setvelZ(float s);
        void reflect_x();
        void updatepos(float time);
        void drawHealth(float health);

// gets
        glm::vec3 get_pos();
        mat4 get_matrix();
        GLuint get_vao();
        GLuint get_facetype();
        GLuint get_facefactor();
        GLuint get_matloc();
        char* get_filename();
        int get_numvertices();
        int get_numfaces();

// sets
        void set_pos(glm::vec3 p);
        void set_facetype(GLuint FTYPE);
        void set_facefactor(GLuint FFACTOR);
        void set_numvertices(int nvert);
        void set_numfaces(int ntri);
        void set_matrix(mat4 M);
        void set_matloc(GLuint matloc);
        void set_shaderprog(GLuint sprog);
};

#endif

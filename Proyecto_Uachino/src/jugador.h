class jugador: public malla{
	private:
		int vida;
		GLuint vao, vbo;
        int numvertices;
        glm::vec3 pos;
        float v_Y, v_Z;
        char* filename;

    public:
    	jugador();
    	void move_R(float k);
    	//void jump(float time);

};

#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

namespace boundforge {

class Shader {
	public:
		GLuint ID;

		Shader(const std::string& vertexPath, const std::string& fragmentPath);
		~Shader();

		void use() const;

		void setBool(const std::string &name, bool value) const;
		void setInt(const std::string &name, int value) const;
		void setFloat(const std::string &name, float value) const;
		void setMat4(const std::string &name, const float* value) const;
	private:
		std::string loadFile(const std::string& path);
		GLuint compileShader(GLenum type, const std::string& source);
};

}

#endif

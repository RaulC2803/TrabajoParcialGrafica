#pragma once
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <glm/glm.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;

class CProgramaShaders {

public:
	GLuint shaderProgram;
	CProgramaShaders(string rutaShaderVertice, string rutaShaderFragmento) {
		//variables para leer los archivos
		string strCodigoShaderVertice;
		string strCodigoShaderFragmento;
		ifstream pArchivoShaderVertice;
		ifstream pArchivoShaderFragmento;

		//Mostramos exepciones en caso haya
		pArchivoShaderVertice.exceptions(ifstream::failbit | ifstream::badbit);
		pArchivoShaderFragmento.exceptions(ifstream::failbit | ifstream::badbit);
		try {
			//Abriendo los archivos de código de los shader
			pArchivoShaderVertice.open(rutaShaderVertice);
			pArchivoShaderFragmento.open(rutaShaderFragmento);
			//Leyendo la información de loar archivos
			stringstream lectorShaderVertice, lectorShaderFragmento;
			lectorShaderVertice << pArchivoShaderVertice.rdbuf();
			lectorShaderFragmento << pArchivoShaderFragmento.rdbuf();
			//cerrando
			pArchivoShaderFragmento.close();
			pArchivoShaderVertice.close();
			//Pasando la información leída a string
			strCodigoShaderFragmento = lectorShaderFragmento.str();
			strCodigoShaderVertice = lectorShaderVertice.str();
		}
		catch (ifstream::failure) {
			cout << "ERROR: Los archivos no pudieron ser leidos correctamente";
		}
		const char* vertexShaderSource = strCodigoShaderVertice.c_str();
		const char* fragmentShaderSource = strCodigoShaderFragmento.c_str();
		//Asociando y compilando los shader
		//shader vertice
		int vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
		glCompileShader(vertexShader);
		verificarErrores(vertexShader, "Vértice");

		//shader fragmento
		int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
		glCompileShader(fragmentShader);
		verificarErrores(fragmentShader, "Fragmento");

		//shader Programa
		this->shaderProgram = glCreateProgram();
		glAttachShader(this->shaderProgram, vertexShader);
		glAttachShader(this->shaderProgram, fragmentShader);
		glLinkProgram(this->shaderProgram);
		verificarErrores(this->shaderProgram, "Program");

		//check for linking errors
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
	}
	~CProgramaShaders() {
		glDeleteProgram(this->shaderProgram);
	}

	void usar() const {
		// draw our first triangle
		glUseProgram(this->shaderProgram);
	}

	void setBool(const std::string& name, bool value) const
	{
		glUniform1i(glGetUniformLocation(shaderProgram, name.c_str()), (int)value);
	}
	// ------------------------------------------------------------------------
	void setInt(const std::string& name, int value) const
	{
		glUniform1i(glGetUniformLocation(shaderProgram, name.c_str()), value);
	}
	// ------------------------------------------------------------------------
	void setFloat(const std::string& name, float value) const
	{
		glUniform1f(glGetUniformLocation(shaderProgram, name.c_str()), value);
	}
	// ------------------------------------------------------------------------
	void setVec2(const std::string& name, const glm::vec2& value) const
	{
		glUniform2fv(glGetUniformLocation(shaderProgram, name.c_str()), 1, &value[0]);
	}
	void setVec2(const std::string& name, float x, float y) const
	{
		glUniform2f(glGetUniformLocation(shaderProgram, name.c_str()), x, y);
	}
	// ------------------------------------------------------------------------
	void setVec3(const std::string& name, const glm::vec3& value) const
	{
		glUniform3fv(glGetUniformLocation(shaderProgram, name.c_str()), 1, &value[0]);
	}
	void setVec3(const std::string& name, float x, float y, float z) const
	{
		glUniform3f(glGetUniformLocation(shaderProgram, name.c_str()), x, y, z);
	}
	// ------------------------------------------------------------------------
	void setVec4(const std::string& name, const glm::vec4& value) const
	{
		glUniform4fv(glGetUniformLocation(shaderProgram, name.c_str()), 1, &value[0]);
	}
	void setVec4(const std::string& name, float x, float y, float z, float w)
	{
		glUniform4f(glGetUniformLocation(shaderProgram, name.c_str()), x, y, z, w);
	}
	// ------------------------------------------------------------------------
	void setMat2(const std::string& name, const glm::mat2& mat) const
	{
		glUniformMatrix2fv(glGetUniformLocation(shaderProgram, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}
	// ------------------------------------------------------------------------
	void setMat3(const std::string& name, const glm::mat3& mat) const
	{
		glUniformMatrix3fv(glGetUniformLocation(shaderProgram, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}
	// ------------------------------------------------------------------------
	void setMat4(const std::string& name, const glm::mat4& mat) const
	{
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}

	void setColor(const string& name, float r, float g, float b) const {
		glUniform4f(glGetUniformLocation(this->shaderProgram, name.c_str()), r, g, b, 1.0);
	}

private:
	void verificarErrores(int identificador, string tipo) {
		int success;
		char infoLog[512];
		if (tipo == "Vértice") {
			glGetShaderiv(identificador, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(identificador, 512, NULL, infoLog);
				std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
			}
		}
		else {
			if (tipo == "Fragmento") {
				glGetShaderiv(identificador, GL_COMPILE_STATUS, &success);
				if (!success)
				{
					glGetShaderInfoLog(identificador, 512, NULL, infoLog);
					std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
				}
			}
			else {
				glGetProgramiv(identificador, GL_LINK_STATUS, &success);
				if (!success) {
					glGetProgramInfoLog(identificador, 512, NULL, infoLog);
					std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
				}
			}

		}

	}
};

struct Figure {
	float* Vertices;
	unsigned int* Triangles;
	int nVertices;
	int nTriangles;
	bool rgb = false, texture = false;
	unsigned int VAO, VBO, EBO;

	~Figure() {
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &EBO);
		delete[] Vertices;
		delete[] Triangles;
	}

	void registerFigure() {
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);


		glBindVertexArray(VAO);


		if (this->nTriangles > 0) {
			glGenBuffers(1, &EBO);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3 * this->nTriangles * sizeof(unsigned int), this->Triangles, GL_STATIC_DRAW);
		}

		if (!this->rgb && !this->texture) {
			glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
			glBufferData(GL_ARRAY_BUFFER, 3 * this->nVertices * sizeof(float), this->Vertices, GL_STATIC_DRAW);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(0);
		}

		else if (this->rgb) {
			glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
			glBufferData(GL_ARRAY_BUFFER, 6 * this->nVertices * sizeof(float), this->Vertices, GL_STATIC_DRAW);

			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
			glEnableVertexAttribArray(1);
		}

		else if (this->texture) {
			glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
			glBufferData(GL_ARRAY_BUFFER, 5 * this->nVertices * sizeof(float), this->Vertices, GL_STATIC_DRAW);

			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
			glEnableVertexAttribArray(2);

		}
	}

	void drawFigure() {
		glBindVertexArray(this->VAO);
		if (this->nTriangles > 0)
			glDrawElements(GL_TRIANGLES, this->nTriangles * 3, GL_UNSIGNED_INT, 0);
		else
			glDrawArrays(GL_TRIANGLES, 0, this->nVertices);
	}
};

void read(std::string nameFile, Figure* figure, bool texture, bool rgb) {
	ifstream file(nameFile, ios::_Noreplace);
	char line[128];
	string aux;

	if (file.fail())
		cout << "Can't read the file" << endl;
	else {

		file.getline(line, '\n');

		file >> figure->nVertices >> figure->nTriangles >> aux;
		if (!rgb && !texture) {
			figure->Vertices = new float[int(figure->nVertices) * 3];
			for (int i = 0; i < figure->nVertices; i++)
				file >> figure->Vertices[3 * i] >> figure->Vertices[3 * i + 1] >> figure->Vertices[3 * i + 2];
		}

		else if (texture) {
			figure->Vertices = new float[int(figure->nVertices) * 5];
			for (int i = 0; i < figure->nVertices; i++)
				file >> figure->Vertices[5 * i] >> figure->Vertices[5 * i + 1] >> figure->Vertices[5 * i + 2] >> figure->Vertices[5 * i + 3] >> figure->Vertices[5 * i + 4];
			figure->texture = true;
		}
		else if (rgb) {
			figure->Vertices = new float[int(figure->nVertices) * 6];
			for (int i = 0; i < figure->nVertices; i++)
				file >> figure->Vertices[6 * i] >> figure->Vertices[6 * i + 1] >> figure->Vertices[6 * i + 2] >> figure->Vertices[6 * i + 3] >> figure->Vertices[6 * i + 4] >> figure->Vertices[6 * i + 5];
			figure->rgb = true;
		}

		figure->Triangles = new unsigned int[int(figure->nTriangles) * 3];

		for (int i = 0; i < figure->nTriangles; i++)
			file >> aux >> figure->Triangles[3 * i] >> figure->Triangles[3 * i + 1] >> figure->Triangles[3 * i + 2];

		file.close();
	}

}
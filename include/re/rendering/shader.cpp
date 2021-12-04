#include "shader.h"

#include <cassert>

#include <glm/gtc/type_ptr.hpp>

#include "../utils/file_helper.h"

Shader::Shader() : logger(Logger::GetInstance()) {}

Shader::Shader(const std::string &vertexPath, const std::string &fragmentPath) : logger(Logger::GetInstance()) {
    Compile(GetOpenGLShaderSourceFromPaths(vertexPath, fragmentPath));
}

Shader::Shader(OpenGLShaderSourceCode openGlShaderSourceCode) : logger(Logger::GetInstance()) {
    Compile(openGlShaderSourceCode);
}

Shader::~Shader() {}

OpenGLShaderSourceCode Shader::GetOpenGLShaderSourceFromPaths(const std::string &vertexPath, const std::string &fragmentPath) {
    OpenGLShaderSourceCode openGlShaderSourceCode;
    if (IsShaderFilesValid(vertexPath, fragmentPath)) {
        std::ifstream vertexShaderFile;
        std::ifstream fragmentShaderFile;
        vertexShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        fragmentShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        try {
            vertexShaderFile.open(vertexPath.c_str());
            fragmentShaderFile.open(fragmentPath.c_str());
            std::stringstream vertexShaderStream, fragmentShaderStream;
            vertexShaderStream << vertexShaderFile.rdbuf();
            fragmentShaderStream << fragmentShaderFile.rdbuf();
            vertexShaderFile.close();
            fragmentShaderFile.close();
            openGlShaderSourceCode.vertex = vertexShaderStream.str();
            openGlShaderSourceCode.fragment = fragmentShaderStream.str();
        } catch(std::ifstream::failure& e) {
            logger->Error("Error reading shader files!\n"
                          "vertex path = '%s'\nfragment path = '%s'!", vertexPath.c_str(), fragmentPath.c_str());
        }
    }
    return openGlShaderSourceCode;
}

void Shader::Compile(OpenGLShaderSourceCode openGlShaderSourceCode) {
    unsigned int vertex, fragment;
    const char *vertexSource = openGlShaderSourceCode.vertex.c_str();
    const char *fragmentSource = openGlShaderSourceCode.fragment.c_str();
    // vertex
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vertexSource, nullptr);
    glCompileShader(vertex);
    CheckCompileErrors(vertex, "VERTEX");
    // fragment
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fragmentSource, nullptr);
    glCompileShader(fragment);
    CheckCompileErrors(fragment, "FRAGMENT");
    // shader
    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);
    CheckCompileErrors(ID, "PROGRAM");
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

void Shader::Use() {
    glUseProgram(ID);
}

void Shader::SetBool(const std::string &name, bool value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::SetInt(const std::string &name, int value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::SetFloat(const std::string &name, float value) const {
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::SetVec2Float(const std::string &name, float v1, float v2) const {
    glUniform2f(glGetUniformLocation(this->ID, name.c_str()), v1, v2);
}

void Shader::SetVec2Float(const std::string &name, const Vector2 &value) const {
    glUniform2f(glGetUniformLocation(this->ID, name.c_str()), value.x, value.y);
}

void Shader::SetVec3Float(const std::string &name, float v1, float v2, float v3) const {
    glUniform3f(glGetUniformLocation(this->ID, name.c_str()), v1, v2, v3);
}

void Shader::SetVec3Float(const std::string &name, const Color &value) const {
    glUniform3f(glGetUniformLocation(this->ID, name.c_str()), value.r, value.g, value.b);
}

void Shader::SetVec3Float(const std::string &name, const Vector3 &value) const {
    glUniform3f(glGetUniformLocation(this->ID, name.c_str()), value.x, value.y, value.z);
}

void Shader::SetVec4Float(const std::string &name, float v1, float v2, float v3, float v4) const {
    glUniform4f(glGetUniformLocation(this->ID, name.c_str()), v1, v2, v3, v4);
}

void Shader::SetVec4Float(const std::string &name, const Color &value) const {
    glUniform4f(glGetUniformLocation(this->ID, name.c_str()), value.r, value.g, value.b, value.a);
}

void Shader::SetMatrix4Float(const std::string &name, const Matrix4 &mat) const {
    glUniformMatrix4fv(glGetUniformLocation(this->ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
}

bool Shader::IsShaderFilesValid(const std::string &vertexPath, const std::string &fragmentPath) {
    bool isValid = true;
    if (!FileHelper::DoesFileExist(vertexPath)) {
        isValid = false;
        logger->Error("Vertex file: %s doesn't exist!", vertexPath.c_str());
    }
    if (!FileHelper::DoesFileExist(fragmentPath)) {
        isValid = false;
        logger->Error("Vertex file: %s doesn't exist!", vertexPath.c_str());
    }
    return isValid;
}

void Shader::CheckCompileErrors(unsigned int shader, const std::string &type) {
    int success;
    char infoLog[1024];
    if(type == "PROGRAM") {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if(!success) {
            glGetProgramInfoLog(shader, 1024, nullptr, infoLog);
            logger->Error("Shader type '%s' linking failed!\n%s", type.c_str(), infoLog);
        }
    } else {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if(!success) {
            glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
            logger->Error("Shader type '%s' compilation failed!\n%s", type.c_str(), infoLog);
        }
    }
}

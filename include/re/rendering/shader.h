#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <glad/glad.h>

#include "./re/math/math.h"
#include "./color.h"
#include "../utils/logger.h"

struct OpenGLShaderSourceCode {
    std::string vertex;
    std::string fragment;
};

class Shader {
  private:
    unsigned int ID;
    Logger *logger = nullptr;

    bool IsShaderFilesValid(const std::string &vertexPath, const std::string &fragmentPath);

    void CheckCompileErrors(unsigned int shader, const std::string &type);

  public:
    Shader();

    Shader(const std::string &vertexPath, const std::string &fragmentPath);

    Shader(OpenGLShaderSourceCode openGlShaderSourceCode);

    ~Shader();

    OpenGLShaderSourceCode GetOpenGLShaderSourceFromPaths(const std::string &vertexPath, const std::string &fragmentPath);

    void Compile(OpenGLShaderSourceCode openGlShaderSourceCode);

    void Use();

    void SetBool(const std::string &name, bool value) const;

    void SetInt(const std::string &name, int value) const;

    void SetFloat(const std::string &name, float value) const;

    void SetVec2Float(const std::string &name, float v1, float v2) const;

    void SetVec2Float(const std::string &name, const Vector2 &value) const;

    void SetVec3Float(const std::string &name, const Color &value) const;

    void SetVec3Float(const std::string &name, const Vector3 &value) const;

    void SetVec3Float(const std::string &name, float v1, float v2, float v3) const;

    void SetVec4Float(const std::string &name, float v1, float v2, float v3, float v4) const;

    void SetVec4Float(const std::string &name, const Color &value) const;

    void SetMatrix4Float(const std::string &name, const Matrix4 &mat) const;
};

#endif //SHADER_H

#define GLFW_INCLUDE_NONE

#include "shader_program.h"

#include <glad/glad.h>

#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

namespace
{
    std::vector<char> ReadFile(const char* path)
    {
        std::ifstream file;

        file.open(path, std::ios::binary | std::ios::ate);
        const size_t file_size = file.tellg();
        file.seekg(0);

        std::vector<char> file_content;

        file_content.resize(file_size);
        file.read(file_content.data(), file_size);
        return file_content;
    }

    const char* GetShaderName(GLint shaderType)
    {
        switch (shaderType)
        {
        case GL_VERTEX_SHADER:
            return "VERTEX";
        case GL_FRAGMENT_SHADER:
            return "FRAGMENT";
        default:
            throw std::runtime_error("Unsupported shader.");
        }
    }
}

ShaderProgram::ShaderProgram() : id(0)
{}
void ShaderProgram::operator = (ShaderProgram&& rvl) noexcept
{
    id = rvl.id;
    rvl.id = -1;
}

ShaderProgram::~ShaderProgram()
{
    if (id != -1)
        glDeleteProgram(id);
}

void ShaderProgram::Use()
{
    glUseProgram(id);
}

void ShaderProgram::SetBool(const std::string& name, bool value) const
{
    glUniform1i(glGetUniformLocation(id, name.c_str()), static_cast<int>(value));
}

void ShaderProgram::SetInt(const std::string& name, int value) const
{
    glUniform1i(glGetUniformLocation(id, name.c_str()), value);
}

void ShaderProgram::SetFloat(const std::string& name, float value) const
{
    glUniform1f(glGetUniformLocation(id, name.c_str()), value);
}

void ShaderProgram::SetVec2f(const std::string& name, glm::vec2 vector) const
{
    glUniform2f(glGetUniformLocation(id, name.c_str()), vector.x, vector.y);
}

void ShaderProgram::SetVec3f(const std::string& name, glm::vec3 vector) const
{
    glUniform3f(glGetUniformLocation(id, name.c_str()), vector.x, vector.y, vector.z);
}

void ShaderProgram::SetVec4i(const std::string& name, glm::ivec4 vector) const
{
    glUniform4i(glGetUniformLocation(id, name.c_str()), vector.x, vector.y, vector.z, vector.w);
}

void ShaderProgram::SetVec4f(const std::string& name, glm::vec4 vector) const
{
    glUniform4f(glGetUniformLocation(id, name.c_str()), vector.x, vector.y, vector.z, vector.w);
}

void ShaderProgram::SetMatrix4f(const std::string& name, glm::mat4& matrix) const 
{
    glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, glm::value_ptr(matrix));
}

void ShaderProgram::LoadShader(const char* vertexPath, const char* fragmentPath)
{
    const GLuint vertex = CreateShader(vertexPath, GL_VERTEX_SHADER);
    const GLuint fragment = CreateShader(fragmentPath, GL_FRAGMENT_SHADER);

    id = glCreateProgram();
    glAttachShader(id, vertex);
    glAttachShader(id, fragment);
    glLinkProgram(id);
    CheckCompileErrors(id, "PROGRAM");

    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

void ShaderProgram::CheckCompileErrors(unsigned int shader, std::string type)
{
    int success;
    char infoLog[1024];
    const std::string baseError = std::string("ERROR:") + type;
    if (type != "PROGRAM")
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            const std::string error = baseError + std::string(":") + std::string(infoLog);
            throw std::runtime_error(error);
        }
    }
    else
    {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            const std::string error = baseError + std::string(":") + std::string(infoLog);
            throw std::runtime_error(error);
        }
    }
}

unsigned int ShaderProgram::CreateShader(const char* shaderPath, const int shaderType)
{
    const std::vector<GLchar> source(ReadFile(shaderPath));
    const char* shaderTypeName = GetShaderName(shaderType);
    const GLint id = glCreateShader(shaderType);
    const GLchar* shaderCode = source.data();
    const GLint shaderSize = source.size();

    glShaderSource(id, 1, &shaderCode, &shaderSize);
    glCompileShader(id);
    CheckCompileErrors(id, shaderTypeName);
    return id;
}

#pragma once

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>

class ShaderProgram
{
public:
    unsigned int id;

    ShaderProgram();

    void operator = (ShaderProgram&& rvl) noexcept;
    
    ShaderProgram(const ShaderProgram& shader) = delete;
    
    const ShaderProgram& operator = (const ShaderProgram& shader) = delete;
    
    ~ShaderProgram();
    
    void Use();
    void SetBool(const std::string& name, bool value) const;
    
    void SetInt(const std::string& name, int value) const;
    
    void SetFloat(const std::string& name, float value) const;

    void SetVec2f(const std::string& name, glm::vec2 vector) const;

    void SetVec3f(const std::string& name, glm::vec3 vector) const;

    void SetVec4i(const std::string& name, glm::ivec4 vector) const;

    void SetVec4f(const std::string& name, glm::vec4 vector) const;

    void SetMatrix4f(const std::string& name, glm::mat4& matrix) const;

    void LoadShader(const char* vertexPath, const char* fragmentPath);
    
private:
    void CheckCompileErrors(unsigned int shader, std::string type);
    
    unsigned int CreateShader(const char* shaderPath, const int shaderType);
};
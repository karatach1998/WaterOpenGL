
// C++
#include <fstream>
#include <iterator>

// Local
#include "shader.h"


auto Shader::loadSource(const char* path)
{
    std::ifstream fin(path);

    return std::string(std::istreambuf_iterator<char>(fin), 
                       std::istreambuf_iterator<char>());
}


Shader::Shader(const char* vertex_path, const char* fragment_path)
{
    auto vertex_src = loadSource(vertex_path);
    auto fragment_src = loadSource(fragment_path);

    const char* vertex_c_src = vertex_src.c_str();
    const char* fragment_c_src = fragment_src.c_str();

    unsigned vertex, fragment;

    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vertex_c_src, nullptr);
    glCompileShader(vertex);
    checkCompileErrors(vertex);

    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fragment_c_src, nullptr);
    glCompileShader(fragment);
    checkCompileErrors(fragment);

    m_id = glCreateProgram();
    glAttachShader(m_id, vertex);
    glAttachShader(m_id, fragment);
    glLinkProgram(m_id);
    checkLinkErrors(m_id);
    
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}


bool Shader::checkCompileErrors(GLuint shader)
{
    GLint success;
    GLchar info_log[1024];

    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(shader, 1024, nullptr, info_log);
        std::cout << "Error::Shader::Compilation failure.\n" << info_log << std::endl;
    }

    return success;
}


bool Shader::checkLinkErrors(GLuint program)
{
    GLint success;
    GLchar info_log[1024];

    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(program, 1024, nullptr, info_log);
        std::cout << "Error::Shader::Program error.\n" << info_log << std::endl;
    }

    return success;
}


int Shader::id() const
{
    return m_id;
}


void Shader::use() const
{
    glUseProgram(m_id);
}


void Shader::setBool(const std::string& name, bool value) const
{
    glUniform1i(glGetUniformLocation(m_id, name.c_str()), static_cast<int>(value));
}


void Shader::setInt(const std::string& name, int value) const
{
    glUniform1i(glGetUniformLocation(m_id, name.c_str()), value);
}


void Shader::setFloat(const std::string& name, float value) const
{
    glUniform1f(glGetUniformLocation(m_id, name.c_str()), value);
}


void Shader::setVec2(const std::string& name, const glm::vec2& value) const
{
    glUniform2fv(glGetUniformLocation(m_id, name.c_str()), 1, glm::value_ptr(value));
}


void Shader::setVec3(const std::string& name, const glm::vec3& value) const
{
    glUniform3fv(glGetUniformLocation(m_id, name.c_str()), 1, glm::value_ptr(value));
}


void Shader::setVec4(const std::string& name, const glm::vec4& value) const
{
    glUniform4fv(glGetUniformLocation(m_id, name.c_str()), 1, glm::value_ptr(value));
}


void Shader::setMat3(const std::string& name, const glm::mat3& value) const
{
    glUniformMatrix3fv(glGetUniformLocation(m_id, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}


void Shader::setMat4(const std::string& name, const glm::mat4& value) const
{
    glUniformMatrix4fv(glGetUniformLocation(m_id, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}

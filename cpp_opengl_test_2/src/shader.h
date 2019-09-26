#pragma once


class Shader
{
public:
    Shader() = default;
    Shader(const char* vertex_path, const char* fragment_path);

    int id() const;
    void use() const;

    void setBool(const std::string& name, bool value) const;
    void setInt(const std::string& name, int value) const;
    void setFloat(const std::string& name, float value) const;
    void setVec2(const std::string& naem, const glm::vec2& value) const;
    void setVec3(const std::string& name, const glm::vec3& value) const;
    void setVec4(const std::string& name, const glm::vec4& value) const;
    void setMat3(const std::string& name, const glm::mat3& value) const;
    void setMat4(const std::string& name, const glm::mat4& value) const;

private:
    static auto loadSource(const char* path);
    bool checkCompileErrors(GLuint shader);
    bool checkLinkErrors(GLuint program);

private:
    int m_id;
};

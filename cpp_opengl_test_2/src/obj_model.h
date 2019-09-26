#pragma once


// C++
#include <string>
#include <vector>
#include <map>


class ObjModel
{
public:    
    struct ObjLoader
    {
        static bool load(const char* obj_file_name, ObjModel& obj);

    private:
        static bool loadVertex(std::ifstream& fin, ObjModel& obj);
        static bool loadTexture(std::ifstream& fin, ObjModel& obj);
        static bool loadNormal(std::ifstream& fin, ObjModel& obj);
        static bool loadFace(std::ifstream& fin, ObjModel& obj);

        template <typename... Types>
        static inline bool try_invoke(const std::string& key, Types&... args);

        static inline void skip_whitespaces(std::istream& is);
        static inline void skip_line( std::istream& is);

    public:
        static std::map<std::string, bool(*)(std::ifstream&, ObjModel&)> m_proc;
    };

public:

    bool load(const char* obj_file_name);
    
public:
    struct VertexAttributes
    {
        unsigned vi; // vertex index
        unsigned vti; // texture index
        unsigned vni; // normal vectors
    };

    std::vector<glm::vec4> vertices;
    std::vector<glm::vec3> textures;
    std::vector<glm::vec3> normals;
    std::vector<std::vector<VertexAttributes>> faces;
};

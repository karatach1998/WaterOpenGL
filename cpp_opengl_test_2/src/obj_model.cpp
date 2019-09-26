
// C++
#include <cctype>
#include <fstream>

// Local
#include "obj_model.h"


// ObjModel::ObjLoader
// ==========================================================================

std::map<std::string, bool(*)(std::ifstream&, ObjModel&)> ObjModel::ObjLoader::m_proc = {
    { "v", ObjModel::ObjLoader::loadVertex },
    { "vt", ObjModel::ObjLoader::loadTexture },
    { "vn", ObjModel::ObjLoader::loadNormal },
    { "f", ObjModel::ObjLoader::loadFace }
};

template <typename... Types>
bool ObjModel::ObjLoader::try_invoke(const std::string& key, Types&... args)
{
    auto it = m_proc.find(key);
    return it == m_proc.end() || (it->second)(args...);
}

bool try_invoke(const std::string& key, std::ifstream& is, ObjModel& obj)
{
    auto it = ObjModel::ObjLoader::m_proc.find(key);
    return it == ObjModel::ObjLoader::m_proc.end() || (it->second)(is, obj);
}


void ObjModel::ObjLoader::skip_whitespaces(std::istream& is)
{
    while (is.good() && std::isblank(is.peek()))
        is.get();
}


void ObjModel::ObjLoader::skip_line(std::istream& is)
{
    is.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}


bool ObjModel::ObjLoader::load(const char* file_name, ObjModel& obj)
{
    struct DropAndRestoreIOStreamFlags
    {
        DropAndRestoreIOStreamFlags( std::ios_base& ib, std::ios_base::fmtflags flags )
            : m_ib(ib), m_backup(ib.setf(~flags)) { }

        ~DropAndRestoreIOStreamFlags()
        {
            m_ib.unsetf(m_backup);
        }

    private:
        std::ios_base& m_ib;
        std::ios_base::fmtflags m_backup;
    };

    using namespace std;

    ifstream obj_file(file_name);
    //DropAndRestoreIOStreamFlags(obj_file, std::ios_base::skipws);
    string s;

    if (!obj_file.is_open())
        return false;

    while (true)
    {
        obj_file >> s;

        if (obj_file.good() && try_invoke(s, obj_file, obj))
            // In this point we suuppose that all functions called by try_invoke to parse current string keep '\n' character in the current line and those suppose that line has correct format.
            skip_line(obj_file);
        else if (obj_file.eof())
            return true;
        else
            return false;
    }
}


bool ObjModel::ObjLoader::loadVertex(std::ifstream& fin, ObjModel& obj)
{
    glm::vec4 v;
    char ch;
    
    if (!(fin >> v[0] >> v[1] >> v[2]))
        return false;

    skip_whitespaces(fin);

    if (fin.peek() == '\n' || !(fin >> v[3]))
        v[3] = 1.0f;

    obj.vertices.push_back(v);
    return true;
}


bool ObjModel::ObjLoader::loadTexture(std::ifstream& fin, ObjModel& obj)
{
    glm::vec3 t;

    if (!(fin >> t[0] >> t[1]))
        return false;

    obj.textures.push_back(t);
    return true;
}


bool ObjModel::ObjLoader::loadNormal(std::ifstream& fin, ObjModel& obj)
{
    glm::vec3 v;

    if (!(fin >> v[0] >> v[1] >> v[2]))
        return false;

    obj.normals.push_back(v);
    return true;
}


bool ObjModel::ObjLoader::loadFace(std::ifstream& fin, ObjModel& obj)
{
    std::vector<ObjModel::VertexAttributes> face;

    while (true)
    {
        ObjModel::VertexAttributes va;
        char dummy, ch;


        if (!(fin >> va.vi))
            return false;

        if (fin.peek() != '/') {
            va.vti = va.vni = -1;
            goto next;
        }
        else if (fin.ignore() && fin.peek() == '/') {
            va.vti = -1;
        }
        else if (!(fin >> va.vti)) {
            return false;
        }

        if (fin.peek() != '/')
            va.vni = -1;
        else if (!fin.ignore() || !(fin >> va.vni))
            return false;
        
        next:
        face.push_back(va);
        skip_whitespaces(fin);
        if (fin.peek() == '\n')
            break;
    }

    obj.faces.push_back(face);
}


// ObjModel
// ==========================================================================

bool ObjModel::load(const char* file_name)
{
    return ObjLoader::load(file_name, *this);
}

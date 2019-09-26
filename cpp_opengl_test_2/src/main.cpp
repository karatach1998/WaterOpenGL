
// C++
#include <random>
#include <string>

// External
#include "../dependencies/stb_image.h"

// Local
#include "shader.h"
#include "input.h"
#include "camera.h"
#include "obj_model.h"
#include "wave.h"
#include "wave_generator.h"



#define MAX_PLANE_X_TESSELLATION 100
#define MAX_PLANE_Y_TESSELLATION 100
#define POOL_VERTEX_COUNT (6 * 5)
#define BOX_INDEX_COUNT (6 * 6)
#define LATERAL_WATER_PLANES_INDEX_COUNT (6 * 5)
#define WATER_LEVEL 0.2f


using namespace std::literals::string_literals;



// GLOBAL DATA
GLFWwindow* g_window;
int g_windowWidth;
int g_windowHeight;
std::shared_ptr<Input> g_pinput;
Camera g_camera{ glm::vec3(-1.5f, 3.0f, 3.5f), glm::vec3(0.0f, 1.0f, 0.0f), -35.0f, 21.0f };
std::unique_ptr<WaveGenerator> g_waveGenerator;

unsigned g_waterPlaneVAO;
unsigned g_boxVAO;
unsigned g_poolVAO;
unsigned g_poolTex;
unsigned g_lateralWaterPlanesVAO;
unsigned g_waterDuDvMap;
unsigned g_waterNormalMap;
unsigned g_reflectionFramebuffer;
unsigned g_reflectionTexture;
unsigned g_reflectionDepth;
unsigned g_refractionFramebuffer;
unsigned g_refractionTexture;
unsigned g_refractionDepth;

Shader g_waterShaderProgram;
Shader g_boxShaderProgram;
Shader g_poolShaderProgram;
Shader g_lateralWaterShaderProgram;

const glm::vec3 g_lightColor{ 1.0f, 1.0f, 0.9f };
const glm::vec3 g_lightPos{ -3.0f, 1.0f, 1.1f };
const glm::vec3 g_waterColor(0.65, 0.77, 0.82);

// ===========


// CALLBACKS

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    g_windowWidth = width;
    g_windowHeight = height;
    glViewport(0, 0, width, height);
}


// Major subroutines.


void load_pool()
{
    float vertices[] = {
        // far
        -1.0f, -1.0f, -1.0f,   0.0f,  0.0f,  1.0f,   0.0f,  0.0f,
        -1.0f,  1.0f, -1.0f,   0.0f,  0.0f,  1.0f,   0.0f,  1.0f,
         1.0f,  1.0f, -1.0f,   0.0f,  0.0f,  1.0f,   1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,   0.0f,  0.0f,  1.0f,   0.0f,  0.0f,
         1.0f,  1.0f, -1.0f,   0.0f,  0.0f,  1.0f,   1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,   0.0f,  0.0f,  1.0f,   1.0f,  0.0f,

        // left
        -1.0f, -1.0f,  1.0f,   1.0f,  0.0f,  0.0f,   0.0f,  0.0f,
        -1.0f,  1.0f,  1.0f,   1.0f,  0.0f,  0.0f,   0.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,   1.0f,  0.0f,  0.0f,   1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,   1.0f,  0.0f,  0.0f,   0.0f,  0.0f,
        -1.0f,  1.0f, -1.0f,   1.0f,  0.0f,  0.0f,   1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,   1.0f,  0.0f,  0.0f,   1.0f,  0.0f,

        // right
         1.0f, -1.0f, -1.0f,  -1.0f,  0.0f,  0.0f,   0.0f,  0.0f,
         1.0f,  1.0f, -1.0f,  -1.0f,  0.0f,  0.0f,   0.0f,  1.0f,
         1.0f,  1.0f,  1.0f,  -1.0f,  0.0f,  0.0f,   1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,  -1.0f,  0.0f,  0.0f,   0.0f,  0.0f,
         1.0f,  1.0f,  1.0f,  -1.0f,  0.0f,  0.0f,   1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,  -1.0f,  0.0f,  0.0f,   1.0f,  0.0f,

        // bottom
        -1.0f, -1.0f,  1.0f,   0.0f,  1.0f,  0.0f,   0.0f,  0.0f,
        -1.0f, -1.0f, -1.0f,   0.0f,  1.0f,  0.0f,   0.0f,  1.0f,
         1.0f, -1.0f, -1.0f,   0.0f,  1.0f,  0.0f,   1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,   0.0f,  1.0f,  0.0f,   0.0f,  0.0f,
         1.0f, -1.0f, -1.0f,   0.0f,  1.0f,  0.0f,   1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,   0.0f,  1.0f,  0.0f,   1.0f,  0.0f,

        // near
         1.0f, -1.0f,  1.0f,   0.0f,  0.0f, -1.0f,   0.0f,  0.0f,
         1.0f,  1.0f,  1.0f,   0.0f,  0.0f, -1.0f,   0.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,   0.0f,  0.0f, -1.0f,   1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,   0.0f,  0.0f, -1.0f,   0.0f,  0.0f,
        -1.0f,  1.0f,  1.0f,   0.0f,  0.0f, -1.0f,   1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,   0.0f,  0.0f, -1.0f,   1.0f,  0.0f,
    };

    unsigned VAO, VBO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);

    g_poolVAO = VAO;
}


void load_water_plane()
{
    std::unique_ptr<float[]> buffer(new float[2 * MAX_PLANE_X_TESSELLATION * MAX_PLANE_Y_TESSELLATION]);

    for (unsigned i = 0; i < MAX_PLANE_X_TESSELLATION; ++i) {
        for (unsigned j = 0; j < MAX_PLANE_Y_TESSELLATION; ++j) {
            buffer[0 + 2 * (j + i * MAX_PLANE_X_TESSELLATION)] = -1.0f + 2.0f / MAX_PLANE_X_TESSELLATION * i;
            buffer[1 + 2 * (j + i * MAX_PLANE_X_TESSELLATION)] =  1.0f - 2.0f / MAX_PLANE_Y_TESSELLATION * j;
        }
    }

    std::vector<unsigned> indices;

    for (unsigned i = 0; i < MAX_PLANE_X_TESSELLATION - 1; ++i) {
        for (unsigned j = 0; j < MAX_PLANE_Y_TESSELLATION - 1; ++j) {
            indices.push_back(j + i * MAX_PLANE_X_TESSELLATION);
            indices.push_back(j + 1 + (i + (i + j + 1) % 2) * MAX_PLANE_X_TESSELLATION);
            indices.push_back(j + (i + 1) * MAX_PLANE_X_TESSELLATION);
            indices.push_back(j + (i + (j + i) % 2) * MAX_PLANE_X_TESSELLATION);
            indices.push_back(j + 1 + i * MAX_PLANE_X_TESSELLATION);
            indices.push_back(j + 1 + (i + 1) * MAX_PLANE_X_TESSELLATION);
        }
    }
    
    unsigned VAO, VBO, EAO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EAO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, 2 * MAX_PLANE_X_TESSELLATION * MAX_PLANE_Y_TESSELLATION * sizeof(float), buffer.get(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EAO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned), indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    g_waterPlaneVAO = VAO;
}


void load_box()
{
    float vertices[] = {
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
    };

    float normals[] = {
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
    };

    unsigned indeces[BOX_INDEX_COUNT] = {
        // far
        0, 1, 2,
        0, 2, 3,
        // left
        4, 5, 1,
        4, 1, 0,
        // top
        5, 1, 2,
        5, 2, 6,
        // right
        3, 2, 6,
        3, 6, 7,
        // bottom
        4, 0, 3,
        4, 3, 7,
        // near
        4, 5, 6,
        4, 6, 7
    };

    unsigned VAO, VBO1, VBO2, EBO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO1);
    glGenBuffers(1, &VBO2);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO1);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, VBO2);
    glBufferData(GL_ARRAY_BUFFER, sizeof(normals), normals, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indeces), indeces, GL_STATIC_DRAW);

    glBindVertexArray(0);

    g_boxVAO = VAO;
}


void load_ball()
{
    ObjModel model;

    model.load("resources/models/ball.obj");

    std::size_t buffer_size = model.faces.size();// *;
    std::unique_ptr<float[]> buffer_ptr(new float[buffer_size]);

    unsigned VAO, VBO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, buffer_size, model.vertices.data(), GL_STATIC_DRAW);
    //glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, );
    //glEnableVertexAttribArray(0);
    //glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, );
    //glEnableVertexAttribArray(1);
    //glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, );
    glEnableVertexAttribArray(2);
    
    glBindVertexArray(0);
}


void load_lateral_water_planes()
{
    float vertices[] = {
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
    };

    unsigned indices[] = {
        // far
        0, 1, 2,
        0, 2, 3,
        // left 
        4, 5, 1,
        4, 1, 0,
        // right
        3, 2, 6,
        3, 6, 7,
        // bottom
        4, 0, 3,
        4, 3, 7,
        // near
        7, 6, 5,
        7, 5, 4,
    };

    unsigned VAO, VBO, EBO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glBindVertexArray(0);

    g_lateralWaterPlanesVAO = VAO;
}


unsigned load_texture(const char* file_path)
{
    unsigned texture;

    glGenTextures(1, &texture);

    glBindTexture(GL_TEXTURE_2D, texture); 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    int width, height, nr_channels;
    unsigned char* data;

    stbi_set_flip_vertically_on_load(true);
    data = stbi_load(file_path, &width, &height, &nr_channels, 0);
    if (data)
    {
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Error::Textures::File can't be loaded." << std::endl;
    }
    stbi_image_free(data);
    return texture;
}


void load_framebuffer(int width, int height, unsigned& framebuffer, unsigned& texture, unsigned& depth)
{
    // Creation of the framebuffer itself.
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glDrawBuffer(GL_COLOR_ATTACHMENT0);

    // The texture.
    glGenTextures(1, &texture);

    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, texture, 0);

    // The depth buffer.
    //glGenRenderbuffers(1, &depth);

    //glBindRenderbuffer(GL_RENDERBUFFER, depth);
    //glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
    //glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depth);

	GLenum draw_buffers[] = {GL_COLOR_ATTACHMENT0};
	glDrawBuffers(1, draw_buffers);
}


void load_reflection_framebuffer()
{
    load_framebuffer(g_windowWidth, g_windowHeight, g_reflectionFramebuffer, g_reflectionTexture, g_reflectionDepth);
}


void load_refraction_framebuffer()
{
    load_framebuffer(g_windowWidth, g_windowHeight, g_refractionFramebuffer, g_refractionTexture, g_refractionDepth);
}


bool init()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    g_window = glfwCreateWindow(800, 600, "My OpenGL Window", nullptr, nullptr);
    if (g_window == nullptr)
    {
        std::cout << "Failed to create window" << std::endl;
        glfwTerminate();
        return false;
    }
    glfwMakeContextCurrent(g_window);

    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
    {
        std::cout << "GLAD initialization failed." << std::endl;
        return false;
    }

    g_windowWidth = 800;
    g_windowHeight = 600;
    glViewport(0, 0, 800, 600);
    glfwSetInputMode(g_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glEnable(GL_DEPTH_TEST);

    g_pinput = Input::create(g_window);

    glfwSetFramebufferSizeCallback(g_window, framebuffer_size_callback);
    glfwSetKeyCallback(g_window, key_callback);
    glfwSetMouseButtonCallback(g_window, button_callback);
    glfwSetCursorPosCallback(g_window, mouse_position_callback);
    glfwSetScrollCallback(g_window, scroll_callback);

    g_waterShaderProgram = Shader("src/shaders/water.vs", "src/shaders/water.fs");
    g_boxShaderProgram = Shader("src/shaders/box.vs", "src/shaders/box.fs");
    g_poolShaderProgram = Shader("src/shaders/pool.vs", "src/shaders/pool.fs");
    g_lateralWaterShaderProgram = Shader("src/shaders/lateral_water_plane.vs", "src/shaders/lateral_water_plane.fs");
    
    load_pool();
    load_water_plane();
    load_box();
    load_lateral_water_planes();

    g_poolTex = load_texture("resources/textures/pool_side.jpg");
    g_waterDuDvMap = load_texture("resources/textures/waterDUDV.png");
    g_waterNormalMap = load_texture("resources/textures/normalMap.png");

    load_reflection_framebuffer();
    load_refraction_framebuffer();

    //g_waveGenerator.reset(new WaveGenerator());

    return true;
}


void process_input(GLFWwindow* window)
{
    if (g_pinput->isKeyPressed(GLFW_KEY_R))
    {
        g_waterShaderProgram = Shader("src/shaders/water.vs", "src/shaders/water.fs");
        g_boxShaderProgram = Shader("src/shaders/box.vs", "src/shaders/box.fs");
        g_poolShaderProgram = Shader("src/shaders/pool.vs", "src/shaders/pool.fs");
        g_lateralWaterShaderProgram = Shader("src/shaders/lateral_water_plane.vs", "src/shaders/lateral_water_plane.fs");
    }

    if (g_pinput->isKeyHolded(GLFW_KEY_M))
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    else
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }


    if (g_pinput->isButtonPressed(GLFW_MOUSE_BUTTON_LEFT))
    {

    }
    else
    {
        g_camera.processInput(*g_pinput);
    }

    if (g_pinput->isKeyPressed(GLFW_KEY_ESCAPE))
    {
        glfwSetWindowShouldClose(g_window, GL_TRUE);
    }
}


void render_scene(const glm::mat4& view, const glm::mat4& projection)
{
    // Pool drawing.

    glm::mat4 pool_model = glm::mat4(1.0f);

    g_poolShaderProgram.use();
    g_poolShaderProgram.setMat4("model", pool_model);
    g_poolShaderProgram.setMat4("view", view);
    g_poolShaderProgram.setMat4("projection", projection);
    g_poolShaderProgram.setVec3("lightColor", g_lightColor);
    g_poolShaderProgram.setVec3("lightPos", g_lightPos);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, g_poolTex);
    glBindVertexArray(g_poolVAO);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
    glDrawArrays(GL_TRIANGLES, 0, POOL_VERTEX_COUNT);
    glDisable(GL_CULL_FACE);

    // Ball drawing.

    //glm::mat4 ball_model = glm::mat4(1.0f);
    //ball_model = glm::translate(ball_model)

    //g_ballShaderProgram.use();
    //g_ballShaderProgram.setMat4("model", ball_model);
    //g_ballShaderProgram.setMat4("view", view);
    //g_ballShaderProgram.setMat4("projection", projection);
    //g_ballShaderProgram.setVec3("lightColor", g_lightColor);
    //g_ballShaderProgram.setVec3("lightPos", g_lightPos);
    //g_ballShaderProgram.setVec3("viewPos", g_camera.position());


    // Box drawing.

    glm::mat4 box_model = glm::mat4(1.0f);
    box_model = glm::translate(box_model, glm::vec3(0.0f, 0.5f, 0.0f));
    box_model = glm::scale(box_model, glm::vec3(0.15f));

    g_boxShaderProgram.use();
    g_boxShaderProgram.setMat4("model", box_model);
    g_boxShaderProgram.setMat4("view", view);
    g_boxShaderProgram.setMat4("projection", projection);
    g_boxShaderProgram.setVec3("lightColor", g_lightColor);
    g_boxShaderProgram.setVec3("lightPos", g_lightPos);

    glBindVertexArray(g_boxVAO);
    glDrawElements(GL_TRIANGLES, BOX_INDEX_COUNT, GL_UNSIGNED_INT, 0);
}


void render_water(const glm::mat4& view, const glm::mat4& projection)
{
    static Wave w1(0.5f / 1.0f, 0.03f, 0.01f, 0.02f, glm::vec2(0.3f, 0.7f));

    // Water drawing.

    glm::mat4 water_model = glm::mat4(1.0f);
    water_model = glm::translate(water_model, glm::vec3(0.0f, WATER_LEVEL, 0.0f));
    //water_model = glm::scale(water_model, glm::vec3(0.8f, 0.8f, 0.8f));

    g_waterShaderProgram.use();
    g_waterShaderProgram.setMat4("model", water_model);
    g_waterShaderProgram.setMat4("view", view);
    g_waterShaderProgram.setMat4("projection", projection);
    g_waterShaderProgram.setFloat("time", static_cast<float>(glfwGetTime()));
    g_waterShaderProgram.setVec3("waterColor", g_waterColor);
    g_waterShaderProgram.setVec3("lightColor", g_lightColor);
    g_waterShaderProgram.setVec3("lightPos", g_lightPos);
    g_waterShaderProgram.setVec3("viewPos", g_camera.position());
    g_waterShaderProgram.setInt("reflectionTexture", 0);
    g_waterShaderProgram.setInt("refractionTexture", 1);
    g_waterShaderProgram.setInt("dudvMap", 2);
    g_waterShaderProgram.setInt("normalMap", 3);
    //for (int i = 0; i < g_waveGenerator->waveCount(); ++i)
    //{
    //    const Wave& w = g_waveGenerator->get(i);

    //    g_waterShaderProgram.setFloat("waves[" + std::to_string(i) + "].Q", w.Q());
    //    g_waterShaderProgram.setFloat("waves[" + std::to_string(i) + "].A", w.A());
    //    g_waterShaderProgram.setFloat("waves[" + std::to_string(i) + "].w", w.w());
    //    g_waterShaderProgram.setFloat("waves[" + std::to_string(i) + "].fi", w.fi());
    //    g_waterShaderProgram.setVec2("waves[" + std::to_string(i) + "].D", w.D());
    //}
    //g_waterShaderProgram.setInt("actualWaveCount", g_waveGenerator->waveCount());
    g_waterShaderProgram.setFloat("waves[0].Q", w1.Q());
    g_waterShaderProgram.setFloat("waves[0].A", w1.A());
    g_waterShaderProgram.setFloat("waves[0].w", w1.w());
    g_waterShaderProgram.setFloat("waves[0].fi", w1.fi());
    g_waterShaderProgram.setVec2("waves[0].D", w1.D());
    g_waterShaderProgram.setInt("actualWaveCount", 1);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, g_reflectionTexture);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, g_refractionTexture);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, g_waterDuDvMap);
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, g_waterNormalMap);

    glBindVertexArray(g_waterPlaneVAO);
    glDrawElements(GL_TRIANGLES, (MAX_PLANE_X_TESSELLATION - 1) * (MAX_PLANE_Y_TESSELLATION - 1) * 6, GL_UNSIGNED_INT, 0);
}


void render_lateral_water_planes(const glm::mat4& view, const glm::mat4& projection)
{
    // Lateral water planes drawing.

    float height = 1.0f + WATER_LEVEL;
    glm::mat4 lateral_water_planes_model = glm::mat4(1.0f);
    lateral_water_planes_model = 
        glm::scale(lateral_water_planes_model, glm::vec3(1.0f, height / 2.0F, 1.0f));
    lateral_water_planes_model =
        glm::translate(lateral_water_planes_model, glm::vec3(0.0f, -height / 2.0f, 0.0f));

    g_lateralWaterShaderProgram.use();
    g_lateralWaterShaderProgram.setMat4("model", lateral_water_planes_model);
    g_lateralWaterShaderProgram.setMat4("view", view);
    g_lateralWaterShaderProgram.setMat4("projection", projection);
    g_lateralWaterShaderProgram.setVec3("waterColor", g_waterColor);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, g_refractionTexture);

    glBindVertexArray(g_lateralWaterPlanesVAO);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glDrawElements(GL_TRIANGLES, LATERAL_WATER_PLANES_INDEX_COUNT, GL_UNSIGNED_INT, 0);
    glDisable(GL_CULL_FACE);
}



int main()
{
    if (!init())
        return -1;


    while (!glfwWindowShouldClose(g_window))
    {
        // Global operations.

        process_input(g_window);

        glClearColor(0.15f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        const glm::mat4& view = g_camera.getViewMatrix();
        const glm::mat4& projection = glm::perspective(glm::radians(g_camera.zoom()), 800.0f / 600.0f, 0.001f, -100.0f);

        glBindFramebuffer(GL_FRAMEBUFFER, g_reflectionFramebuffer);
        glViewport(0, 0, g_windowWidth, g_windowHeight);
        render_scene(view, projection);
        glBindFramebuffer(GL_FRAMEBUFFER, g_refractionFramebuffer);
        glViewport(0, 0, g_windowWidth, g_windowHeight);
        render_scene(view, projection);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(0, 0, g_windowWidth, g_windowHeight);

        render_scene(view, projection);
        render_water(view, projection); 
        render_lateral_water_planes(view, projection);
        
        glfwSwapBuffers(g_window);
        glfwPollEvents();
    }
}

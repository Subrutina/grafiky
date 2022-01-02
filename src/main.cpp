#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <learnopengl/filesystem.h>
#include <learnopengl/shader.h>
#include <learnopengl/camera.h>
#include <learnopengl/model.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow *window, int width, int height);

void mouse_callback(GLFWwindow *window, double xpos, double ypos);

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);

void processInput(GLFWwindow *window);

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);


unsigned int loadTexture(char const * path);
unsigned int loadCubeMap(vector<std::string> faces);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
bool paused = false;

bool blinn = false;
bool blinnKeyPressed = false;


Camera camera(glm::vec3(0.0f, 0.0f, 50.0f));

glm::vec3 lightPos(0.0f, 2.0f, 13.0f);


float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;


int main() {
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "RG projekat", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetKeyCallback(window, key_callback);
    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // tell stb_image.h to flip loaded texture's on the y-axis (before loading model).
    stbi_set_flip_vertically_on_load(true);



    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);



    // build and compile shaders
    // -------------------------
    Shader baconShader("resources/shaders/bacon.vs", "resources/shaders/bacon.fs");
    Shader portalShader("resources/shaders/portal.vs", "resources/shaders/portal.fs");
    Shader pyramidShader("resources/shaders/pyramid.vs", "resources/shaders/pyramid.fs");
    Shader lightSourceCubeShader("resources/shaders/lightCube.vs", "resources/shaders/lightCube.fs");
    Shader skyBoxShader("resources/shaders/skyBox.vs", "resources/shaders/skyBox.fs");
    Model SDModel("resources/objects/finiii/Finn.obj");
    Shader SDShader("resources/shaders/2.model_lighting.vs", "resources/shaders/2.model_lighting.fs");
    //kocka:
    float vertices[] = {

            -0.5f, -0.5f, -0.5f,
            0.5f,  0.5f, -0.5f,
            0.5f, -0.5f, -0.5f,
            0.5f,  0.5f, -0.5f,
            -0.5f, -0.5f, -0.5f,
            -0.5f,  0.5f, -0.5f,

            -0.5f, -0.5f,  0.5f,
            0.5f, -0.5f,  0.5f,
            0.5f,  0.5f,  0.5f,
            0.5f,  0.5f,  0.5f,
            -0.5f,  0.5f,  0.5f,
            -0.5f, -0.5f,  0.5f,

            -0.5f,  0.5f,  0.5f,
            -0.5f,  0.5f, -0.5f,
            -0.5f, -0.5f, -0.5f,
            -0.5f, -0.5f, -0.5f,
            -0.5f, -0.5f,  0.5f,
            -0.5f,  0.5f,  0.5f,

            0.5f,  0.5f,  0.5f,
            0.5f, -0.5f, -0.5f,
            0.5f,  0.5f, -0.5f,
            0.5f, -0.5f, -0.5f,
            0.5f,  0.5f,  0.5f,
            0.5f, -0.5f,  0.5f,

            -0.5f, -0.5f, -0.5f,
            0.5f, -0.5f, -0.5f,
            0.5f, -0.5f,  0.5f,
            0.5f, -0.5f,  0.5f,
            -0.5f, -0.5f,  0.5f,
            -0.5f, -0.5f, -0.5f,

            -0.5f,  0.5f, -0.5f,
            0.5f,  0.5f,  0.5f,
            0.5f,  0.5f, -0.5f,
            0.5f,  0.5f,  0.5f,
            -0.5f,  0.5f, -0.5f,
            -0.5f,  0.5f,  0.5f,
    };

    //waffle piramida:
    float waffleVertices[] = {
            //position          //texture   //normals
            0.0f,  0.0f,  0.5f, 0.5f, 1.0f, 0.0f, 1.0f, 0.5f,
            0.5f,  0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.5f,
            -0.5f,  0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.5f,

            0.0f,  0.0f,  0.5f, 0.5f, 1.0f, -1.0f, 0.0f, 0.5f,
            -0.5f,  0.5f, -0.5f, 1.0f, 0.0f, -1.0f, 0.0f, 0.5f,
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.5f,

            0.0f,  0.0f,  0.5f, 0.5f, 1.0f, 0.0f, -1.0f, 0.5f,
            -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, -1.0f, 0.5f,
            0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, -1.0f, 0.5f,

            0.0f,  0.0f,  0.5f, 0.5f, 1.0f, 1.0f, 0.0f, 0.5f,
            0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.5f,
            0.5f,  0.5f, -0.5f, 1.0f, 0.0f, 1.0f, 0.0f, 0.5f,


            0.5f,  0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f,
            -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f,
            -0.5f,  0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f,
            -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f,
            0.5f,  0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f,
            0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, -1.0f

    };

    float BaconVertices[] = {
            //position          //texture
            0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

    float portalVertices[] = {
            // positions         // texture Coords (swapped y coordinates because texture is flipped upside down)
            0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
            0.0f, -0.5f,  0.0f,  0.0f,  1.0f,
            1.0f, -0.5f,  0.0f,  1.0f,  1.0f,

            0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
            1.0f, -0.5f,  0.0f,  1.0f,  1.0f,
            1.0f,  0.5f,  0.0f,  1.0f,  0.0f
    };

    unsigned int indices[] = {
            0, 1, 3,  // first Triangle
            1, 2, 3   // second Triangle
    };

    float skyboxVertices[] = {
            // positions
            -1.0f,  1.0f, -1.0f,
            -1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,

            -1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,

            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,

            -1.0f, -1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,

            -1.0f,  1.0f, -1.0f,
            1.0f,  1.0f, -1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f, -1.0f,

            -1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
            1.0f, -1.0f,  1.0f
    };
    //svetlo:
    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(VAO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);


    //Waffle Piramida:

    unsigned int WaffleVBO, WaffleVAO;

    glGenVertexArrays(1, &WaffleVAO);
    glGenBuffers(1, &WaffleVBO);

    glBindVertexArray(WaffleVAO);

    glBindBuffer(GL_ARRAY_BUFFER, WaffleVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(waffleVertices), waffleVertices, GL_STATIC_DRAW);

    glBindVertexArray(WaffleVAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
    glEnableVertexAttribArray(2);


    //bacon avenue:
    unsigned int BaconVAO, BaconVBO, BaconEBO;
    glGenVertexArrays(1, &BaconVAO);
    glGenBuffers(1, &BaconVBO);
    glGenBuffers(1, &BaconEBO);

    glBindVertexArray(BaconVAO);

    glBindBuffer(GL_ARRAY_BUFFER, BaconVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(BaconVertices), BaconVertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, BaconEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    //portal
    unsigned int portalVAO, portalVBO;
    glGenVertexArrays(1, &portalVAO);
    glGenBuffers(1, &portalVBO);
    glBindVertexArray(portalVAO);
    glBindBuffer(GL_ARRAY_BUFFER, portalVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(portalVertices), portalVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glBindVertexArray(0);


    // skybox VAO
    unsigned int skyboxVAO, skyboxVBO;
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);


    //teksture:

    stbi_set_flip_vertically_on_load(false);

    unsigned int texture1 = loadTexture(FileSystem::getPath("resources/textures/waffle.jpg").c_str());
    unsigned int texture2 = loadTexture(FileSystem::getPath("resources/textures/kiki.jpg").c_str());
    unsigned int texture3 = loadTexture(FileSystem::getPath("resources/textures/wafflee.jpg").c_str());
    unsigned int portalTexture = loadTexture(FileSystem::getPath("resources/textures/portal.png").c_str());
    unsigned int finnTexture = loadTexture(FileSystem::getPath("resources/objects/finiii/Finn.png").c_str());
    vector<std::string> faces
            {
                    FileSystem::getPath("resources/textures/SkyBox/px.jpg"),
                    FileSystem::getPath("resources/textures/SkyBox/nx.jpg"),
                    FileSystem::getPath("resources/textures/SkyBox/py.jpg"),
                    FileSystem::getPath("resources/textures/SkyBox/ny.jpg"),
                    FileSystem::getPath("resources/textures/SkyBox/pz.jpg"),
                    FileSystem::getPath("resources/textures/SkyBox/nz.jpg")
            };
    unsigned int cubemapTexture = loadCubeMap(faces);

    // don't forget to enable shader before setting uniforms



    pyramidShader.use();
    pyramidShader.setInt("material.diffuse", 0);
    pyramidShader.setInt("material.specular", 2);

    skyBoxShader.use();
    skyBoxShader.setInt("skybox", 0);

    baconShader.use();
    baconShader.setInt("texture2", 1);

    SDShader.use();
    SDShader.setInt("material.texture_diffuse1", 3);

    portalShader.use();
    portalShader.setInt("portalTexture", 4);


    // render loop
    // -----------
    while (!glfwWindowShouldClose(window)) {
        // per-frame time logic
        // --------------------
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window);


        // render
        // ------
        glClearColor(0.0f, 0.0f, 1.0f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        // view/projection transformations
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();



        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, texture3);

        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, finnTexture);

        glActiveTexture(GL_TEXTURE4);
        glBindTexture(GL_TEXTURE_2D, portalTexture);



        pyramidShader.use();
        //direkciono:
        pyramidShader.setVec3("dirLight.direction", 0.0f, 0.0f, -0.3f);
        pyramidShader.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
        pyramidShader.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
        pyramidShader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);

        //spotlight:
        pyramidShader.setVec3("spotLight.position", camera.Position);
        pyramidShader.setVec3("spotLight.direction", camera.Front);
        pyramidShader.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
        pyramidShader.setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
        pyramidShader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
        pyramidShader.setFloat("spotLight.constant", 1.0f);
        pyramidShader.setFloat("spotLight.linear", 0.09);
        pyramidShader.setFloat("spotLight.quadratic", 0.032);
        pyramidShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
        pyramidShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));

        // material properties
        pyramidShader.setBool("blinn", blinn);
        pyramidShader.setFloat("material.shininess", 8.0f);

        //piramida1
        pyramidShader.use();
        pyramidShader.setMat4("projection", projection);
        pyramidShader.setMat4("view", view);

        glm::mat4 modelW = glm::mat4(1.0f);
        modelW = glm::translate(modelW, glm::vec3(0.0f, 0.0f, -3.0f));
        modelW = glm::rotate(modelW, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        modelW = glm::rotate(modelW, glm::radians(90.0f), glm::vec3(-1.0f, 0.0f, 0.0f));
        modelW = glm::scale(modelW, glm::vec3(6.0f));


        pyramidShader.setMat4("model", modelW);

        glBindVertexArray(WaffleVAO);
        glDrawArrays(GL_TRIANGLES, 0, 18);

        //piramida2:
        modelW = glm::mat4(1.0f);
        modelW = glm::translate(modelW, glm::vec3(7.0f, 0.0f, -6.0f));
        modelW = glm::rotate(modelW, glm::radians(90.0f), glm::vec3(-1.0f, 0.0f, 0.0f));
        modelW = glm::rotate(modelW, float(sin(glfwGetTime())), glm::vec3(0.0f, 0.0f, -1.0f));
        modelW = glm::scale(modelW, glm::vec3(4.0f));

        pyramidShader.setMat4("model", modelW);

        glBindVertexArray(WaffleVAO);
        glDrawArrays(GL_TRIANGLES, 0, 18);

        //piramida3:
        modelW = glm::mat4(1.0f);
        modelW = glm::translate(modelW, glm::vec3(-7.0f, 0.0f, -6.0f));
        modelW = glm::rotate(modelW, glm::radians(90.0f), glm::vec3(-1.0f, 0.0f, 0.0f));
        modelW = glm::rotate(modelW, float(sin(glfwGetTime())), glm::vec3(0.0f, 0.0f, 1.0f));
        modelW = glm::scale(modelW, glm::vec3(4.0f));
        pyramidShader.setMat4("model", modelW);
        glBindVertexArray(WaffleVAO);
        glDrawArrays(GL_TRIANGLES, 0, 18);


        //osvetljenje modela:
        modelW = glm::mat4(1.0f);
        modelW = glm::translate(modelW, lightPos);
        lightSourceCubeShader.use();
        lightSourceCubeShader.setVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));
        lightSourceCubeShader.setMat4("model", modelW);
        lightSourceCubeShader.setMat4("view", view);
        lightSourceCubeShader.setMat4("projection", projection);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);


        //bacon:
        baconShader.use();
        baconShader.setMat4("projection", projection);
        baconShader.setMat4("view", view);

        glBindVertexArray(BaconVAO);
        glDisable(GL_CULL_FACE);

        glm::mat4 modelB = glm::mat4(1.0f);
        modelB = glm::translate(modelB, glm::vec3(0.0f, -4.5f, 2.0f));
        modelB = glm::rotate(modelB, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        modelB = glm::rotate(modelB, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        modelB = glm::scale(modelB, glm::vec3(100.0f, 6.0f, 3.0f));

        baconShader.setMat4("model", modelB);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glEnable(GL_CULL_FACE);

        //portali:
        glBindVertexArray(portalVAO);
        glDisable(GL_CULL_FACE);
        portalShader.use();
        portalShader.setMat4("projection", projection);
        portalShader.setMat4("view", view);
        glm::mat4 modelP = glm::mat4(1.0f);
        modelP = glm::translate(modelP, glm::vec3(-5.0f, -1.0f, 52.0f));
        modelP = glm::scale(modelP, glm::vec3(10, 5, 2));
        portalShader.setMat4("model", modelP);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glEnable(GL_CULL_FACE);






        //skybox
        glDepthMask(GL_FALSE);
        glDepthFunc(GL_LEQUAL);
        skyBoxShader.use();
        skyBoxShader.setMat4("view", glm::mat4(glm::mat3(view)));
        skyBoxShader.setMat4("projection", projection);
        glBindVertexArray(skyboxVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glDepthMask(GL_TRUE);
        glDepthFunc(GL_LESS);


        //Finn:
        SDShader.use();

        SDShader.setVec3("viewPos", camera.Position);
        SDShader.setVec3("pointLight.position", lightPos);
        SDShader.setVec3("pointLight.ambient", glm::vec3(0.2f));
        SDShader.setVec3("pointLight.diffuse", glm::vec3(1.0f));

        SDShader.setFloat("pointLight.constant", 1.0f);
        SDShader.setFloat("pointLight.linear", 0.09f);
        SDShader.setFloat("pointLight.quadratic", 0.032f);

        SDShader.setMat4("projection", projection);
        SDShader.setMat4("view", view);

        glm::vec3 pos;
        pos.x = sin(glfwGetTime()) ;
        pos.y = cos(glfwGetTime()) - 2;
        pos.z = 10.0f;
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, pos);

        model = glm::scale(model, glm::vec3(0.05f));
        SDShader.setMat4("model", model);
        SDModel.Draw(SDShader);


        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }


    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS && !blinnKeyPressed)
    {
        blinn = !blinn;
        blinnKeyPressed = true;
    }
    if (glfwGetKey(window, GLFW_KEY_B) == GLFW_RELEASE)
    {
        blinnKeyPressed = false;
    }

}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow *window, double xpos, double ypos) {
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;


    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
    camera.ProcessMouseScroll(yoffset);
}
unsigned int loadTexture(char const * path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}

unsigned int loadCubeMap(vector<std::string> faces){
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID); //obratiti paznju kako se aktivira ovo, nije GL_TEXTURE_2D

    int width, height, nrChannels;
    unsigned char* data;

    for(unsigned int i = 0; i < faces.size(); i++){
        data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if(data){
            //u redosledu kojem smo naveli teksture sada se mapiraju na cubeMap
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0,
                         GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        }
        else{
            std::cerr << "Failed to load cube map texture\n";
            return -1;
        }

        stbi_image_free(data);
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);


    return textureID;

}



void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS){
        paused = !paused;
    }
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS){
        glfwSetWindowShouldClose(window, true);
    }
}

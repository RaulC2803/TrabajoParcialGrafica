#include "readFile.hpp"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);



const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;


Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = (float)SCR_WIDTH / 2.0;
float lastY = (float)SCR_HEIGHT / 2.0;
bool firstMouse = true;


float deltaTime = 0.0f;
float lastFrame = 0.0f;


int main() {

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif


    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }


    glEnable(GL_DEPTH_TEST);

    Shader shader("GLSL/codigo.vs", "GLSL/codigo.fs");
    Shader shader2("GLSL/codigo.vs", "GLSL/codigo.fs");
    Shader shader3("GLSL/codigo.vs", "GLSL/codigo.fs");
    Shader shader4("GLSL/codigo.vs", "GLSL/codigo.fs");
    Shader simpleDepthShader("GLSL/shadowmappingdepth.vs", "GLSL/shadowmappingdepth.fs");
    Shader lightshader("GLSL/light.vs", "GLSL/light.fs");



    Figure* sun = new Figure();
    Figure* cube = new Figure();
    Figure* plane = new Figure();
    Model* limon = new Model("Models/limon/10187_Lime.obj");




    read("OFF/esfera.off", sun, false, false);
    read("OFF/cuboNormalesTexturas.off", cube, true, true);
    read("OFF/plane.off", plane, true, true);


    // load textures
    // -------------
    unsigned int bricktexture = loadTexture("Textures/brickwall.jpg");
    unsigned int containerTexture = loadTexture("Models/sandia/watermelon.jpg");
    unsigned int cubeTextureDiffuse = loadTexture("Textures/container2.png");
    unsigned int cubeTextureSpecular = loadTexture("Textures/container2_specular.png");



    sun->registerFigure();
    cube->registerFigure();
    plane->registerFigure();


    //cargar el mapa de profundidad
    DepthMap* depthmap = new DepthMap();
    depthmap->registerDepth(1024, 1024);



    shader.use();
    shader.setInt("diffuseTexture", 0);
    shader.setInt("shadowMap", 1);
    shader4.use();
    shader4.setInt("diffuseTexture", 0);
    shader4.setInt("specularTexture", 1);
    shader4.setInt("shadowMap", 2);

    glm::vec3 lightPos(-2.0f, 6.0f, 1.0f);
    glm::vec3 lightColor(1.0);


    while (!glfwWindowShouldClose(window)) {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        glm::mat4 lightProjection, lightView;
        glm::mat4 lightSpaceMatrix;
        float near_plane = 1.0f, far_plane = 8.5f;
        lightProjection = glm::ortho(-20.0f, 20.0f, -20.0f, 20.0f, near_plane, far_plane);
        lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
        lightSpaceMatrix = lightProjection * lightView;
        simpleDepthShader.use();
        simpleDepthShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);
        simpleDepthShader.setVec3("lightcolor", lightColor);
        simpleDepthShader.setVec3("lightPos", lightPos);

        glViewport(0, 0, depthmap->resolution.first, depthmap->resolution.second);
        glBindFramebuffer(GL_FRAMEBUFFER, depthmap->FBO);
        glClear(GL_DEPTH_BUFFER_BIT);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, bricktexture);
        glm::mat4 model = glm::mat4(1.0f);
        simpleDepthShader.setMat4("model", model);
        plane->drawFigure();

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(sin(glfwGetTime()) / 2.0, 4.0f, sin(glfwGetTime())));

        model = glm::scale(model, glm::vec3(0.45f));
        simpleDepthShader.setMat4("model", model);
        limon->Draw(simpleDepthShader);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0));
        simpleDepthShader.setMat4("model", model);
        cube->drawFigure();
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-1.0f, 1.0f, 1.0));
        model = glm::rotate(model, (float)glfwGetTime(), glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
        simpleDepthShader.setMat4("model", model);
        cube->drawFigure();
        glBindFramebuffer(GL_FRAMEBUFFER, 0);


        glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        //Piso
        shader.use();
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        model = glm::mat4(1.0f);
        shader.setMat4("model", model);
        shader.setMat4("projection", projection);
        shader.setMat4("view", view);
        shader.setVec3("viewPos", camera.Position);
        shader.setVec3("lightPos", lightPos);
        shader.setVec3("lightcolor", lightColor);
        shader.setMat4("lightSpaceMatrix", lightSpaceMatrix);
        shader.setVec3("specularTexture", 0.5f, 0.5f, 0.5f);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, bricktexture);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, depthmap->depthTexture);
        plane->drawFigure();


        //Limon
        shader2.use();
        shader2.setMat4("projection", projection);
        shader2.setMat4("view", view);
        shader2.setVec3("viewPos", camera.Position);
        shader2.setVec3("lightPos", lightPos);
        shader2.setVec3("lightcolor", lightColor);
        shader2.setMat4("lightSpaceMatrix", lightSpaceMatrix);
        shader2.setVec3("specularTexture", 0.5f, 0.5f, 0.5f);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(sin(glfwGetTime()), 4.0f, sin(glfwGetTime())));
        model = glm::scale(model, glm::vec3(0.45f));
        shader2.setMat4("model", model);
        limon->Draw(shader2);



        //Cajas
        shader4.use();
        shader4.setMat4("projection", projection);
        shader4.setMat4("view", view);
        shader4.setVec3("viewPos", camera.Position);
        shader4.setVec3("lightPos", lightPos);
        shader4.setVec3("lightcolor", lightColor);
        shader4.setMat4("lightSpaceMatrix", lightSpaceMatrix);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0));

        shader4.setMat4("model", model);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, cubeTextureDiffuse);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, cubeTextureSpecular);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, depthmap->depthTexture);
        cube->drawFigure();
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-1.0f, 1.0f, 1.0));
        model = glm::rotate(model, (float)glfwGetTime(), glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
        shader4.setMat4("model", model);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, cubeTextureDiffuse);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, cubeTextureSpecular);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, depthmap->depthTexture);
        cube->drawFigure();

        //Sol
        lightshader.use();
        lightshader.setMat4("projection", projection);
        lightshader.setMat4("view", view);
        lightshader.setVec3("viewPos", camera.Position);
        lightshader.setVec3("color", lightColor);
        model = glm::mat4(1.0f);
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(2.0f));
        lightshader.setMat4("model", model);
        sun->drawFigure();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    delete cube;
    delete plane;
    delete sun;
    delete limon;

    glfwTerminate();
    return 0;
}




void processInput(GLFWwindow* window) {
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
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height) {

    glViewport(0, 0, width, height);
}



void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}


void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    camera.ProcessMouseScroll(yoffset);
}
#include "readFile.hpp"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
void renderLeafs(int x, int z, Figure* figure, Shader shader);


const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;


Camera camera(glm::vec3(0.0f, 5.0f, 3.0f));
float lastX = (float)SCR_WIDTH / 2.0;
float lastY = (float)SCR_HEIGHT / 2.0;
bool firstMouse = true;


float deltaTime = 0.0f;
float lastFrame = 0.0f;
bool night = false;
bool day = true;
bool activeParticles = true;

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
    Shader shader4("GLSL/codigo.vs", "GLSL/codigo.fs");
    Shader simpleDepthShader("GLSL/shadowmappingdepth.vs", "GLSL/shadowmappingdepth.fs");
    Shader lightshader("GLSL/light.vs", "GLSL/light.fs");
    Shader particle("GLSL/particle.vs", "GLSL/particle.fs");
    Shader shaderSkybox("GLSL/skybox.vs", "GLSL/skybox.fs");
    Shader shaderLava("GLSL/light2.vs", "GLSL/light2.fs");
    Shader shaderWood("GLSL/codigo.vs", "GLSL/codigo.fs");
    Shader shaderCortex("GLSL/codigo.vs", "GLSL/codigo.fs");
    Shader shaderLeaf("GLSL/codigo.vs", "GLSL/codigo.fs");
    Shader shaderCloud("GLSL/codigo.vs", "GLSL/codigo.fs");
    Shader particle2("GLSL/particle.vs", "GLSL/particle.fs");
    Shader shadercubeMetal("GLSL/codigo.vs", "GLSL/codigo.fs");



    Figure* sun = new Figure();
    Figure* cube = new Figure();
    Figure* cubelava = new Figure();
    Figure* plane = new Figure();
    Figure* skybox = new Figure();
    Figure* cubewood = new Figure();
    Figure* cubecortex = new Figure();
    Figure* cubeleaf = new Figure();
    Figure* cubecloud = new Figure();
    Figure* cubemetal = new Figure();
    Model* limon = new Model("Models/thenvsnow/thenvsnow.obj");

    



    read("OFF/esfera.off", sun, false, false);
    read("OFF/cuboNormalesTexturas.off", cube, true, true);
    read("OFF/cubotex.off", cubelava, true, false);
    read("OFF/plane.off", plane, true, true);
    read("OFF/skybox.off", skybox, false, false);
    read("OFF/cuboNormalesTexturas.off", cubewood, true, true);
    read("OFF/cuboNormalesTexturas.off", cubecortex, true, true);
    read("OFF/cuboNormalesTexturas.off", cubeleaf, true, true);
    read("OFF/cuboNormalesTexturas.off", cubecloud, true, true);
    read("OFF/cuboNormalesTexturas.off", cubemetal, true, true);

    // load textures
    // -------------

    vector<string> faces;
    faces.push_back("Textures/Skybox/DaylightBox_Pieces/DaylightBox_Right.jpg");
    faces.push_back("Textures/Skybox/DaylightBox_Pieces/DaylightBox_Left.jpg");
    faces.push_back("Textures/Skybox/DaylightBox_Pieces/DaylightBox_Top.jpg");
    faces.push_back("Textures/Skybox/DaylightBox_Pieces/DaylightBox_Bottom.jpg");
    faces.push_back("Textures/Skybox/DaylightBox_Pieces/DaylightBox_Front.jpg");
    faces.push_back("Textures/Skybox/DaylightBox_Pieces/DaylightBox_Back.jpg");
    vector<string> faces2;
    faces2.push_back("Textures/Skybox/kurt/space_ft.png");
    faces2.push_back("Textures/Skybox/kurt/space_bk.png");
    faces2.push_back("Textures/Skybox/kurt/space_up.png");
    faces2.push_back("Textures/Skybox/kurt/space_dn.png");
    faces2.push_back("Textures/Skybox/kurt/space_rt.png");
    faces2.push_back("Textures/Skybox/kurt/space_lf.png");
    
    unsigned int cubemapTexture = loadCubemap(faces);
    unsigned int cubemapTexture2 = loadCubemap(faces2);
    unsigned int bricktexture = loadTexture("Textures/cesped.jpg");
    unsigned int containerTexture = loadTexture("Models/steve-obj/Steve.png");
    unsigned int cubeTextureDiffuse = loadTexture("Textures/cesped.jpg");
    unsigned int cubeTextureSpecular = loadTexture("Textures/cesped.jpg");
    unsigned int particleTexture = loadTexture("Textures/cesped.jpg");
    unsigned int cubelavaTexture = loadTexture("Textures/lava.jpg");
    unsigned int cubewoodTexture = loadTexture("Textures/wood.png");
    unsigned int cubecortexTexture = loadTexture("Textures/corteza.jpg");
    unsigned int cubeleafTexture = loadTexture("Textures/hojas.jpg");
    unsigned int cubecloudTexture = loadTexture("Textures/nube.jpg");
    unsigned int particle2Texture = loadTexture("Textures/agua.jpg");
    unsigned int cubemetalTexure = loadTexture("Textures/piedra.png");
    unsigned int cubemetalTexure2 = loadTexture("Textures/maderametal2.png");
    unsigned int negro = loadTexture("Textures/negro.png");



    sun->registerFigure();
    cube->registerFigure();
    plane->registerFigure();
    skybox->registerFigure();
    cubelava->registerFigure();
    cubewood->registerFigure();
    cubecortex->registerFigure();
    cubeleaf->registerFigure();
    cubecloud->registerFigure();
    cubemetal->registerFigure();


    //cargar el mapa de profundidad
    DepthMap* depthmap = new DepthMap();
    depthmap->registerDepth(1024, 1024);

    ParticleGenerator* particles = new ParticleGenerator(particle, particleTexture, 1000);
    ParticleGenerator* particles2 = new ParticleGenerator(particle2, particle2Texture, 1000);


    shader.use();
    shader.setInt("diffuseTexture", 0);
    shader.setInt("specularTexture", 1);
    shader.setInt("shadowMap", 2);
    shader2.use();
    shader2.setInt("shadowMap", 1);
    shadercubeMetal.use();
    shadercubeMetal.setInt("diffuseTexture", 0);
    shadercubeMetal.setInt("specularTexture", 1);
    shadercubeMetal.setInt("shadowMap", 2);
    shader4.use();
    shader4.setInt("diffuseTexture", 0);
    shader4.setInt("specularTexture", 1);
    shader4.setInt("shadowMap", 2);
    shaderSkybox.use();
    shaderSkybox.setInt("skybox", 0);
    shaderLava.use();
    shaderLava.setInt("texture1", 0);
    shaderWood.use();
    shaderWood.setInt("diffuseTexture", 0);
    shaderWood.setInt("shadowMap", 1);
    shaderCortex.use();
    shaderCortex.setInt("diffuseTexture", 0);
    shaderCortex.setInt("shadowMap", 1);
    shaderLeaf.use();
    shaderLeaf.setInt("diffuseTexture", 0);
    shaderLeaf.setInt("specularTexture", 1);
    shaderLeaf.setInt("shadowMap", 2);
    shaderCloud.use();
    shaderCloud.setInt("diffuseTexture", 0);
  


    glm::vec3 lightPos(1.0, 15.0f, 20.0f);
    glm::vec3 lightColor(1.0);


    while (!glfwWindowShouldClose(window)) {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        float z = (sin(glfwGetTime() / 15) * 20);
        lightPos = glm::vec3(1.0, 15.0,z);
        if (night)
            lightColor = glm::vec3(0.2, 0.2, 1.0);
        else
            lightColor = glm::vec3(1.0, 1.0, 1.0);
        cout << night;
        
        // do shadowmap painting stuff here
        glCullFace(GL_FRONT);
       
        glm::mat4 lightProjection, lightView;
        glm::mat4 lightSpaceMatrix;
        float near_plane = 1.0f, far_plane = 50.5f;
        lightProjection = glm::ortho(-45.0f, 45.0f, -45.0f, 45.0f, near_plane, far_plane);
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
        for (int i = -20; i < 20; i++) {
            for (int j = -20; j < 20; j++) {
                if (!(i >= -18 && i <= -12 && j >= -18 && j <= -12)) {
                    model = glm::mat4(1.0f);
                    model = glm::translate(model, glm::vec3(i, 0.0f, j));
                    simpleDepthShader.setMat4("model", model);
                    cube->drawFigure();
                }
                
            }
           
        }
        //Izquierda
        for (int i = 0; i <= 4; i++) {
            for (int j = 0; j <= 9; j++) {
                if (!(i > 1 && i <= 3 && j >= 2 && j <= 7)) {
                    model = glm::mat4(1.0f);
                    model = glm::translate(model, glm::vec3(-7.0, i, j));
                    simpleDepthShader.setMat4("model", model);
                    cubewood->drawFigure();
                }
            }

        }
        //Derecha
        for (int i = 0; i <= 4; i++) {
            for (int j = 0; j <= 9; j++) {
                model = glm::mat4(1.0f);
                model = glm::translate(model, glm::vec3(-16.0, i, j));

                simpleDepthShader.setMat4("model", model);
                cubewood->drawFigure();
            }
        }
        //Adelante
        for (int i = 0; i <= 4; i++) {
            for (int j = -15; j <= -8; j++) {
                if (!(i >= 0 && i <= 2 && j >= -13 && j < -11)) {
                    model = glm::mat4(1.0f);
                    model = glm::translate(model, glm::vec3(j, i, -1.0));
                    simpleDepthShader.setMat4("model", model);
                    cubewood->drawFigure();
                }
            }
        }
        //Atrás
        for (int i = 0; i <= 4; i++) {
            for (int j = -15; j <= -8; j++) {
                model = glm::mat4(1.0f);
                model = glm::translate(model, glm::vec3(j, i, 10.0));
                simpleDepthShader.setMat4("model", model);
                cubewood->drawFigure();
            }
        }
        //Techo
        for (int i = -15; i <= -8; i++) {
            for (int j = 0; j <= 9; j++) {
                model = glm::mat4(1.0f);
                model = glm::translate(model, glm::vec3(i, 5.0, j));
                simpleDepthShader.setMat4("model", model);
                cubewood->drawFigure();
            }
        }
        //Troncos
        for (int z = 1; z <= 3; z++) {
            if (z % 2 == 0) {
                for (int i = -18; i <= 18; i++) {
                    for (int j = 1; j <= 5; j++) {
                        if ((i + 1) % 6 == 0) {
                            model = glm::mat4(1.0f);
                            model = glm::translate(model, glm::vec3(21.0 - (z * 7), j, i));
                            simpleDepthShader.setMat4("model", model);
                            cubecortex->drawFigure();
                        }

                    }

                }
            }
            else {
                for (int i = -18; i <= 18; i++) {
                    for (int j = 1; j <= 5; j++) {
                        if ((i + 3) % 6 == 0) {
                            model = glm::mat4(1.0f);
                            model = glm::translate(model, glm::vec3(21.0 - (z * 7), j, i - 1));
                            simpleDepthShader.setMat4("model", model);
                            cubecortex->drawFigure();
                        }

                    }

                }
            }
        }

        //leafs
        for (int z = 1; z <= 3; z++) {
            if (z % 2 == 0) {
                for (int i = -18; i <= 18; i++) {

                    if ((i + 1) % 6 == 0) {
                        renderLeafs(21.0 - (z * 7), i, cubeleaf, simpleDepthShader);
                    }



                }
            }
            else {
                for (int i = -18; i <= 18; i++) {

                    if ((i + 3) % 6 == 0) {
                        renderLeafs(21.0 - (z * 7), i - 1, cubeleaf, simpleDepthShader);
                    }



                }
            }
        }

        //nube
        for (int i = -4; i <= 4; i++) {
            for (int j = -4; j <= 4; j++) {
                if (!((i == -4 && j == 4) || (i == 4 && j == -4) || (i == 4 && j == 4) || (i == -4 && j == -4))) {
                    model = glm::mat4(1.0f);
                    model = glm::translate(model, glm::vec3(6.0 - i, 10.0, (sin(glfwGetTime() / 10) * 20) - j));
                    simpleDepthShader.setMat4("model", model);
                    cubecloud->drawFigure();
                }

            }
        }

        //cheems
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-15.0, 0.50f, -8.0));
        model = glm::scale(model, glm::vec3(0.7));
        simpleDepthShader.setMat4("model", model);
        limon->Draw(shader2);

        //metal
        for (int i = 0; i <= 4; i++) {
            model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(-7.0, i, -1.0));
            simpleDepthShader.setMat4("model", model);
            cubemetal->drawFigure();
        }
        for (int i = 0; i <= 4; i++) {
            model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(-16.0, i, -1.0));
            simpleDepthShader.setMat4("model", model);
            cubemetal->drawFigure();
        }
        for (int i = 0; i <= 4; i++) {
            model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(-7.0, i, 10.0));
            simpleDepthShader.setMat4("model", model);
            cubemetal->drawFigure();
        }
        for (int i = 0; i <= 4; i++) {
            model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(-16.0, i, 10.0));
            simpleDepthShader.setMat4("model", model);
            cubemetal->drawFigure();
        }

        //model = glm::mat4(1.0f);
        //model = glm::translate(model, glm::vec3(1.0, 2.0f, 5.0));
        //simpleDepthShader.setMat4("model", model);
        //limon->Draw(simpleDepthShader);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glCullFace(GL_BACK);

        


        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        model = glm::mat4(1.0f);
        if (activeParticles) {
            particles->Update(deltaTime, 1, glm::vec3(-15.0f, 0.7, -15.0f), 2.0, glm::vec3(0.0, 0.5, 0.0), true, glm::vec4(0.0, 0.0, 0.0, 0.0), 300);
            particles->Draw(projection, view, model);

            particles2->Update(deltaTime, 4, glm::vec3(6.0, 10.0, (sin(glfwGetTime() / 10) * 20)), 2.0, glm::vec3(0.0, -6.0, 0.0), false, glm::vec4(0.0, 0.6, 0.9, 1.0), 400);
            particles2->Draw(projection, view, model);
        }
        


        


        //Piso
        shader.use();
        projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        model = glm::mat4(1.0f);
        shader.setMat4("model", model);
        shader.setMat4("projection", projection);
        shader.setMat4("view", view);
        shader.setVec3("viewPos", camera.Position);
        shader.setVec3("lightPos", lightPos);
        shader.setVec3("lightcolor", lightColor);
        shader.setMat4("lightSpaceMatrix", lightSpaceMatrix);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, bricktexture);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, negro);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, depthmap->depthTexture);
        plane->drawFigure();



        //cheems
        shader2.use();
        shader2.setMat4("projection", projection);
        shader2.setMat4("view", view);
        shader2.setVec3("viewPos", camera.Position);
        shader2.setVec3("lightPos", lightPos);
        shader2.setVec3("lightcolor", lightColor);
        shader2.setMat4("lightSpaceMatrix", lightSpaceMatrix);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, depthmap->depthTexture);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-15.0, 0.50f, -8.0));
        model = glm::scale(model, glm::vec3(0.7));
        shader2.setMat4("model", model);
        limon->Draw(shader2);


        //Cajas
        shader4.use();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, cubeTextureDiffuse);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, negro);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, depthmap->depthTexture);
        shader4.setMat4("projection", projection);
        shader4.setMat4("view", view);
        shader4.setVec3("viewPos", camera.Position);
        shader4.setVec3("lightPos", lightPos);
        shader4.setVec3("lightcolor", lightColor);
        shader4.setMat4("lightSpaceMatrix", lightSpaceMatrix);
        for (int i = -20; i < 20; i++) {
            for (int j = -20; j < 20; j++) {
                if (!(i >= -18 && i <= -12 && j >= -18 && j <= -12)) {
                    model = glm::mat4(1.0f);
                    model = glm::translate(model, glm::vec3(i, 0.0f, j));
                    shader4.setMat4("model", model);
                    cube->drawFigure();
                }

            }
            
        }


        //Lava
        shaderLava.use();
        shaderLava.setMat4("projection", projection);
        shaderLava.setMat4("view", view);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, cubelavaTexture);
        
        for (int i = -18; i <= -12; i++) {
            for (int j = -18; j <= -12; j++) {
                model = glm::mat4(1.0f);
                model = glm::translate(model, glm::vec3(i, 0.0f, j));
                model = glm::scale(model, glm::vec3(1.0, 0.8, 1.0));
                shaderLava.setMat4("model", model);
                cubelava->drawFigure();
            }
            
        }
        
        //Madera
        shaderWood.use();
        shaderWood.setMat4("projection", projection);
        shaderWood.setMat4("view", view);
        shaderWood.setVec3("viewPos", camera.Position);
        shaderWood.setVec3("lightPos", lightPos);
        shaderWood.setVec3("lightcolor", lightColor);
        shaderWood.setMat4("lightSpaceMatrix", lightSpaceMatrix);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, cubewoodTexture);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, depthmap->depthTexture);
        //Izquierda
        for (int i = 0; i <= 4; i++) {
            for (int j = 0; j <= 9; j++) {
                if (!(i > 1 && i <= 3 && j >= 2 && j <= 7)) {
                    model = glm::mat4(1.0f);
                    model = glm::translate(model, glm::vec3(-7.0, i, j));

                    shaderWood.setMat4("model", model);
                    cubewood->drawFigure();
                }
            }

        }
        //Derecha
        for (int i = 0; i <= 4; i++) {
            for (int j = 0; j <= 9; j++) {
                model = glm::mat4(1.0f);
                model = glm::translate(model, glm::vec3(-16.0, i, j));

                shaderWood.setMat4("model", model);
                cubewood->drawFigure();
            }
        }
        //Adelante
        for (int i = 0; i <= 4; i++) {
            for (int j = -15; j <= -8; j++) {
                if (!(i >= 0 && i <= 2 && j >= -13 && j < -11)) {
                    model = glm::mat4(1.0f);
                    model = glm::translate(model, glm::vec3(j, i, -1.0));

                    shaderWood.setMat4("model", model);
                    cubewood->drawFigure();
                }
            }
        }
        //Atrás
        for (int i = 0; i <= 4; i++) {
            for (int j = -15; j <= -8; j++) {
                model = glm::mat4(1.0f);
                model = glm::translate(model, glm::vec3(j, i, 10.0));
                shaderWood.setMat4("model", model);
                cubewood->drawFigure();
            }
        }
        //Techo
        for (int i = -15; i <= -8; i++) {
            for (int j = 0; j <= 9; j++) {
                model = glm::mat4(1.0f);
                model = glm::translate(model, glm::vec3(i, 5.0, j));
                shaderWood.setMat4("model", model);
                cubewood->drawFigure();
            }
        }

        //Troncos
        shaderCortex.use();
        shaderCortex.setMat4("projection", projection);
        shaderCortex.setMat4("view", view);
        shaderCortex.setVec3("viewPos", camera.Position);
        shaderCortex.setVec3("lightPos", lightPos);
        shaderCortex.setVec3("lightcolor", lightColor);
        shaderCortex.setMat4("lightSpaceMatrix", lightSpaceMatrix);
        shaderCortex.setVec3("specularTexture", 1.0f, 1.0f, 1.0f);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, cubecortexTexture);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, depthmap->depthTexture);
        for (int z = 1; z <= 3; z++) {
            if (z % 2 == 0) {
                for (int i = -18; i <= 18; i++) {
                    for (int j = 1; j <= 5; j++) {
                        if ((i + 1) % 6 == 0) {
                            model = glm::mat4(1.0f);
                            model = glm::translate(model, glm::vec3(21.0 - (z*7), j, i));
                            shaderCortex.setMat4("model", model);
                            cubecortex->drawFigure();
                        }

                    }

                }
            }
            else {
                for (int i = -18; i <= 18; i++) {
                    for (int j = 1; j <= 5; j++) {
                        if ((i + 3) % 6 == 0) {
                            model = glm::mat4(1.0f);
                            model = glm::translate(model, glm::vec3(21.0 -(z*7), j, i - 1));
                            shaderCortex.setMat4("model", model);
                            cubecortex->drawFigure();
                        }

                    }

                }
            }
        }
       

        //hojas
        shaderLeaf.use();
        shaderLeaf.setMat4("projection", projection);
        shaderLeaf.setMat4("view", view);
        shaderLeaf.setVec3("viewPos", camera.Position);
        shaderLeaf.setVec3("lightPos", lightPos);
        shaderLeaf.setVec3("lightcolor", lightColor);
        shaderLeaf.setMat4("lightSpaceMatrix", lightSpaceMatrix);
        shaderLeaf.setVec3("specularTexture", 1.0f, 1.0f, 1.0f);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, cubeleafTexture);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, negro);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, depthmap->depthTexture);
        
        for (int z = 1; z <= 3; z++) {
            if (z % 2 == 0) {
                for (int i = -18; i <= 18; i++) {
                    
                        if ((i + 1) % 6 == 0) {
                            renderLeafs(21.0 - (z * 7), i, cubeleaf, shaderLeaf);
                        }

                 

                }
            }
            else {
                for (int i = -18; i <= 18; i++) {
                   
                        if ((i + 3) % 6 == 0) {
                            renderLeafs(21.0 - (z * 7), i - 1, cubeleaf, shaderLeaf);
                        }

                 

                }
            }
        }

        //Nube
        shaderCloud.use();
        shaderCloud.setMat4("projection", projection);
        shaderCloud.setMat4("view", view);
        shaderCloud.setVec3("viewPos", camera.Position);
        shaderCloud.setVec3("lightPos", lightPos);
        shaderCloud.setVec3("lightcolor", lightColor);
        shaderCloud.setMat4("lightSpaceMatrix", lightSpaceMatrix);
        shaderCloud.setVec3("specularTexture", 1.0f, 1.0f, 1.0f);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, cubecloudTexture);
        for (int i = -4; i <= 4; i++) {
            for (int j = -4; j <= 4; j++) {
                if (!((i == -4 && j == 4) || (i == 4 && j == -4) || (i == 4 && j == 4) || (i == -4 && j == -4))) {
                    model = glm::mat4(1.0f);
                    model = glm::translate(model, glm::vec3(6.0 - i, 10.0, (sin(glfwGetTime() / 10) * 20) - j));
                    shaderCloud.setMat4("model", model);
                    cubecloud->drawFigure();
                }
                
            }
        }
        

        //Metal
        shadercubeMetal.use();
        shadercubeMetal.setMat4("projection", projection);
        shadercubeMetal.setMat4("view", view);
        shadercubeMetal.setVec3("viewPos", camera.Position);
        shadercubeMetal.setVec3("lightPos", lightPos);
        shadercubeMetal.setVec3("lightcolor", lightColor);
        shadercubeMetal.setMat4("lightSpaceMatrix", lightSpaceMatrix);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, cubemetalTexure);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, cubemetalTexure2);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, depthmap->depthTexture);
        for(int i = 0; i <= 4; i++){
            model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(-7.0, i, -1.0));
            shadercubeMetal.setMat4("model", model);
            cubemetal->drawFigure();
        }
        for (int i = 0; i <= 4; i++) {
            model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(-16.0, i, -1.0));
            shadercubeMetal.setMat4("model", model);
            cubemetal->drawFigure();
        }
        for (int i = 0; i <= 4; i++) {
            model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(-7.0, i, 10.0));
            shadercubeMetal.setMat4("model", model);
            cubemetal->drawFigure();
        }
        for (int i = 0; i <= 4; i++) {
            model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(-16.0, i, 10.0));
            shadercubeMetal.setMat4("model", model);
            cubemetal->drawFigure();
        }
    

        //Sol
        lightshader.use();
        lightshader.setMat4("projection", projection);
        lightshader.setMat4("view", view);
        model = glm::mat4(1.0f);
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(2.0f));
        lightshader.setMat4("model", model);
        sun->drawFigure();
        


        glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
        shaderSkybox.use();
        view = glm::mat4(glm::mat3(camera.GetViewMatrix())); // remove translation from the view matrix
        shaderSkybox.setMat4("view", view);
        shaderSkybox.setMat4("projection", projection);
        // skybox cube
        skybox->drawFigure();
        if (night) {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture2);
        }
        else {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
        }
            
        
        glDepthFunc(GL_LESS);

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

void renderLeafs(int x, int z, Figure* figure, Shader shader) {
    for (int k = 3; k <= 7; k++) {
        for (int i = -2; i <= 2; i++) {
            for (int j = -2; j <= 2; j++) {
                if (k == 3 || k == 4) {
                    glm::mat4 model = glm::mat4(1.0f);
                    model = glm::translate(model, glm::vec3(x + i, k, z + j));
                    shader.setMat4("model", model);
                    figure->drawFigure();
                }
                if (k == 5 && j > -2 && j < 2 && i > -2 && i < 2) {
                    glm::mat4 model = glm::mat4(1.0f);
                    model = glm::translate(model, glm::vec3(x + i, k, z + j));
                    shader.setMat4("model", model);
                    figure->drawFigure();
                }
                if (k == 6 && ((j == 0 && i > -2 && i < 2) || (i == 0 && j > -2 && j < 2))) {
                    glm::mat4 model = glm::mat4(1.0f);
                    model = glm::translate(model, glm::vec3(x + i, k, z + j));
                    shader.setMat4("model", model);
                    figure->drawFigure();
                }
                
            }

        }
    }
    
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
    if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS) 
        night = true;
    if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS ) 
        night = false;
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
        activeParticles = true;
    if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
        activeParticles = false;
    
      
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
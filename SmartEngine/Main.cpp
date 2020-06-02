/*
*Course work 
*3rd year student Ovchinnikov Maxim
*/

#define _CRT_SECURE_NO_WARNINGS
// GLEW GLFW
#define GLEW32_DLL
#include <GL/glew.h>
#define GLFW_DLL
#include <GLFW/glfw3.h>
#include <stdio.h>
// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <bullet/btBulletDynamicsCommon.h>
// Classes
#include "log.h"
//#include "Model.h"
#include "Lobby.h"
//#include "Player.h"
#include "Bulletcallback.h"
#include "keycallback.h"
#include "Light.h"
#include "GameObject.h"
#include "SkyBox.h"
#include "DepthMap.h"
// IMGUI
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#define WIDTH 640
#define HEIGHT 480
#define FPS 30

GLfloat lastX = WIDTH;
GLfloat lastY = HEIGHT;
GLfloat yaw = -90.0f;
GLfloat pitch = 0.0f;
int count = 0;

int main() {

    float points[]{
        0.0f, 0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f
    };
    float colours[] = {
        1.0f, 0.0f,  0.0f,
        0.0f, 1.0f,  0.0f,
        0.0f, 0.0f,  1.0f
    };
    std::vector<bulletObject*> bodies;

    gl_log("starting GLFW\n%s\n", glfwGetVersionString());
    glfwSetErrorCallback(glfw_error_callback);
    // start GL context and O/S window using the GLFW helper library
    if (!glfwInit()) {
        fprintf(stderr, "ERROR: could not start GLFW3\n");
        return 1;
    }

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "The Runners", NULL, NULL);
    if (!window) {
        fprintf(stderr, "ERROR: could not open window with GLFW3\n");
        glfwTerminate();
        return 1;
    }
    glfwMakeContextCurrent(window);

    // start GLEW extension handler
    glewExperimental = GL_TRUE;
    glewInit();
    log_gl_param();
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    //glfwSetWindowSizeCallback(window, window_size_callback);
    Lobby main_lobby;
    Player player(glm::vec3(0.0f, 0.0f, 0.3f),window);
    main_lobby.add_player(&player);
    //player.setupdayekey();
    //player.setupdatemouse();
    // get version info
    const GLubyte* renderer = glGetString(GL_RENDERER); // get renderer string
    const GLubyte* version = glGetString(GL_VERSION); // version as a string
    printf("Renderer: %s\n", renderer);
    printf("OpenGL version supported %s\n", version);

    // tell GL to only draw onto a pixel if the shape is closer to the viewer
    glEnable(GL_DEPTH_TEST); // enable depth-testing
    glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"
    glfwWindowHint(GLFW_SAMPLES, 4);
    
    // IMGUI CONTEXT SETUP
    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    //IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    //ImGuiIO& io = ImGui::GetIO(); (void)io;
    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    const char* glsl_version = "#version 330";
    ImGui_ImplOpenGL3_Init(glsl_version);
    glClearColor(0.6f, 0.6f, 0.8f, 1.0f);
    // Load Shaders
    Shader shader("triangle.vert", "triangle.frag");
    Shader nanos("model.vert", "model.frag");
    Shader cubes("cube.vert", "cube.frag");
    Shader slight("lightcube.vert", "lightcube.frag");
    Shader skyboxShader("cubeMap.vert", "cubeMap.frag");
    Shader depthShader("depthShader.vert", "depthShader.frag");
    ShaderLoader* anim = new ShaderLoader();
    anim->loadShaders("anim.vert", "anim.frag");
    // Load Models and AnimModels
    Model nanosuit((char*)"models/nanosuit/nanosuit.obj");
    Model m4((char*)"models/guns/m4/m4.obj");
    //Model wall((char*)"models/fallingwall/swall.dae");
    Light light1((char*)"models/cube/cube.obj");
    Model level((char*)"models/gamelevels/basiclevel.obj");
    Model cube((char*)"models/acube/cube.obj");
    GameObject* object = new GameObject(); //create model 
    // DEPTHMAP LOAD
    DepthMap depthmap;
    cubes.Use();
    cubes.setInt("diffuseTexture", 0);
    cubes.setInt("shadowMap", 1);
    // SKYBOX LOAD--------
    SkyBox skybox(std::vector<std::string>({ 
        "skybox/right.jpg","skybox/left.jpg",
        "skybox/top.jpg","skybox/bottom.jpg",
        "skybox/front.jpg","skybox/back.jpg"}));
    //-------------------
    /*for (int i = 0; i < cube.meshes[0].vertices.size(); i++)
    {
        std::cout << "x: " << cube.meshes[0].vertices[i].Position.x << " y: " << cube.meshes[0].vertices[i].Position.y << " z: " << cube.meshes[0].vertices[i].Position.z << std::endl;
    }*/
    // Faster render
    //glEnable(GL_CULL_FACE);
    //glCullFace(GL_BACK);
    //glFrontFace(GL_CW);

    glm::vec3 position = glm::vec3(0.3f, 0.5f, -1.0f);
    glm::quat orientation = glm::normalize(glm::quat(glm::vec3(rand() % 360, rand() % 360, rand() % 360)));

    // Start initialize Bullet
    btBroadphaseInterface* broadphase = new btDbvtBroadphase();

    btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();
    btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfiguration);

    btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver;
    btDiscreteDynamicsWorld* dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher,broadphase,solver,collisionConfiguration);
    dynamicsWorld->setGravity(btVector3(0, -9.81f, 0));

    std::vector<btRigidBody*> rigidbodies;

    btCollisionShape* boxCollisionShape = new btBoxShape(btVector3(1.0f, 1.0f, 1.0f));
    btScalar mass(1.f);
    bool isDynamic = (mass != 0.f);
    btVector3 localIntertia(1, 0, 0);
    if (isDynamic)
        boxCollisionShape->calculateLocalInertia(mass, localIntertia);
    gContactAddedCallback = callbackFunc;
    btTransform t;
    t.setIdentity();
    t.setOrigin(btVector3(0,0,0));
    btStaticPlaneShape* plane = new btStaticPlaneShape(btVector3(0, 1, 0),0);
    btMotionState* motion = new btDefaultMotionState(t);
    btRigidBody::btRigidBodyConstructionInfo info(0, motion, plane);
    // ADD FIRST RIGID BODY
    btRigidBody* body = new btRigidBody(info);
    unsigned int pomodel = addmodel(&level, dynamicsWorld,&bodies);
    double lastTime = glfwGetTime();
    int nbFrames = 0;

    //btRigidBody* lightc = addBox(2.f, 2.f, 2.f, player.getCameraPos().x, player.getCameraPos().y, player.getCameraPos().z, 1.f, dynamicsWorld);
    glm::vec3 look = player.getCameraLook();
    //lightc->setLinearVelocity(btVector3(look.x * 20, look.y * 20, look.z * 20));
    //lightc->setCollisionFlags(lightc->getCollisionFlags() | btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);
    //bodies.push_back(new bulletObject(lightc, 1, 1.0, 0.0, 0.0));
    // ADD ANIMATION MODEL
    object->createGraphicsObject("models/basicmodels/m4.fbx"); //get data from file
    //object->applyLocalRotation(180, vec3(1, 0, 0)); //there are some problems with loading fbx files. Models could be rotated or scaled. So we rotate it to the normal state
    object->playAnimation(new Animation("anim", vec2(0, 1147), 0.60, 10, true)); //forcing our model to play the animation (name, frames, speed, priority, loop)

    lastTime = glfwGetTime();
    float linearveloc = 20.0f;
    // ADD Main Player Model
    {
        btRigidBody* cube2 = addBox(17.196674f, 100.196674f, 17.196674f, 0.f, 30.f, 2.f, 1.f, dynamicsWorld);
        cube2->forceActivationState(DISABLE_DEACTIVATION);
        glm::vec3 look = player.getCameraLook();
        cube2->setCollisionFlags(cube2->getCollisionFlags() | btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);
        bulletObject* cubee = new bulletObject(cube2, bodies.size(), 1.0, 0.0, 0.0);
        cubee->body->setAngularFactor(0.f);
        cubee->type = 0;
        player.model = cubee;
        bodies.push_back(cubee);
        cube2->setUserPointer(bodies[bodies.size() - 1]);
        player.id_in_lobby_arr = main_lobby.find_playerid_by_obj_id(player.model->id);
        player.dynamicsWorld = dynamicsWorld;
        main_lobby.set_update_mouse(player.id_in_lobby_arr);
    }
    float dummies[] = { 100.f,100.f,10.f,
                        100.f,100.f,30.f,
                        300.f,100.f,10.f,
                        -100.f,100.f,10.f };
    // Add some dummies to level
    for (int i = 0; i < 4; i++)
    {
        btRigidBody* cube2 = addBox(17.196674f, 60.196674f, 17.196674f, dummies[i * 3], dummies[i * 3 + 1], dummies[i * 3 + 2], 10.f, dynamicsWorld);
        glm::vec3 look = player.getCameraLook();
        //cube2->setLinearVelocity(btVector3(look.x * linearveloc, look.y * linearveloc, look.z * linearveloc));
        //cube2->setCollisionFlags(cube2->getCollisionFlags() | btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);
        bulletObject* cubee = new bulletObject(cube2, bodies.size(), 1.0, 0.0, 0.0);
        cubee->type = 1;
        bodies.push_back(cubee);
        cube2->setUserPointer(bodies[bodies.size() - 1]);
    }
    while (!glfwWindowShouldClose(window))
    {
        int km = 0;
        double currentTime = glfwGetTime();
        nbFrames++;
        float deltaTime = currentTime - lastTime;
        // Change player time for maximize smooth movement
        player.elapsedtime = (currentTime - lastTime)/1;

        lastTime = currentTime;
        dynamicsWorld->stepSimulation(1.f / 10.f, 4);
        // Update Keyboard and Camera
        main_lobby.update_key(player.id_in_lobby_arr);
        player.updateCamPos();
        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        //----------------SHADOW MAP
        glm::mat4 lightProjection, lightView;
        glm::mat4 lightSpaceMatrix;
        float near_plane = 1.0f, far_plane = 1000.0f;
        lightProjection = glm::ortho(-1000.0f, 1000.0f, -1000.0f, 1000.0f, near_plane, far_plane);
        lightView = glm::lookAt(depthmap.lightPos, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        lightSpaceMatrix = lightProjection * lightView;

        depthShader.Use();
        depthShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);

        depthmap.bind();
        glCullFace(GL_FRONT);
        for (int i = 0; i < bodies.size(); i++)
        {
            if (bodies[i]->body->getCollisionShape()->getShapeType() == BOX_SHAPE_PROXYTYPE)
            {
                btRigidBody* sphere = bodies[i]->body;
                btVector3 extent = ((btBoxShape*)sphere->getCollisionShape())->getHalfExtentsWithoutMargin();
                btTransform t;
                sphere->getMotionState()->getWorldTransform(t);
                float mat[16];
                t.getOpenGLMatrix(mat);
                glm::mat4 trans = glm::mat4(1.0f);
                trans = glm::make_mat4(mat);
                depthShader.setMat4("model", trans);
                depthShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);

                nanosuit.Draw(depthShader);


            }
            if (bodies[i]->body->getCollisionShape()->getShapeType() == CONVEX_HULL_SHAPE_PROXYTYPE)
            {
                btRigidBody* sphere = bodies[i]->body;
                btVector3 extent = ((btBoxShape*)sphere->getCollisionShape())->getHalfExtentsWithoutMargin();
                btTransform t;
                sphere->getMotionState()->getWorldTransform(t);
                float mat[16];
                t.getOpenGLMatrix(mat);
                glm::mat4 trans = glm::mat4(1.0f);
                trans = glm::make_mat4(mat);
                depthShader.setMat4("model", trans);
                depthShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);
                level.meshes[pomodel + km].Draw(depthShader);
                km++;
            }
        }
        glCullFace(GL_BACK);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        // reset viewport
        glViewport(0, 0, WIDTH, HEIGHT);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
       
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        shader.Use();
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        {
            btRigidBody* cube2 = addBox(17.196674f, 50.196674f, 17.196674f, player.getCameraPos().x, player.getCameraPos().y, player.getCameraPos().z, 10000000.f, dynamicsWorld);
            glm::vec3 look = player.getCameraLook();
            cube2->setLinearVelocity(btVector3(look.x * linearveloc, look.y * linearveloc, look.z * linearveloc));
            cube2->setCollisionFlags(cube2->getCollisionFlags() | btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);
            bulletObject *cubee = new bulletObject(cube2, bodies.size(), 1.0, 0.0, 0.0);
            bodies.push_back(cubee);
            cube2->setUserPointer(bodies[bodies.size()-1]);
        }
        // ON/OFF Cursor for debug menu
        if (glfwGetKey(window, GLFW_KEY_9) == GLFW_PRESS)
        {
            if (player.cursor == true)
                player.cursor = false;
            else
                player.cursor = true;
        }
        if(player.cursor == true)
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        else
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        km = 0;
        /*
         *In this cycle we draw every body in collision world
         *Every body has a shapetype
         *CONVEX HULL shape - ALWAYS map
         *BOX shape - 3D models
         *SPHERE shape - idk...sphere
         *STATIC PLANE - low level map
        */
        for (int i = 0; i < bodies.size(); i++)
        {
            // Second object is lamp. This need to rewrite
            if (i == 1)
            {
                slight.Use();
                slight.setVec3("objectcolour", glm::vec3(1.0, 0.0, 0.0));
                slight.setVec3("lightcolour", glm::vec3(1.0, 1.0, 1.0));
                //cube.Draw(bodies[1], slight, &player);
            }
            if (bodies[i]->body->getCollisionShape()->getShapeType() == SPHERE_SHAPE_PROXYTYPE)
                renderSphere(bodies[i]->body, &cube, nanos, &player);
            if (bodies[i]->body->getCollisionShape()->getShapeType() == BOX_SHAPE_PROXYTYPE)
            {
                btRigidBody* sphere = bodies[i]->body;
                btVector3 extent = ((btBoxShape*)sphere->getCollisionShape())->getHalfExtentsWithoutMargin();
                btTransform t;
                sphere->getMotionState()->getWorldTransform(t);
                float mat[16];
                t.getOpenGLMatrix(mat);
                cubes.Use();
                glm::mat4 trans = glm::make_mat4(mat);
                glm::mat4 view = glm::mat4(1.0f);
                /*if (bodies[i]->hit == true)
                    cubes.setVec3("color", glm::vec3(0.0, 1.0, 0.0));
                else
                    cubes.setVec3("color", glm::vec3(0.0, 0.0, 1.0));*/
                view = player.lookAt();
                cubes.setMat4("view", view);
                glm::mat4 projection = glm::mat4(1.0f);
                projection = glm::perspective(45.0f, (GLfloat)640 / (GLfloat)480, 0.1f, 1000.0f);
                cubes.setMat4("projection", projection);
                cubes.setMat4("model", trans);
                cubes.setMat4("lightSpaceMatrix", lightSpaceMatrix);
                cubes.setVec3("lightPos", depthmap.lightPos);
                cubes.setVec3("viewPos", player.getCameraPos());
                //cubes.setInt("material.diffuse", 0);
                //cubes.setInt("material.specular", 1);
                //cubes.setVec3("viewPos", player.getCameraPos());
                //cubes.setFloat("material.shininess", 32.0f);
                
                //// directional light
                //cubes.setVec3("dirlight.direction", -0.2f, -1.0f, -0.3f);
                //cubes.setVec3("dirlight.ambient", 0.05f, 0.05f, 0.05f);
                //cubes.setVec3("dirlight.diffuse", 0.4f, 0.4f, 0.4f);
                //cubes.setVec3("dirlight.specular", 0.5f, 0.5f, 0.5f);
                //
                //cubes.setVec3("pointLight[0].position", light1.lightspos);
                //cubes.setVec3("pointLight[0].ambient", 0.05f, 0.05f, 0.05f);
                //cubes.setVec3("pointLight[0].diffuse", 0.8f, 0.8f, 0.8f);
                //cubes.setVec3("pointLight[0].specular", 1.0f, 1.0f, 1.0f);
                //cubes.setFloat("pointLight[0].constant", 1.0f);
                //cubes.setFloat("pointLight[0].linear", 0.09);
                //cubes.setFloat("pointLight[0].quadratic", 0.032);
                if (bodies[i]->type == 1)
                {
                    
                    trans = glm::translate(trans,glm::vec3(0.f,-30.0f,0.f));
                    trans = glm::scale(trans, glm::vec3(4.2f));
                    cubes.setMat4("model", trans);
                    nanosuit.Draw(cubes);
                }
                else
                    m4.Draw(cubes);
                

            }
            if (bodies[i]->body->getCollisionShape()->getShapeType() == STATIC_PLANE_PROXYTYPE)
                renderPlane(bodies[i]->body);
            if (bodies[i]->body->getCollisionShape()->getShapeType() == CONVEX_HULL_SHAPE_PROXYTYPE)
            {
                btRigidBody* sphere = bodies[i]->body;
                btVector3 extent = ((btBoxShape*)sphere->getCollisionShape())->getHalfExtentsWithoutMargin();
                btTransform t;
                sphere->getMotionState()->getWorldTransform(t);
                float mat[16];
                t.getOpenGLMatrix(mat);
                cubes.Use();
                glm::mat4 trans = glm::mat4(1.0f);
                trans = glm::make_mat4(mat);
                //trans = glm::scale(trans, glm::vec3(100, 100, 100));
                glm::mat4 view = glm::mat4(1.0f);
                view = player.lookAt();
                cubes.setMat4("view", view);
                glm::mat4 projection = glm::mat4(1.0f);
                projection = glm::perspective(45.0f, (GLfloat)640 / (GLfloat)480, 0.1f, 1000.0f);
                cubes.setMat4("projection", projection);
                cubes.setMat4("model", trans);
                cubes.setMat4("lightSpaceMatrix", lightSpaceMatrix);
                cubes.setVec3("lightPos", depthmap.lightPos);
                cubes.setVec3("viewPos", player.getCameraPos());
                level.meshes[pomodel + km].Draw(cubes);
                km++;
            }
        }
        slight.Use();
        glm::mat4 trans = glm::mat4(1.0);
        glm::mat4 view = glm::mat4(1.0f);
        view = player.lookAt();
        slight.setMat4("view", view);
        glm::mat4 projection = glm::mat4(1.0f);
        projection = glm::perspective(45.0f, (GLfloat)640 / (GLfloat)480, 0.1f, 1000.0f);
        slight.setMat4("projection", projection);
        slight.setMat4("model", trans);
        light1.Draw(slight);
        mat4 objectModel = mat4(1.0); //model matrix      
        //objectModel = glm::scale(objectModel, glm::vec3(0.1, 0.1, 0.1));
        anim->use();
        anim->setMat4("view", view);
        anim->setMat4("projection", projection);
        anim->setMat4("model", objectModel);
        object->render(anim);
        //print positions of all objects
        //for (int j = dynamicsWorld->getNumCollisionObjects() - 1; j >= 0; j--)
        //{
        //    btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[j];
        //    btRigidBody* body = btRigidBody::upcast(obj);
        //    btTransform trans;
        //    if (body && body->getMotionState())
        //    {
        //        body->getMotionState()->getWorldTransform(trans);
        //    }
        //    else
        //    {
        //        trans = obj->getWorldTransform();
        //    }
        //    //if (currentTime - lastTime >= 1.0)
        //    {
        //        printf("world pos object %d = %f,%f,%f\n", j, float(trans.getOrigin().getX()), float(trans.getOrigin().getY()), float(trans.getOrigin().getZ()));

        //    }
        //}
        // Set IMGUI DEBUG WINDOW
        ImGui::ShowDemoWindow(&show_demo_window);

        static float f = 0.0f;
        static int counter = 0;

        ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

        ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
        ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
        ImGui::Checkbox("Another Window", &show_another_window);

        ImGui::SliderFloat("float", &depthmap.lightPos.y, 0.0f, 1000.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
        ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

        if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
            counter++;
        ImGui::SameLine();
        ImGui::Text("counter = %d", counter);

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::End();
        if (show_another_window)
        {
            ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
            ImGui::Text("Hello from another window!");
            if (ImGui::Button("Close Me"))
                show_another_window = false;
            ImGui::End();
        }
        ImGui::Render();
        glDepthFunc(GL_LEQUAL);
        skyboxShader.Use();
        view = glm::mat4(glm::mat3(player.lookAt()));
        skyboxShader.setMat4("view", view);
        skyboxShader.setMat4("projection", projection);
        skybox.draw();

        glfwPollEvents();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
        if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_ESCAPE))
        {
            glfwSetWindowShouldClose(window, 1);
        }
    }
    // close GL context and any other GLFW resources
    for (int i = 0; i < bodies.size(); i++)
    {
        dynamicsWorld->removeCollisionObject(bodies[i]->body);
        btMotionState* motionState = bodies[i]->body->getMotionState();
        btCollisionShape* shape = bodies[i]->body->getCollisionShape();
        delete bodies[i]->body;
        delete shape;
        delete motionState;
        delete bodies[i];
    }
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    delete dynamicsWorld;
    delete collisionConfiguration;
    delete dispatcher;
    delete solver;
    glfwTerminate();
    return 0;
}

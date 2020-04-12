#define _CRT_SECURE_NO_WARNINGS
#define GLEW32_DLL
#include <GL/glew.h>
#define GLFW_DLL
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <bullet/btBulletDynamicsCommon.h>
#include "log.h"
#include "Model.h"
#include "Player.h"
#include "Bulletcallback.h"
#include "Light.h"
#include "GameObject.h"
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

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Hello Triangle", NULL, NULL);
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
    Player player(glm::vec3(0.0f, 0.0f, 0.3f),window);

    player.setupdayekey();
    player.setupdatemouse();
    // get version info
    const GLubyte* renderer = glGetString(GL_RENDERER); // get renderer string
    const GLubyte* version = glGetString(GL_VERSION); // version as a string
    printf("Renderer: %s\n", renderer);
    printf("OpenGL version supported %s\n", version);

    // tell GL to only draw onto a pixel if the shape is closer to the viewer
    glEnable(GL_DEPTH_TEST); // enable depth-testing
    glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"
    glfwWindowHint(GLFW_SAMPLES, 4);

    GLuint points_vbo = 0;
    glGenBuffers(1, &points_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
    glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), points, GL_STATIC_DRAW);
    GLuint colours_vbo = 0;
    glGenBuffers(1, &colours_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, colours_vbo);
    glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), colours, GL_STATIC_DRAW);
    GLuint vao = 0;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glBindBuffer(GL_ARRAY_BUFFER, colours_vbo);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    
    

    const char* vertex_shader =
        "#version 400\n"
        "layout(location = 0) in vec3 vertex_position;"
        "layout(location = 1) in vec3 vertex_colour;"
        "out vec3 colour;"
        "void main() {"
        "colour = vertex_colour;"
        "  gl_Position = vec4(vertex_position, 1.0);"
        "}";

    const char* fragment_shader =
        "#version 400\n"
        "in vec3 colour;"
        "out vec4 frag_colour;"
        "void main() {"
        "  frag_colour = vec4(colour, 1.0);"
        "}";
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
    //------
    Shader shader("triangle.vert", "triangle.frag");
    glClearColor(0.6f, 0.6f, 0.8f, 1.0f);
    Shader nanos("model.vert", "model.frag");
    Shader cubes("cube.vert", "cube.frag");
    Shader slight("lightcube.vert", "lightcube.frag");

    //Model nanosuit((char*)"models/nanosuit/nanosuit.obj");
    Model wall((char*)"models/fallingwall/swall.dae");
    Model cube((char*)"models/cube/cube.obj");
    Model level((char*)"models/gamelevels/SandFinal.obj");
    for(int i=0;i<cube.meshes[0].vertices.size();i++)
    {
        std::cout << "x: "<< cube.meshes[0].vertices[i].Position.x<<" y: " << cube.meshes[0].vertices[i].Position.y<< " z: " << cube.meshes[0].vertices[i].Position.z << std::endl;
    }
    Light light1((char*)"models/cube/cube.obj");
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
    btRigidBody* body = new btRigidBody(info);
    //dynamicsWorld->addRigidBody(body);
    //btRigidBody* spher = addSphere(10.0, 0, 20, 0, 10.0, dynamicsWorld);
    //spher->setLinearVelocity(btVector3(1.0, 0.0, 0.0));
    //btRigidBody* box = addBox(1.f, 1.f, 1.f, 0.f, 30.f, -1.f, 1.f, dynamicsWorld);
   // btRigidBody* cube1 = addBox(1.f, 1.f, 1.f, 0.f, 10.f, -1.f, 1.f, dynamicsWorld);
    //bodies.push_back(new bulletObject(cube1, bodies.size(), 1.0, 0.0, 0.0));
    unsigned int pomodel = addmodel(&level, dynamicsWorld,&bodies);
    //bodies.push_back(new bulletObject(btlevel, bodies.size(), 0.0f, 0.0f, 0.0f));
    double lastTime = glfwGetTime();
    int nbFrames = 0;

    //btRigidBody* lightc = addBox(2.f, 2.f, 2.f, player.getCameraPos().x, player.getCameraPos().y, player.getCameraPos().z, 1.f, dynamicsWorld);
    glm::vec3 look = player.getCameraLook();
    //lightc->setLinearVelocity(btVector3(look.x * 20, look.y * 20, look.z * 20));
    //lightc->setCollisionFlags(lightc->getCollisionFlags() | btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);
    //bodies.push_back(new bulletObject(lightc, 1, 1.0, 0.0, 0.0));
    ShaderLoader * anim = new ShaderLoader();
    anim->loadShaders("anim.vert", "anim.frag");
    GameObject* object = new GameObject(); //create model 

    //object->createGraphicsObject("models/Caterpillar/caterpillar.fbx"); //get data from file
   // object->applyLocalRotation(180, vec3(1, 0, 0)); //there are some problems with loading fbx files. Models could be rotated or scaled. So we rotate it to the normal state
    //object->playAnimation(new Animation("Orange", vec2(0, 195), 0.60, 10, true)); //forcing our model to play the animation (name, frames, speed, priority, loop)

    lastTime = glfwGetTime();
    while (!glfwWindowShouldClose(window))
    {
        double currentTime = glfwGetTime();
        nbFrames++;
        if (currentTime - lastTime >= 1.0)
        {
            printf("%f ms/frame\n", 1000.0 / double(nbFrames));
            // printf("p0:%f %f %f v0:%f %f %f\n", p0.x(), p0.y(), p0.z(), v0.x, v0.y, v0.z);
            nbFrames = 0;
            lastTime += 1.0;
        }

        float deltaTime = currentTime - lastTime;
        for (int i = 0; i < bodies.size(); i++)
        {
            if(bodies[i]->hit==true)
            bodies[i]->hit = false;
        }
        currentTime = glfwGetTime();
        player.elapsedtime = (currentTime - lastTime)/1;
        lastTime = currentTime;
        player.updatekey();
        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        btCollisionWorld::ClosestRayResultCallback raycallback (btVector3(player.getCameraPos().x, player.getCameraPos().y, player.getCameraPos().z), btVector3(player.getCameraLook().x*10000, player.getCameraLook().y*10000, player.getCameraLook().z*10000));
        //dynamicsWorld->rayTest(btVector3(player.getCameraPos().x, player.getCameraPos().y, player.getCameraPos().z), btVector3(player.getCameraLook().x * 10000, player.getCameraLook().y * 10000, player.getCameraLook().z * 10000),raycallback);
        if (raycallback.hasHit())
        {
            bulletObject* ob1 = (bulletObject*)(raycallback.m_collisionObject->getUserPointer());
            if(ob1!=NULL)
                ob1->hit = true;
        }
        //btVector3 p0 = rigidbodies[0]->getCenterOfMassPosition();
        //glm::vec3 v0 = position;
        dynamicsWorld->stepSimulation(1.f / 60.f, 10);

        


        _update_fps_counter(window);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        shader.Use();
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        {
            btRigidBody* cube2 = addBox(2.f, 2.f, 2.f, player.getCameraPos().x, player.getCameraPos().y, player.getCameraPos().z, 1.f, dynamicsWorld);
            glm::vec3 look = player.getCameraLook();
            cube2->setLinearVelocity(btVector3(look.x * 20, look.y * 20, look.z * 20));
            cube2->setCollisionFlags(cube2->getCollisionFlags() | btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);
            bulletObject *cubee = new bulletObject(cube2, bodies.size(), 1.0, 0.0, 0.0);
            bodies.push_back(cubee);
            cube2->setUserPointer(bodies[bodies.size()-1]);
            //count++;
            //cubee->body->setUserPointer((void*)(bodies.size()-1));

        }
        if (glfwGetKey(window, GLFW_KEY_9) == GLFW_PRESS)
        {
            if (player.cursor == true)
                player.cursor = false;
            else
                player.cursor = true;
        }
        if(player.cursor == true)
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

        //renderSphere(spher, &cube, nanos, &player);
        //renderBox(box, &shader, &player);
        int km = 0;
        for (int i = 0; i < bodies.size(); i++)
        {
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
                if (bodies[i]->hit == true)
                    cubes.setVec3("color", glm::vec3(0.0, 1.0, 0.0));
                else
                    cubes.setVec3("color", glm::vec3(0.0, 0.0, 1.0));
                view = player.lookAt();
                cubes.setMat4("view", view);
                glm::mat4 projection = glm::mat4(1.0f);
                projection = glm::perspective(45.0f, (GLfloat)640 / (GLfloat)480, 0.1f, 1000.0f);
                cubes.setMat4("projection", projection);
                cubes.setMat4("model", trans);


                cubes.setInt("material.diffuse", 0);
                cubes.setInt("material.specular", 1);
                cubes.setVec3("viewPos", player.getCameraPos());
                cubes.setFloat("material.shininess", 32.0f);
                
                // directional light
                cubes.setVec3("dirlight.direction", -0.2f, -1.0f, -0.3f);
                cubes.setVec3("dirlight.ambient", 0.05f, 0.05f, 0.05f);
                cubes.setVec3("dirlight.diffuse", 0.4f, 0.4f, 0.4f);
                cubes.setVec3("dirlight.specular", 0.5f, 0.5f, 0.5f);
                
                cubes.setVec3("pointLight[0].position", light1.lightspos);
                cubes.setVec3("pointLight[0].ambient", 0.05f, 0.05f, 0.05f);
                cubes.setVec3("pointLight[0].diffuse", 0.8f, 0.8f, 0.8f);
                cubes.setVec3("pointLight[0].specular", 1.0f, 1.0f, 1.0f);
                cubes.setFloat("pointLight[0].constant", 1.0f);
                cubes.setFloat("pointLight[0].linear", 0.09);
                cubes.setFloat("pointLight[0].quadratic", 0.032);
                
              /*  cubes.setVec3("pointLight[1].position", glm::vec3(2.3f, -3.3f, -4.0f));
                cubes.setVec3("pointLight[1].ambient", 0.05f, 0.05f, 0.05f);
                cubes.setVec3("pointLight[1].diffuse", 0.8f, 0.8f, 0.8f);
                cubes.setVec3("pointLight[1].specular", 1.0f, 1.0f, 1.0f);
                cubes.setFloat("pointLight[1].constant", 1.0f);
                cubes.setFloat("pointLight[1].linear", 0.09);
                cubes.setFloat("pointLight[1].quadratic", 0.032);
                
                cubes.setVec3("pointLight[2].position", glm::vec3(-4.0f, 2.0f, -12.0f));
                cubes.setVec3("pointLight[2].ambient", 0.05f, 0.05f, 0.05f);
                cubes.setVec3("pointLight[2].diffuse", 0.8f, 0.8f, 0.8f);
                cubes.setVec3("pointLight[2].specular", 1.0f, 1.0f, 1.0f);
                cubes.setFloat("pointLight[2].constant", 1.0f);
                cubes.setFloat("pointLight[2].linear", 0.09);
                cubes.setFloat("pointLight[2].quadratic", 0.032);
                
                cubes.setVec3("pointLight[3].position", glm::vec3(0.0f, 0.0f, -3.0f));
                cubes.setVec3("pointLight[3].ambient", 0.05f, 0.05f, 0.05f);
                cubes.setVec3("pointLight[3].diffuse", 0.8f, 0.8f, 0.8f);
                cubes.setVec3("pointLight[3].specular", 1.0f, 1.0f, 1.0f);
                cubes.setFloat("pointLight[3].constant", 1.0f);
                cubes.setFloat("pointLight[3].linear", 0.09);
                cubes.setFloat("pointLight[3].quadratic", 0.032);*/
                cube.Draw(cubes);

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
                nanos.Use();
                glm::mat4 trans = glm::mat4(1.0f);
                trans = glm::make_mat4(mat);
                //trans = glm::scale(trans, glm::vec3(100, 100, 100));
                glm::mat4 view = glm::mat4(1.0f);
                view = player.lookAt();
                nanos.setMat4("view", view);
                glm::mat4 projection = glm::mat4(1.0f);
                projection = glm::perspective(45.0f, (GLfloat)640 / (GLfloat)480, 0.1f, 1000.0f);
                nanos.setMat4("projection", projection);
                nanos.setMat4("model", trans);
                level.meshes[pomodel + km].Draw(nanos);
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
        anim->use();
        glUniformMatrix4fv(glGetUniformLocation(anim->ID, "view"), 1, GL_FALSE, value_ptr(view)); //send the view matrix to the shader
        glUniformMatrix4fv(glGetUniformLocation(anim->ID, "projection"), 1, GL_FALSE, value_ptr(projection)); //send the projection matrix to the shader
                                                
                                                
        mat4 objectModel = mat4(1.0); //model matrix      
        //objectModel = glm::scale(objectModel, glm::vec3(0.1, 0.1, 0.1));
        glUniformMatrix4fv(glGetUniformLocation(anim->ID, "model"), 1, GL_FALSE, value_ptr(objectModel)); //send the empty model matrix to the shader
        //object->render(anim);
        //print positions of all objects
        for (int j = dynamicsWorld->getNumCollisionObjects() - 1; j >= 0; j--)
        {
            btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[j];
            btRigidBody* body = btRigidBody::upcast(obj);
            btTransform trans;
            if (body && body->getMotionState())
            {
                body->getMotionState()->getWorldTransform(trans);
            }
            else
            {
                trans = obj->getWorldTransform();
            }
            //if (currentTime - lastTime >= 1.0)
            {
                printf("world pos object %d = %f,%f,%f\n", j, float(trans.getOrigin().getX()), float(trans.getOrigin().getY()), float(trans.getOrigin().getZ()));

            }
        }
        {
            ImGui::ShowDemoWindow(&show_demo_window);

            static float f = 0.0f;
            static int counter = 0;

            ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

            ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
            ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
            ImGui::Checkbox("Another Window", &show_another_window);

            ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
            ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

            if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
                counter++;
            ImGui::SameLine();
            ImGui::Text("counter = %d", counter);

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::End();
        }
        if (show_another_window)
        {
            ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
            ImGui::Text("Hello from another window!");
            if (ImGui::Button("Close Me"))
                show_another_window = false;
            ImGui::End();
        }
        ImGui::Render();
        glfwPollEvents();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);

        if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_ESCAPE))
        {
            glfwSetWindowShouldClose(window, 1);
        }
       /* while (glfwGetTime() < lastTime + 1.0 / FPS) {
            _sleep(300);
        }
        lastTime += 1.0 / FPS;*/
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

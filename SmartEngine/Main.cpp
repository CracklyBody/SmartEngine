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
#include "BulletSamples.h"

#define WIDTH 640
#define HEIGHT 480

GLfloat lastX = WIDTH;
GLfloat lastY = HEIGHT;
GLfloat yaw = -90.0f;
GLfloat pitch = 0.0f;

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

    std::vector<btRigidBody*> bodies;

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
    Shader shader("triangle.vert", "triangle.frag");
    glClearColor(0.6f, 0.6f, 0.8f, 1.0f);
    Shader nanos("model.vert", "model.frag");
    //Model nanosuit((char*)"models/nanosuit/nanosuit.obj");
    Model wall((char*)"models/fallingwall/swall.dae");
    Model cube((char*)"models/cube/cube.obj");
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
    for (int i = 0; i < 1; i++)
    {/*
        btDefaultMotionState* motionState = new btDefaultMotionState(btTransform(btQuaternion(orientation.x, orientation.y, orientation.z, orientation.w), btVector3(position.x, position.y, position.z)));
        
        btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(100, motionState, boxCollisionShape, btVector3(10, 0, 0));
        btRigidBody* rigidBody = new btRigidBody(rigidBodyCI);

        rigidbodies.push_back(rigidBody);
        dynamicsWorld->addRigidBody(rigidBody);

        rigidBody->setUserPointer((void*)i);*/
    }
    btTransform t;
    t.setIdentity();
    t.setOrigin(btVector3(0,0,0));
    btStaticPlaneShape* plane = new btStaticPlaneShape(btVector3(0, 1, 0),0);
    btMotionState* motion = new btDefaultMotionState(t);
    btRigidBody::btRigidBodyConstructionInfo info(0, motion, plane);
    btRigidBody* body = new btRigidBody(info);
    dynamicsWorld->addRigidBody(body);
    btRigidBody* spher = addSphere(10.0, 0, 20, 0, 10.0, dynamicsWorld);
    spher->setLinearVelocity(btVector3(1.0, 0.0, 0.0));
    btRigidBody* box = addBox(1.f, 1.f, 1.f, 0.f, 30.f, -1.f, 1.f, dynamicsWorld);
    btRigidBody* cube1 = addBox(1.f, 1.f, 1.f, 0.f, 10.f, -1.f, 1.f, dynamicsWorld);
    bodies.push_back(cube1);
    double lastTime = glfwGetTime();
    int nbFrames = 0;

    while (!glfwWindowShouldClose(window))
    {
        //btVector3 p0 = rigidbodies[0]->getCenterOfMassPosition();
        //glm::vec3 v0 = position;
        dynamicsWorld->stepSimulation(1.f / 60.f, 10);

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


        player.updatekey();
        _update_fps_counter(window);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        shader.Use();
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        {
            btRigidBody* cube2 = addBox(2.f, 2.f, 2.f, player.getCameraPos().x, player.getCameraPos().y, player.getCameraPos().z, 1.f, dynamicsWorld);
            glm::vec3 look = player.getCameraLook();
            cube2->setLinearVelocity(btVector3(look.x * 20, look.y * 20, look.z * 20));
            bodies.push_back(cube2);
        }
        //glm::mat4 trans = glm::mat4(1.0f);
        ////trans = glm::translate(trans, glm::vec3(1.5f, -0.5f, 0.0f));
        ////trans = glm::rotate(trans, 90.0f, glm::vec3(0.0f, 0.0f, 1.0f));
        //GLuint transformLoc = glGetUniformLocation(shader.Program, "trans");
        //glUniformMatrix4fv(transformLoc, 1, GL_FALSE, &trans[0][0]);

        //glm::mat4 view = glm::mat4(1.0f);
        //view = player.lookAt();
        //GLuint viewLoc = glGetUniformLocation(shader.Program, "view");
        //glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);

        //glm::mat4 projection = glm::mat4(1.0f);
        //projection = glm::perspective(45.0f, (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 1000.0f);
        //GLuint projectionLoc = glGetUniformLocation(shader.Program, "projection");
        //glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, &projection[0][0]);
        //
        //glBindVertexArray(vao);
        //glDrawArrays(GL_TRIANGLES, 0, 3);
        //nanos.Use();
        //nanos.setMat4("view", view);
        //nanos.setMat4("projection", projection);
        //nanos.setMat4("model", trans);
        ////wall.Draw(nanos);
        ////wall.update();
        //trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
        //trans = glm::scale(trans, glm::vec3(0.2f, 0.2f, 0.2f));
        //
        //nanos.setMat4("view", view);
        //nanos.setMat4("projection", projection);
        //nanos.setMat4("model", trans);
        ////nanosuit.Draw(nanos);
        ////nanosuit.update();
        //for (int i = 0; i < 1; i++)
        //{
        //    glm::mat4 RotationMatrix = glm::toMat4(orientation);
        //    glm::mat4 TranslationMatrix = glm::translate(glm::mat4(1.0f), position);
        //    TranslationMatrix = glm::scale(TranslationMatrix, glm::vec3(0.2f, 0.2f, 0.2f));
        //    glm::mat4 ModelMatrix = TranslationMatrix * RotationMatrix;
        //    nanos.setMat4("view", view);
        //    nanos.setMat4("projection", projection);
        //    nanos.setMat4("model", ModelMatrix);
        //    //cube.Draw(nanos);

        //}
        //renderPlane(body);
        renderSphere(spher, &cube, nanos, &player);
        //renderBox(box, &shader, &player);
        for (int i = 0; i < bodies.size(); i++)
        {
            if (bodies[i]->getCollisionShape()->getShapeType() == SPHERE_SHAPE_PROXYTYPE)
                renderSphere(bodies[i], &cube, nanos, &player);
            if (bodies[i]->getCollisionShape()->getShapeType() == BOX_SHAPE_PROXYTYPE)
                cube.Draw(bodies[i], nanos, &player);
            if (bodies[i]->getCollisionShape()->getShapeType() == STATIC_PLANE_PROXYTYPE)
                renderPlane(bodies[i]);
        }

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
        //btVector3 p0 = spher->getCenterOfMassPosition();
        //printf("%f %f %f\n", p0.x(), p0.y(), p0.z());
        glfwPollEvents();
        glfwSwapBuffers(window);

        if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_ESCAPE))
        {
            glfwSetWindowShouldClose(window, 1);
        }
    }
    // close GL context and any other GLFW resources
    delete dynamicsWorld;
    delete collisionConfiguration;
    delete dispatcher;
    delete solver;
    glfwTerminate();
    return 0;
}

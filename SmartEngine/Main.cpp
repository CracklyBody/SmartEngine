#define _CRT_SECURE_NO_WARNINGS
#define GLEW32_DLL
#include <GL/glew.h>
#define GLFW_DLL
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "log.h"
#include "Model.h"

#define WIDTH 640
#define HEIGHT 480

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

GLfloat lastX = WIDTH;
GLfloat lastY = HEIGHT;
GLfloat yaw = -90.0f;
GLfloat pitch = 0.0f;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);

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
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
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
    //glEnable(GL_CULL_FACE);
    //glCullFace(GL_BACK);
    //glFrontFace(GL_CW);

    while (!glfwWindowShouldClose(window))
    {
        _update_fps_counter(window);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        shader.Use();

        glm::mat4 trans = glm::mat4(1.0f);
        //trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
        //trans = glm::rotate(trans, 90.0f, glm::vec3(0.0f, 0.0f, 1.0f));
        GLuint transformLoc = glGetUniformLocation(shader.Program, "trans");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, &trans[0][0]);

        glm::mat4 view = glm::mat4(1.0f);
        view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        GLuint viewLoc = glGetUniformLocation(shader.Program, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);

        glm::mat4 projection = glm::mat4(1.0f);
        projection = glm::perspective(45.0f, (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f);
        GLuint projectionLoc = glGetUniformLocation(shader.Program, "projection");
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, &projection[0][0]);
        
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        nanos.Use();
        nanos.setMat4("view", view);
        nanos.setMat4("projection", projection);
        nanos.setMat4("model", trans);
        wall.Draw(nanos);
        trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
        trans = glm::scale(trans, glm::vec3(0.2f, 0.2f, 0.2f));
        
        nanos.setMat4("view", view);
        nanos.setMat4("projection", projection);
        nanos.setMat4("model", trans);
        //nanosuit.Draw(nanos);
        glfwPollEvents();
        glfwSwapBuffers(window);

        if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_ESCAPE))
        {
            glfwSetWindowShouldClose(window, 1);
        }
    }
    // close GL context and any other GLFW resources
    glfwTerminate();
    return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    GLfloat cameraSpeed = 0.05f;
    if (key == GLFW_KEY_W)
        cameraPos += cameraSpeed * cameraFront;
    if (key == GLFW_KEY_S)
        cameraPos -= cameraSpeed * cameraFront;
    if (key == GLFW_KEY_A)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if(key==GLFW_KEY_D)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;

}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    GLfloat xoffset = xpos - lastX;
    GLfloat yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    GLfloat sensitivity = 0.05f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
    front.y = sin(glm::radians(pitch));
    front.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
    cameraFront = glm::normalize(front);
}
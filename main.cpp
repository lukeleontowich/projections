/*****************************************************
** Project: Math 3200 Project
** File: main.cpp
** Author: Luke Leontowich & Jenna McDonnell
** Date: November 19, 2021
** Description: Geometry project.
*****************************************************/
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <cmath>

#include "Camera.h"
#include "Shader.h"
#include "Model.h"


//  Callback Functions
void reshape(GLFWwindow* window, int width, int height);
void mouse(GLFWwindow* window, double xpos, double ypos);
void input(GLFWwindow* window);

//  Timer
float delta_time = 0.0f;
float last_frame = 0.0f;

//  Window setting
unsigned int WIDTH = 900;
unsigned int HEIGHT = 660;

//  mouse settings
float sensitivity = 0.1f;
float last_x = float(WIDTH) / 2.0;
float last_y = float(HEIGHT) / 2.0;

//  Camera
Camera* camera = nullptr;


//  Temp function
glm::mat4 getMatrix(const float& alpha, const float& beta) {
    glm::mat4 m1(1.0f);
    glm::mat4 m2(1.0f);

    float cos_alpha = cos(glm::radians(alpha));
    float sin_alpha = sin(glm::radians(alpha));

    float cos_beta = cos(glm::radians(beta));
    float sin_beta = sin(glm::radians(beta));

     m1[1][1] = cos_alpha;
     m1[1][2] = sin_alpha;
     m1[2][1] = -sin_alpha;
     m1[2][2] = cos_alpha;

     m2[0][0] = cos_beta;
     m2[0][2] = -sin_beta;
     m2[2][0] = sin_beta;
     m2[2][2] = cos_beta;

     return m1 * m2;
}

/*************************************************************
***   MAIN   *************************************************
*************************************************************/
int main() {
    /***   Initializing ***/
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    //  Create Window
    auto window = glfwCreateWindow(WIDTH, HEIGHT, "Projections Project", NULL, NULL);
    if (window == NULL) {
        std::cout << "Error creating window\n";
        glfwTerminate();
        return 1;
    }


    glfwMakeContextCurrent(window);

    //  Set callbacks
    glfwSetFramebufferSizeCallback(window, reshape);
    glfwSetCursorPosCallback(window, mouse);

    //  Initialize glad
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    //  Enable depth testing
    glEnable(GL_DEPTH_TEST);

    //  Disable mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    //  Initialize Camera
    camera = new Camera();
    camera->setPos(glm::vec3(0.0f, 0.0f, -10.0f));
    camera->setYaw(90.0f);


    //  Model
    game::Model tree;
    tree.init("objects/tree1/tree1.obj");
    game::Shader shader;
    shader.init("shaders/object.vs", "shaders/object.fs");

    /*********** MAIN LOOP  ***********************/
    while (!glfwWindowShouldClose(window)) {

        //  Set background color
        glClearColor(0.9, 0.9, 0.9, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        float current_frame = glfwGetTime();
        delta_time = current_frame - last_frame;
        last_frame = current_frame;

        //  update camera
        camera->update();

        //  Set View Matrix
        auto view = glm::lookAt(camera->pos(), camera->pos() + camera->dir(), camera->up());

        //  Set Projection Matrix
        auto projection = glm::perspective(glm::radians(45.0f), float(WIDTH) / float(HEIGHT), 0.1f, 100.0f);
        //auto projection = getMatrix(45.0f, 45.0f);

        //  Set Model Matrix (Identity Matrix)
        auto model = glm::mat4(1.0f);


        //  Need to set shader to current shader'
        shader.use();

        //  Set the matrices in the shader
        //  The shader will do the multiplication M*V*P = model*view*projection
        glUniformMatrix4fv(glGetUniformLocation(shader.id(), "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(shader.id(), "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(shader.id(), "model"), 1, GL_FALSE, glm::value_ptr(model));


        tree.draw(shader);

        //  Process input
        input(window);


        //  Swap buffers and poll events
        glfwSwapBuffers(window);


        glfwPollEvents();
    }

    //  Deallocate memory
    tree.destroyModel();
    delete camera;

    glfwTerminate();
    return 0;
}

/***************************************************************
***   RESHAPE   ************************************************
***************************************************************/
void reshape(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
    WIDTH = width;
    HEIGHT = height;
}


/******************************************************************
***   MOUSE   *****************************************************
******************************************************************/
void mouse(GLFWwindow* window, double xpos, double ypos) {
    camera->updateYaw((xpos - last_x) * sensitivity);
    camera->updatePitch((last_y - ypos) * sensitivity);

    if (camera->pitch() > 89.0) {
        camera->setPitch(89.0);
    }
    if (camera->pitch() < -89.0) {
        camera->setPitch(-89.0);
    }

    last_x = xpos;
    last_y = ypos;
}


/***********************************************************************
***  INPUT   ***********************************************************
***********************************************************************/
void input(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
    //  set speed
    float speed = delta_time * 2.0;

    //  Get the up and direction vectors
    glm::vec3 direction = glm::vec3(camera->dir().x, 0.0f, camera->dir().z);
    glm::vec3 up = glm::vec3(0.0, 1.0, 0.0);

    //  The output_vector is the amount that the camera will move
    glm::vec3 output_vector = glm::vec3(0.0, 0.0, 0.0);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        output_vector += speed * glm::normalize(direction);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        output_vector -= speed * glm::normalize(direction);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        output_vector -= glm::normalize(glm::cross(direction, up)) * speed;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        output_vector += glm::normalize(glm::cross(direction, up)) * speed;
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        output_vector += glm::vec3(0.0f, speed, 0.0f);
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        output_vector -= glm::vec3(0.0f, speed, 0.0f);
    }

    //  Update the camera position
    camera->updatePos(output_vector);
}

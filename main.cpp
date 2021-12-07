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
#include <iomanip>
#include <cstdlib>
#include <vector>
#include <utility>

#include "Camera.h"
#include "Shader.h"
#include "Model.h"
#include "AbstractProjectionFactory.h"
#include "PerspectiveOnePointFactory.h"
#include "OrthogonalProjection.h"
#include "ObliqueProjection.h"
#include "PerspectiveProjection.h"


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

//  parameters for projections
float left = -10.0f, right = 10.0f;
float bottom = -10.0f, top = 10.0f;
float near = 1.0f, far = 100.0f;

//  container to store objects
typedef std::pair<game::Model, glm::vec4> pr;
std::vector<pr> objects;


//  Projection matrix factory
AbstractProjectionFactory* projection_factory = nullptr;

void printMat(const glm::mat4& m) {
    std::cout << std::fixed << std::setprecision(6);
    for (unsigned i = 0; i < 4; ++i) {
        for (unsigned j = 0; j < 4; ++j) {
            std::cout << m[j][i] << " ";
        }
        std::cout << "\n";
    }
    std::cout << "\n";
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


    //  Initialize shader
    //  We will just use one simple shader for this application
    game::Shader shader;
    shader.init("shaders/object.vs", "shaders/object.fs");

    //  Initialize game objects;
    game::Model tree;
    tree.init("objects/PenroseTriangle/PenroseTriangle.obj");
    pr p;
    p.first = tree;
    p.second = glm::vec4(0.2f, 1.0f, 0.2f, 1.0f);

    //  push into vector
    objects.push_back(p);


    shader.use();
    glUniform4f(glGetUniformLocation(shader.id(), "color"), 0.7f, 0.5f, 0.3f, 1.0f);

    //  Set Projection Matrix
    projection_factory = new PerspectiveOnePointFactory(45.0f, 1.0f, near, far);

    //  Output matrix info
    projection_factory->print();
    printMat(projection_factory->createProjection());


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

        //  Get projection matrix
        auto projection = projection_factory->createProjection();


        //  Set Model Matrix (Identity Matrix)
        auto model = glm::mat4(1.0f);

        model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));

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
    delete projection_factory;

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
    float speed = delta_time * 3.0;

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

    //  checks if we changed matrices
    static bool matrix_changed = false;

    static bool one_pressed = false;
    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS && !one_pressed) {
        if (projection_factory != nullptr) {
            delete projection_factory;
            projection_factory =
                new PerspectiveOnePointFactory(45.0f, float(WIDTH) / float(HEIGHT), near, far);
            matrix_changed = true;
        }
        one_pressed = true;
    }
    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_RELEASE && one_pressed) {
        one_pressed = false;
    }

    static bool two_pressed = false;
        if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS && !two_pressed) {
        if (projection_factory != nullptr) {
            delete projection_factory;
            projection_factory =  new OrthogonalProjection(left, right, bottom, top, near, far);
            matrix_changed = true;
        }
        two_pressed = true;
    }
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_RELEASE && two_pressed) {
        two_pressed = false;
    }

    static bool three_pressed = false;
    if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS && !three_pressed) {
        if (projection_factory != nullptr) {
            delete projection_factory;
            projection_factory =  new ObliqueProjection(left, right, bottom, top, near, far, 30.0f, 42.0f);
            matrix_changed = true;
        }
        three_pressed = true;
    }
    if (glfwGetKey(window, GLFW_KEY_3) == GLFW_RELEASE && three_pressed) {
        three_pressed = false;
    }

    //  Take care of possible new output stream
    if (matrix_changed) {
        //  Clear the output stream and print the new matrix info
        system("clear");
        projection_factory->print();
        printMat(projection_factory->createProjection());
        matrix_changed = false;
    }

    static bool p_pressed = false;
    static bool paused = false;
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS && !p_pressed) {
        if (!paused) {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        } else {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }
        p_pressed = true;
    }
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_RELEASE && p_pressed) {
        p_pressed = false;
    }

    //  Update the camera position
    camera->updatePos(output_vector);
}

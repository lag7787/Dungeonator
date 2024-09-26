#include <iostream>
#include "glad/glad.h"
#include "glfw3.h"
#include "generator.h"

const unsigned int SCRN_WIDTH = 800;
const unsigned int SCRN_HEIGHT = 600;

int main(){

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* window = glfwCreateWindow(SCRN_WIDTH, SCRN_HEIGHT, "dungeon", NULL, NULL);

    glfwMakeContextCurrent(window);
    if (!gladLoadGL()) {
        std::cout << "loading gl" << std::endl;
    }

//    while(!glfwWindowShouldClose(window)) {
//        glfwSwapBuffers(window);
//        glfwPollEvents();
//    }


    

    Generator gen = Generator(SCRN_WIDTH, SCRN_HEIGHT);
    Cell cell = gen.generate();
    cell.split(SCRN_WIDTH/4, SCRN_HEIGHT/4);
    cell.buildRooms(&gen);
    // build rooms in every leaf node
    gen.printRooms();
    float* verticies = gen.getVerticies(gen.rooms[0]);
    for (int i = 0; i < 18; i++) {
        std::cout << verticies[i] << std::endl;
    }


    glfwTerminate();

}
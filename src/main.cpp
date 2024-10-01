#include <iostream>
#include "glad/glad.h"
#include "glfw3.h"
#include "generator.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

const unsigned int SCRN_WIDTH = 800;
const unsigned int SCRN_HEIGHT = 600;

void keyCallBacks(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

void frameBufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}   

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

    glfwSetKeyCallback(window, keyCallBacks);
    glfwSetFramebufferSizeCallback(window, frameBufferSizeCallback);

    Generator gen = Generator(SCRN_WIDTH, SCRN_HEIGHT);
    Cell cell = gen.generate(); // should proboably just pass scrn width and height into the genreator
    cell.split(SCRN_WIDTH/6, SCRN_HEIGHT/6, Cell::HORIZONTAL);
    srand(time(NULL));
    cell.buildRooms(&gen);


    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    try {
        vShaderFile.open("/home/lucgarabrant/Documents/Projects/dungeon/src/shaders/shader.vs");
        fShaderFile.open("/home/lucgarabrant/Documents/Projects/dungeon/src/shaders/shader.fs");
        std::stringstream vShaderStream;
        std::stringstream fShaderStream;
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        vShaderFile.close();
        fShaderFile.close();
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    } catch(std::ifstream::failure e) {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }

    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    unsigned int vertex, fragment;
    int success;
    char infoLog[512];

    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(vertex, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION::FAILED\n" << infoLog << std::endl;
    }

    infoLog[0] = '\0';

    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(fragment, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION::FAILED\n" << infoLog << std::endl;
    }

    unsigned int ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);
    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(ID, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    glUseProgram(ID);
    glDeleteShader(vertex);
    glDeleteShader(fragment);


    glm::mat4 projection = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f, 0.1f, 1000.0f);
    glUniformMatrix4fv(glGetUniformLocation(ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

    float vertices[] = {
        0.5f, 0.5f, -1.0f,
        0.5f, -0.5f, -1.0f,
        -0.5f, 0.5f, -1.0f,
        0.5f, -0.5f, -1.0f,
        -0.5f, -0.5f, -1.0f,
        -0.5f, 0.5f, -1.0f
    };

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // i think it is the verticies... if we bind different ones. it will probably work
    unsigned int count = 0;
    while(!glfwWindowShouldClose(window)) {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(ID);
        glBindVertexArray(VAO);
    //    glm::mat4 model = glm::mat4(1.0f);
    //    model = glm::translate(model, glm::vec3(400.0f, 300.0f, 0.0f));
    //    model = glm::scale(model, glm::vec3(
    //        400.0f,
    //        300.0f,
    //        1.0f
    //    ));

    //    glUniformMatrix4fv(glGetUniformLocation(ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
    //    glDrawArrays(GL_TRIANGLES, 0, 6);
        for (unsigned int i = 0; i < gen.rooms.size(); i++) {
            glm::mat4 model = glm::mat4(1.0f);
            float midX = ((float) gen.rooms[i]->x2 + (float)gen.rooms[i]->x1)/2.0f;
            float midY = ((float) gen.rooms[i]->y2 + (float)gen.rooms[i]->y1)/2.0f;
            model = glm::translate(model, glm::vec3(midX, midY, 0.0f));
            model = glm::scale(model, glm::vec3(
                (float) gen.rooms[i]->getLength(),
                (float) gen.rooms[i]->getHeight(),
                1.0f
            ));
            glUniformMatrix4fv(glGetUniformLocation(ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
            glDrawArrays(GL_TRIANGLES, 0, 6);
            if (count == 0) {
                std::cout << "room " << i << " height: " << gen.rooms[i]->getHeight() << std::endl;
                std::cout << "room " << i << " length: " << gen.rooms[i]->getLength() << std::endl;
               // std::cout << "x1: " << gen.rooms[i]->x1 << " y1: " << gen.rooms[i]->y1 << std::endl;
               // std::cout << "x2: " << gen.rooms[i]->x2 << " y2: " << gen.rooms[i]->y2 << std::endl;
                std::cout << "midX: " << midX << std::endl;
                std::cout << "midY: " << midY << std::endl;
            }
        }
        count++;

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();

}
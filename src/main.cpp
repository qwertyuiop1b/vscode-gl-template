#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <cmath>
#include <string>
#include <fstream>
#include <sstream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

const unsigned int WIDTH = 800;
const unsigned int HEIGHT = 600;


class Shader
{
public:
  unsigned int ID;
  // constructor generates the shader on the fly
  // ------------------------------------------------------------------------
  Shader(const char *vertexPath, const char *fragmentPath)
  {
    // 1. retrieve the vertex/fragment source code from filePath
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    // ensure ifstream objects can throw exceptions:
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
      // open files
      vShaderFile.open(vertexPath);
      fShaderFile.open(fragmentPath);
      std::stringstream vShaderStream, fShaderStream;
      // read file's buffer contents into streams
      vShaderStream << vShaderFile.rdbuf();
      fShaderStream << fShaderFile.rdbuf();
      // close file handlers
      vShaderFile.close();
      fShaderFile.close();
      // convert stream into string
      vertexCode = vShaderStream.str();
      fragmentCode = fShaderStream.str();
    }
    catch (std::ifstream::failure &e)
    {
      std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ: " << e.what() << std::endl;
    }
    const char *vShaderCode = vertexCode.c_str();
    const char *fShaderCode = fragmentCode.c_str();
    // 2. compile shaders
    unsigned int vertex, fragment;
    // vertex shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    checkCompileErrors(vertex, "VERTEX");
    // fragment Shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    checkCompileErrors(fragment, "FRAGMENT");
    // shader Program
    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);
    checkCompileErrors(ID, "PROGRAM");
    // delete the shaders as they're linked into our program now and no longer necessary
    glDeleteShader(vertex);
    glDeleteShader(fragment);
  }
  // activate the shader
  // ------------------------------------------------------------------------
  void use()
  {
    glUseProgram(ID);
  }
  // utility uniform functions
  // ------------------------------------------------------------------------
  void setBool(const std::string &name, bool value) const
  {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
  }
  // ------------------------------------------------------------------------
  void setInt(const std::string &name, int value) const
  {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
  }
  // ------------------------------------------------------------------------
  void setFloat(const std::string &name, float value) const
  {
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
  }

private:
  // utility function for checking shader compilation/linking errors.
  // ------------------------------------------------------------------------
  void checkCompileErrors(unsigned int shader, std::string type)
  {
    int success;
    char infoLog[1024];
    if (type != "PROGRAM")
    {
      glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
      if (!success)
      {
        glGetShaderInfoLog(shader, 1024, NULL, infoLog);
        std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n"
                  << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
      }
    }
    else
    {
      glGetProgramiv(shader, GL_LINK_STATUS, &success);
      if (!success)
      {
        glGetProgramInfoLog(shader, 1024, NULL, infoLog);
        std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n"
                  << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
      }
    }
  }
};



int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "learnopengl", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glViewport(0, 0, WIDTH, HEIGHT);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    Shader ourShader("vert.glsl", "frag.glsl");

    int width, height, nrChannels;
    unsigned char *data = stbi_load("container.jpg", &width, &height, &nrChannels, 0);

    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);



    float vertices[] = {
        // 位置              // 颜色
        0.5f, 0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // 右下
        -0.5f, 0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // 左下
        0.0f,  -0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // 顶部
    }; 

    float texCoords[] = {
      0.0f, 0.0f,
      1.0f, 0.0f,
      0.5f, 0.5f,
    };

    // // create VAO VBO
    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // 获取顶点属性的最大个数
    // int nrAttributes;
    // glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
    // std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes << std::endl;  

   


   
    while (!glfwWindowShouldClose(window)) {
        processInput(window);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        float time = glfwGetTime();
        

        ourShader.use();
        ourShader.setFloat("uOffsetX", sin(time) * 0.1);
        ourShader.setFloat("uOffsetY", cos(time) * 0.1);
        ourShader.setFloat("time", time);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);

    glfwTerminate();
    return 0;
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}
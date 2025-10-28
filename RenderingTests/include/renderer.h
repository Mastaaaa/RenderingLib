#pragma once
#include "shader.h"
#include "texture.h"
#include "camera.h"
#include <SDL2/SDL.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <vector>


// Forward declarations
class Texture;
class GameObject;

class Renderer
{
public:
    Renderer();
    ~Renderer();
    
    bool initialize(SDL_Window* window, const glm::mat4& projectionMatrix);
    void shutdown();
    void clearScreen();
    void drawSprite(Texture* texture,
        glm::vec2 position,
        glm::vec2 size,
        float rotate = 0.0f);
    void present();
    void onWindowResize(int newWidth, int newHeight, const glm::mat4& projectionMatrix);
    void renderScene(const glm::mat4& viewMatrix);

    void update(float deltaTime);

private:
    Shader* triangleShader_;

    //what if I have a large amout of VAO,VBO, EBO? maybe just save the VAO.
    unsigned int triangleVAO_;
    unsigned int triangleVBO_;
    unsigned int triangleEBO_;

    //transformations
    float rotation_;
    float rotationSpeed_;
    glm::mat4 transformStatic_;
    glm::mat4 transformDynamic_;

    //texture
    Texture* texture1_;
    Texture* texture2_;

    SDL_GLContext context_;
    unsigned int quadVAO_;
    glm::mat4 m_projectionMatrix;
    SDL_Window* window_;
    bool loadShaders();
    unsigned int compileShader(const std::string& source, int shaderType);
    unsigned char* textureFromImage(const char* name);
};
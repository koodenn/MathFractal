#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <algorithm>
#include <string>
#include <cmath>
#include "Shader.h"

int winWidth = 1280;
int winHeight = 720;

float offsetX = 0.0f;
float offsetY = 0.0f;
float zoom = 1.0f;

bool isDragging = false;
double lastMouseX = 0.0;
double lastMouseY = 0.0;
float mouseNormX = 0.0f;
float mouseNormY = 0.0f;
bool juliaMouseControl = false;

int fractalType = 0;
int paletteType = 0;
int maxIterations = 250;
bool isAnimated = true;
float animTime = 0.0f;

const char* fractalNames[] = { "Julia Set", "Mandelbrot", "Burning Ship", "Tricorn" };
const char* paletteNames[] = { "Cyberpunk Neon", "Fire and Magma", "Deep Ocean", "Psychedelic Rainbow", "Amethyst" };

void resetView() {
    zoom = 1.0f;
    if (fractalType == 0) {
        offsetX = 0.0f;
        offsetY = 0.0f;
    } else if (fractalType == 1) {
        offsetX = -0.5f;
        offsetY = 0.0f;
    } else if (fractalType == 2) {
        offsetX = -0.45f;
        offsetY = -0.5f;
        zoom = 1.2f;
    } else {
        offsetX = -0.2f;
        offsetY = 0.0f;
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    if (width > 0 && height > 0) {
        winWidth = width;
        winHeight = height;
        glViewport(0, 0, width, height);
    }
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        if (action == GLFW_PRESS) {
            isDragging = true;
            glfwGetCursorPos(window, &lastMouseX, &lastMouseY);
        } else if (action == GLFW_RELEASE) {
            isDragging = false;
        }
    } else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
        juliaMouseControl = !juliaMouseControl;
        std::cout << "[INFO] Mode souris Julia: " << (juliaMouseControl ? "ACTIVE" : "DESACTIVE") << std::endl;
    }
}

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
    float minDim = (float)std::min(winWidth, winHeight);
    if (isDragging) {
        float dx = (float)(xpos - lastMouseX);
        float dy = (float)(ypos - lastMouseY);
        offsetX -= dx * (3.0f / zoom) / minDim;
        offsetY += dy * (3.0f / zoom) / minDim;
        lastMouseX = xpos;
        lastMouseY = ypos;
    }

    mouseNormX = (float)((xpos - 0.5 * winWidth) / minDim) * 2.0f;
    mouseNormY = (float)((0.5 * winHeight - ypos) / minDim) * 2.0f;
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    float minDim = (float)std::min(winWidth, winHeight);

    float worldX = (float)(xpos - 0.5 * winWidth) / minDim * (3.0f / zoom) + offsetX;
    float worldY = (float)(0.5 * winHeight - ypos) / minDim * (3.0f / zoom) + offsetY;

    float zoomFactor = (yoffset > 0) ? 1.25f : 0.8f;
    zoom *= zoomFactor;
    if (zoom < 0.01f) zoom = 0.01f;

    offsetX = worldX - (float)(xpos - 0.5 * winWidth) / minDim * (3.0f / zoom);
    offsetY = worldY - (float)(0.5 * winHeight - ypos) / minDim * (3.0f / zoom);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action != GLFW_PRESS && action != GLFW_REPEAT) return;

    switch (key) {
        case GLFW_KEY_ESCAPE:
            glfwSetWindowShouldClose(window, GLFW_TRUE);
            break;
        case GLFW_KEY_1:
            fractalType = 0;
            resetView();
            break;
        case GLFW_KEY_2:
            fractalType = 1;
            resetView();
            break;
        case GLFW_KEY_3:
            fractalType = 2;
            resetView();
            break;
        case GLFW_KEY_4:
            fractalType = 3;
            resetView();
            break;
        case GLFW_KEY_C:
            paletteType = (paletteType + 1) % 5;
            std::cout << "[Palette] " << paletteNames[paletteType] << std::endl;
            break;
        case GLFW_KEY_SPACE:
            isAnimated = !isAnimated;
            std::cout << "[Animation] " << (isAnimated ? "Play" : "Pause") << std::endl;
            break;
        case GLFW_KEY_R:
            resetView();
            std::cout << "[Reset View]" << std::endl;
            break;
        case GLFW_KEY_M:
            juliaMouseControl = !juliaMouseControl;
            std::cout << "[Controle Souris Julia] " << (juliaMouseControl ? "ON" : "OFF") << std::endl;
            break;
        case GLFW_KEY_UP:
            maxIterations = std::min(maxIterations + 50, 1000);
            std::cout << "[Iterations] " << maxIterations << std::endl;
            break;
        case GLFW_KEY_DOWN:
            maxIterations = std::max(maxIterations - 50, 50);
            std::cout << "[Iterations] " << maxIterations << std::endl;
            break;
        case GLFW_KEY_H:
            std::cout << "\n=== CONTROLES FRACTALES ===" << std::endl;
            std::cout << " [Clic gauche + glisser] : Deplacer la fractale (pan)" << std::endl;
            std::cout << " [Molette souris]        : Zoom avant / arriere sur curseur" << std::endl;
            std::cout << " [1 / 2 / 3 / 4]         : Julia, Mandelbrot, Burning Ship, Tricorn" << std::endl;
            std::cout << " [C]                     : Changer de palette de couleurs (5 palettes)" << std::endl;
            std::cout << " [Clic droit / M]        : Piloter la forme Julia avec la souris" << std::endl;
            std::cout << " [ESPACE]                : Pause / Reprise animation" << std::endl;
            std::cout << " [R]                     : Reinitialiser la vue" << std::endl;
            std::cout << " [Fleches HAUT / BAS]    : Iterations" << std::endl;
            std::cout << " [ECHAP]                 : Quitter" << std::endl;
            std::cout << "===========================\n" << std::endl;
            break;
    }
}

int main(void)
{
    if (!glfwInit()) {
        std::cerr << "Erreur: Impossible d initialiser GLFW" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(winWidth, winHeight, "Fractales OpenGL", NULL, NULL);
    if (!window) {
        std::cerr << "Erreur: Impossible de creer la fenetre GLFW" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cerr << "Erreur: Impossible d initialiser GLEW" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetKeyCallback(window, key_callback);

    glfwGetFramebufferSize(window, &winWidth, &winHeight);
    glViewport(0, 0, winWidth, winHeight);

    float vertices[] = {
        // x      y     z
        -1.0f, -1.0f, 0.0f,
         1.0f, -1.0f, 0.0f,
         1.0f,  1.0f, 0.0f,
        -1.0f,  1.0f, 0.0f
    };

    unsigned int indices[] = {
        0, 1, 2,
        0, 2, 3
    };

    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    Shader shader("vertex.glsl", "fragment.glsl");

    resetView();

    std::cout << "\n=== FRACTALES OPENGL DEMARRE ===" << std::endl;
    std::cout << "Appuyez sur [H] pour afficher la liste des commandes.\n" << std::endl;

    double lastTime = glfwGetTime();
    double titleTimer = lastTime;
    int frameCount = 0;

    while (!glfwWindowShouldClose(window))
    {
        double currentTime = glfwGetTime();
        double dt = currentTime - lastTime;
        lastTime = currentTime;

        if (isAnimated) {
            animTime += (float)dt;
        }

        frameCount++;
        if (currentTime - titleTimer >= 0.25) {
            double fps = frameCount / (currentTime - titleTimer);
            char title[256];
            snprintf(title, sizeof(title), "Fractales OpenGL | %s | %s | Zoom: %.2fx | Iter: %d | FPS: %.0f",
                     fractalNames[fractalType], paletteNames[paletteType], zoom, maxIterations, fps);
            glfwSetWindowTitle(window, title);
            titleTimer = currentTime;
            frameCount = 0;
        }

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shader.Bind();
        shader.setVec2("resolution", (float)winWidth, (float)winHeight);
        shader.setVec2("offset", offsetX, offsetY);
        shader.setFloat("zoom", zoom);
        shader.setFloat("time", animTime);
        shader.setInt("fractalType", fractalType);
        shader.setInt("paletteType", paletteType);
        shader.setInt("maxIterations", maxIterations);
        shader.setVec2("mousePos", mouseNormX, mouseNormY);
        shader.setInt("juliaMouseControl", juliaMouseControl ? 1 : 0);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    glfwTerminate();
    return 0;
}

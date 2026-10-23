#include <iostream>
#include <sstream>
#include <math.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <../glm/gtc/matrix_transform.hpp>
#include <algorithm>

#include "ShaderProgram.h"
#include "Texture2D.h"
#include "Camera.h"
#include "Mesh.h"

#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"

int WIDTH = 1920, HEIGHT = 1080;
const char* TITLE = "Project OpenGL";
GLFWwindow* gWindow = NULL;
bool gWireFrame = true;
bool gFlashlightOn = true;

float cubeAngle = 0.0f;

FPSCamera fpsCamera(glm::vec3(0.0f, 5.0f, 10.0f));
const double ZOOM_SENSITIVITY = -3.0f;
const float MOVE_SPEED = 10.0;
const float MOUSE_SENSITIVITY = 0.05f;
bool gCinematicMode = true;
float cinematicStartTime = 0.0f;

void glfw_onKey(GLFWwindow* window, int key, int scancode, int action, int mode);
void glfw_OnFrameBufferSize(GLFWwindow* window, int width, int height);
void glfw_onMouseMove(GLFWwindow* window, double posX, double posY);
void glfw_onMouseScroll(GLFWwindow* window, double deltaX, double deltaY);
void update(double elapsedTime);
void showFPS(GLFWwindow* window);
bool initOpenGL();
glm::vec3 cinematicMoveTo(glm::vec3 startPos, glm::vec3 endPos, float startTime, float duration, float currentTime);
void cinematicLookAt(glm::vec3 cameraPos, glm::vec3 targetPos, float& outYaw, float& outPitch);
glm::vec3 cinematicOrbit(glm::vec3 center, float radius, float speed, float currentTime);

int main()
{
    if (!initOpenGL())
    {
        std::cerr << "GLFW initialization failed" << std::endl;
        return -1;
    }

    ShaderProgram lightShader;
    lightShader.loadShader("shaders/basic.vert", "shaders/basic.frag");

    ShaderProgram lightingShader;
    lightingShader.loadShader("shaders/lighting_dir.vert", "shaders/lighting_mix.frag");


    // Load meshes and textures
    const int numModels = 101;
    Mesh mesh[numModels];
    Texture2D texture[numModels];

    mesh[0].loadOBJ("models/Captain.obj");
    mesh[1].loadOBJ("models/ocean.obj");
    mesh[2].loadOBJ("models/island.obj");

    mesh[3].loadOBJ("models/dock.obj");
    mesh[4].loadOBJ("models/dock.obj");
    mesh[5].loadOBJ("models/dock.obj");
    mesh[6].loadOBJ("models/dock.obj");
    mesh[7].loadOBJ("models/dock.obj");
    mesh[8].loadOBJ("models/dock.obj");

    mesh[9].loadOBJ("models/ship.obj");
    mesh[10].loadOBJ("models/henry.obj");
    mesh[11].loadOBJ("models/house.obj");
    mesh[12].loadOBJ("models/boat.obj");
    mesh[13].loadOBJ("models/anne.obj");
    mesh[14].loadOBJ("models/skeleton.obj");
    mesh[15].loadOBJ("models/chest.obj");

    mesh[16].loadOBJ("models/palm1.obj");
    mesh[17].loadOBJ("models/palm2.obj");
    mesh[18].loadOBJ("models/palm3.obj");
    mesh[19].loadOBJ("models/palm1.obj");
    mesh[20].loadOBJ("models/palm2.obj");
    mesh[21].loadOBJ("models/palm3.obj");
    mesh[22].loadOBJ("models/palm1.obj");
    mesh[23].loadOBJ("models/palm2.obj");
    mesh[24].loadOBJ("models/palm3.obj");
    mesh[25].loadOBJ("models/palm1.obj");
    mesh[26].loadOBJ("models/palm2.obj");
    mesh[27].loadOBJ("models/palm3.obj");
    mesh[28].loadOBJ("models/palm1.obj");
    mesh[29].loadOBJ("models/palm2.obj");
    mesh[30].loadOBJ("models/palm3.obj");
    mesh[31].loadOBJ("models/palm1.obj");
    mesh[32].loadOBJ("models/palm2.obj");
    mesh[33].loadOBJ("models/palm3.obj");
    mesh[34].loadOBJ("models/palm1.obj");
    mesh[35].loadOBJ("models/palm2.obj");
    mesh[36].loadOBJ("models/palm3.obj");
    mesh[37].loadOBJ("models/palm1.obj");
    mesh[38].loadOBJ("models/palm2.obj");
    mesh[39].loadOBJ("models/palm3.obj");
    mesh[40].loadOBJ("models/palm1.obj");
    mesh[41].loadOBJ("models/palm2.obj");
    mesh[42].loadOBJ("models/palm3.obj");
    mesh[43].loadOBJ("models/palm1.obj");
    mesh[44].loadOBJ("models/palm2.obj");
    mesh[45].loadOBJ("models/palm3.obj");
    mesh[46].loadOBJ("models/palm1.obj");
    mesh[47].loadOBJ("models/palm2.obj");
    mesh[48].loadOBJ("models/palm3.obj");
    mesh[49].loadOBJ("models/palm1.obj");
    mesh[50].loadOBJ("models/palm2.obj");
    mesh[51].loadOBJ("models/palm3.obj");
    mesh[52].loadOBJ("models/palm1.obj");
    mesh[53].loadOBJ("models/palm2.obj");
    mesh[54].loadOBJ("models/palm3.obj");
    mesh[55].loadOBJ("models/palm1.obj");

    mesh[56].loadOBJ("models/kraken.obj");
    mesh[57].loadOBJ("models/kraken.obj");
    mesh[58].loadOBJ("models/kraken.obj");
    mesh[59].loadOBJ("models/kraken.obj");

    mesh[60].loadOBJ("models/rock1.obj");
    mesh[61].loadOBJ("models/rock2.obj");
    mesh[62].loadOBJ("models/rock3.obj");
    mesh[63].loadOBJ("models/rock4.obj");

    mesh[64].loadOBJ("models/canon_ball.obj");
    mesh[65].loadOBJ("models/shark.obj");

    mesh[66].loadOBJ("models/lantern.obj");
    mesh[67].loadOBJ("models/lantern.obj");
    mesh[68].loadOBJ("models/lantern.obj");
    mesh[69].loadOBJ("models/lantern.obj");

    mesh[70].loadOBJ("models/lantern_stand.obj");
    mesh[71].loadOBJ("models/lantern_stand.obj");

    mesh[72].loadOBJ("models/bones.obj");
    mesh[73].loadOBJ("models/rock1.obj");
    mesh[74].loadOBJ("models/rock4.obj");
    mesh[75].loadOBJ("models/skull.obj");
    mesh[76].loadOBJ("models/rock1.obj");
    mesh[77].loadOBJ("models/rock2.obj");


    mesh[78].loadOBJ("models/palm1.obj");
    mesh[79].loadOBJ("models/palm2.obj");
    mesh[80].loadOBJ("models/palm3.obj");
    mesh[81].loadOBJ("models/palm1.obj");
    mesh[82].loadOBJ("models/palm2.obj");
    mesh[83].loadOBJ("models/palm3.obj");
    mesh[84].loadOBJ("models/palm1.obj");
    mesh[85].loadOBJ("models/palm2.obj");
    mesh[86].loadOBJ("models/palm3.obj");
    mesh[87].loadOBJ("models/palm1.obj");
    mesh[88].loadOBJ("models/palm2.obj");
    mesh[89].loadOBJ("models/palm3.obj");
    mesh[90].loadOBJ("models/palm1.obj");
    mesh[91].loadOBJ("models/palm2.obj");
    mesh[92].loadOBJ("models/palm3.obj");
    mesh[93].loadOBJ("models/palm1.obj");
    mesh[94].loadOBJ("models/palm2.obj");
    mesh[95].loadOBJ("models/palm3.obj");
    mesh[96].loadOBJ("models/palm1.obj");
    mesh[97].loadOBJ("models/palm2.obj");

    mesh[98].loadOBJ("models/lantern.obj");
    mesh[99].loadOBJ("models/lantern.obj");
    mesh[100].loadOBJ("models/lantern.obj");




    texture[0].loadTexture("textures/Atlas_Pirate.png", true);
    texture[1].loadTexture("textures/sea2.jpg", true);
    texture[2].loadTexture("textures/sand2.jpg", true);

    for (int i = 3; i <= 65; i++) {
        texture[i].loadTexture("textures/Atlas_Pirate.png", true);
    }
    for (int i = 66; i <= 71; i++) {
        texture[i].loadTexture("textures/lantern.png", true);
    }
    for (int i = 72; i <= 97; i++) {
        texture[i].loadTexture("textures/Atlas_Pirate.png", true);
    }
    for (int i = 98; i <= 100; i++) {
        texture[i].loadTexture("textures/lantern.png", true);
    }

    // Model positions
    glm::vec3 modelPos[] = {
        glm::vec3(13.0f, 0.0f, 2.0f),	// pirate
        glm::vec3(0.0f, -1.0f, 0.0f),	// sea
        glm::vec3(0.0f, 0.0f, 0.0f),	// island

        glm::vec3(20.0f, -0.5f, 0.0f),	// dock1
        glm::vec3(22.0f, -0.5f, 0.0f),	// dock2
        glm::vec3(24.0f, -0.5f, 0.0f),	// dock3
        glm::vec3(26.0f, -0.5f, 0.0f),	// dock4
        glm::vec3(28.0f, -0.5f, 0.0f),	// dock5
        glm::vec3(18.0f, -0.5f, 0.0f),	// dock6

        glm::vec3(33.0f, -0.2f, 0.0f),	// ship
        glm::vec3(33.0f, 0.8f, -1.0f),	// henry
        glm::vec3(7.0f, 0.0f, 0.0f),	// house
        glm::vec3(-21.5f, -1.5f, 5.0f),	// boat
        glm::vec3(-13.5f, 0.0f, 6.0f),	// anne
        glm::vec3(-12.0f, 0.0f, 5.5f),	// skeleton
        glm::vec3(-10.0f, 0.0f, 6.0f),	// chest

        // 40 palms
        glm::vec3(5.0f, 0.0f, 5.0f),
        glm::vec3(-7.0f, 0.0f, 4.0f),
        glm::vec3(12.0f, 0.0f, -6.0f),
        glm::vec3(-4.0f, 0.0f, -12.0f),
        glm::vec3(2.0f, 0.0f, -15.0f),
        glm::vec3(-15.0f, 0.0f, 10.0f),
        glm::vec3(10.0f, 0.0f, 12.0f),
        glm::vec3(-12.0f, 0.0f, -8.0f),
        glm::vec3(4.0f, 0.0f, -3.0f),
        glm::vec3(-2.0f, 0.0f, 14.0f),
        glm::vec3(-4.0f, 0.0f, 3.0f),
        glm::vec3(-6.0f, 0.0f, 8.0f),
        glm::vec3(3.0f, 0.0f, -8.0f),
        glm::vec3(-10.0f, 0.0f, -2.0f),
        glm::vec3(11.0f, 0.0f, 8.0f),
        glm::vec3(-13.0f, 0.0f, -4.0f),
        glm::vec3(-5.0f, 0.0f, -1.0f),
        glm::vec3(-5.0f, 0.0f, 15.0f),
        glm::vec3(15.0f, 0.0f, -5.0f),
        glm::vec3(-12.0f, 0.0f, -10.0f),
        glm::vec3(8.0f, 0.0f, 14.0f),
        glm::vec3(-11.0f, 0.0f, 12.0f),
        glm::vec3(14.0f, 0.0f, 4.0f),
        glm::vec3(-3.0f, 0.0f, -16.0f),
        glm::vec3(-1.0f, 0.0f, 7.0f),
        glm::vec3(0.0f, 0.0f, -4.0f),
        glm::vec3(6.0f, 0.0f, -15.0f),
        glm::vec3(-16.0f, 0.0f, 0.0f),
        glm::vec3(17.0f, 0.0f, -3.0f),
        glm::vec3(-8.0f, 0.0f, -13.0f),
        glm::vec3(2.0f, 0.0f, 2.0f),
        glm::vec3(-13.0f, 0.0f, 2.0f),
        glm::vec3(4.0f, 0.0f, 12.0f),
        glm::vec3(-1.0f, 0.0f, -9.0f),
        glm::vec3(14.0f, 0.0f, 10.0f),
        glm::vec3(9.0f, 0.0f, -10.0f),
        glm::vec3(-9.0f, 0.0f, 11.0f),
        glm::vec3(1.0f, 0.0f, 16.0f),
        glm::vec3(-7.0f, 0.0f, -5.0f),
        glm::vec3(4.0f, 0.0f, -14.0f),

        glm::vec3(28.0f, -1.0f, -5.0f), //kraken1
        glm::vec3(30.0f, -1.0f, 8.0f),  //kraken2
        glm::vec3(40.0f, -1.0f, 4.0f),  //kraken3
        glm::vec3(36.0f, -1.0f, -7.0f), //kraken4

        glm::vec3(10.0f, -1.0f, -14.0f),   //rock1
        glm::vec3(-10.0f, -1.0f, -17.0f),    //rock2
        glm::vec3(-19.0f, -1.0f, -5.0f),    //rock3
        glm::vec3(-14.0f, -1.0f, 20.0f),    //rock4

        glm::vec3(37.0f, 2.0f, 2.0f),    //canon ball
        glm::vec3(-26.0f, -1.0f, -19.0f),    //shark

        glm::vec3(25.0f, 0.5f, -1.12f),    //lantern1
        glm::vec3(21.0f, 0.5f, 1.15f),    //lantern2
        glm::vec3(33.0f, 2.0f, 9.3f),    //lantern3
        glm::vec3(33.0f, 2.75f, -5.7f),    //lantern4

        glm::vec3(12.0f, 0.0f, -4.0f),    //lantern stand1
        glm::vec3(8.0f, 0.0f, 5.0f),     //lantern stand2

        glm::vec3(-11.0f, 0.0f, 6.0f),	    // bone
        glm::vec3(-26.0f, -1.0f, -12.0f),     //rock1
        glm::vec3(-20.0f, -1.0f, -21.0f),     //rock4
        glm::vec3(-15.0f, 0.0f, -12.0f),     //skull
        glm::vec3(-15.0f, -1.0f, -20.0f),     //rock1
        glm::vec3(-22.0f, -1.0f, -8.0f),     //rock2

        // 20 palms
        glm::vec3(5.0f, 0.0f, 15.0f),
        glm::vec3(7.0f, 0.0f, 10.0f),
        glm::vec3(-1.0f, 0.0f, 10.0f),
        glm::vec3(-6.0f, 0.0f, 11.0f),
        glm::vec3(-4.0f, 0.0f, 6.0f),
        glm::vec3(-1.0f, 0.0f, -1.0f),
        glm::vec3(-7.0f, 0.0f, -10.0f),
        glm::vec3(-10.0f, 0.0f, -6.0f),
        glm::vec3(8.0f, 0.0f, -7.0f),
        glm::vec3(0.0f, 0.0f, -6.0f),
        glm::vec3(-1.0f, 0.0f, -13.0f),
        glm::vec3(6.0f, 0.0f, -10.0f),
        glm::vec3(-8.0f, 0.0f, 0.0f),
        glm::vec3(-5.0f, 0.0f, -8.0f),
        glm::vec3(3.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, 4.0f),
        glm::vec3(7.0f, 0.0f, -12.0f),
        glm::vec3(9.0f, 0.0f, 16.0f),
        glm::vec3(-2.0f, 0.0f, -12.0f),
        glm::vec3(3.0f, 0.0f, 8.0f),
            
        glm::vec3(-22.5f, -0.8f, 5.0f),    //lantern5
        glm::vec3(-25.0f, 1.2f, -13.0f),    //lantern6
        glm::vec3(-16.0f, 0.1f, -18.0f)    //lantern7
    };

    // Model scale
    glm::vec3 modelScale[] = {
        glm::vec3(1.0f, 1.0f, 1.0f),	// pirate
        glm::vec3(5.0f, 5.0f, 5.0f),	// sea
        glm::vec3(3.0f, 3.0f, 3.0f),	// island

        glm::vec3(1.0f, 1.0f, 1.0f),	// dock1
        glm::vec3(1.0f, 1.0f, 1.0f),	// dock2
        glm::vec3(1.0f, 1.0f, 1.0f), 	// dock3
        glm::vec3(1.0f, 1.0f, 1.0f), 	// dock4
        glm::vec3(1.0f, 1.0f, 1.0f), 	// dock5
        glm::vec3(1.0f, 1.0f, 1.0f), 	// dock6

        glm::vec3(1.0f, 1.0f, 1.0f), 	// ship
        glm::vec3(1.0f, 1.0f, 1.0f), 	// henry
        glm::vec3(1.0f, 1.0f, 1.0f), 	// house
        glm::vec3(1.0f, 1.0f, 1.0f), 	// boat
        glm::vec3(1.0f, 1.0f, 1.0f), 	// anne
        glm::vec3(1.0f, 1.0f, 1.0f), 	// skeleton
        glm::vec3(1.0f, 1.0f, 1.0f), 	// chest

        // 40 palms
        glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f),
        glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f),
        glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f),
        glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f),
        glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f),
        glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f),
        glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f),
        glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f),
        glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f),
        glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f),
        glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f),
        glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f),
        glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f),
        glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f),
        glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f),
        glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f),
        glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f),
        glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f),
        glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f),
        glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f),

        glm::vec3(0.3f, 0.3f, 0.3f),    //kraken1
        glm::vec3(0.3f, 0.3f, 0.3f),    //kraken2
        glm::vec3(0.3f, 0.3f, 0.3f),    //kraken3
        glm::vec3(0.3f, 0.3f, 0.3f),    //kraken4

        glm::vec3(1.0f, 1.0f, 1.0f),    //rock1
        glm::vec3(1.0f, 1.0f, 1.0f),    //rock2
        glm::vec3(1.0f, 1.0f, 1.0f),    //rock3
        glm::vec3(1.0f, 1.0f, 1.0f),    //rock4

        glm::vec3(1.0f, 1.0f, 1.0f),    //canon ball
        glm::vec3(1.0f, 1.0f, 1.0f),    //shark

        glm::vec3(0.5f, 0.5f, 0.5f),    //lantern1
        glm::vec3(0.5f, 0.5f, 0.5f),    //lantern2
        glm::vec3(0.5f, 0.5f, 0.5f),    //lantern3
        glm::vec3(0.5f, 0.5f, 0.5f),    //lantern4

        glm::vec3(1.0f, 1.0f, 1.0f),    //lantern stand1
        glm::vec3(1.0f, 1.0f, 1.0f),     //lantern stand2

        glm::vec3(1.0f, 1.0f, 1.0f), 	// bones
        glm::vec3(1.0f, 1.0f, 1.0f), 	// rock1
        glm::vec3(1.0f, 1.0f, 1.0f), 	// rock4
        glm::vec3(1.0f, 1.0f, 1.0f), 	// skull
        glm::vec3(1.0f, 1.0f, 1.0f), 	// rock1
        glm::vec3(1.0f, 1.0f, 1.0f), 	// rock2

        //20 palms
        glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f),
        glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f),
        glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f),
        glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f),
        glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f),
        glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f),
        glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f),
        glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f),
        glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f),
        glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f),


        glm::vec3(0.5f, 0.5f, 0.5f),    //lantern5
        glm::vec3(0.5f, 0.5f, 0.5f),    //lantern6
        glm::vec3(0.5f, 0.5f, 0.5f)    //lantern7
    };

    ma_engine engine;
    ma_result result = ma_engine_init(NULL, &engine);

    if (result != MA_SUCCESS) {
        std::cerr << "Erreur d'initialisation du moteur audio." << std::endl;
    }
    else {
        // Lance la musique (.wav ou .mp3 sont gérés nativement)
        ma_engine_play_sound(&engine, "audio/pirate_theme.mp3", NULL);
    }

    cinematicStartTime = glfwGetTime();
    double lastTime = glfwGetTime();
    float angle = 0.0f;


    // Rendering loop
    while (!glfwWindowShouldClose(gWindow))
    {
        showFPS(gWindow);

        double currentTime = glfwGetTime();
        double deltaTime = currentTime - lastTime;

        // Poll for and process events
        glfwPollEvents();
        update(deltaTime);

        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 model(1.0), view(1.0), projection(1.0);

        // Create the View matrix
        view = fpsCamera.getViewMatrix();

        // Create the projection matrix
        projection = glm::perspective(glm::radians(fpsCamera.getFOV()), (float)WIDTH / (float)HEIGHT, 0.1f, 200.0f);

        glm::vec3 viewPos;
        viewPos.x = fpsCamera.getPosition().x;
        viewPos.y = fpsCamera.getPosition().y;
        viewPos.z = fpsCamera.getPosition().z;

        //the light
        glm::vec3 lightPos(0.0f, 1.0f, 10.0f);
        glm::vec3 moonColor = glm::vec3(0.15f, 0.25f, 0.45f) * 0.3f;
        glm::vec3 lightDirection(-0.3f, -0.8f, -0.3f);
        glm::vec3 lanternColor = glm::vec3(1.0f, 0.6f, 0.2f) * 0.8f ;
        glm::vec3 lanternPositions[] = {
            modelPos[66],
            modelPos[67],
            modelPos[68],
            modelPos[69],
            glm::vec3(12.0f, 2.0f, -3.0f),
            glm::vec3(9.0f, 2.0f, 5.0f),
            modelPos[98],
            modelPos[99],
            modelPos[100],
        };

        // Must be called BEFORE setting uniforms because setting uniforms is done
        // on the currently active shader program.
        lightingShader.use();

        // Pass the matrices to the shader
        lightingShader.setUniform("view", view);
        lightingShader.setUniform("projection", projection);
        lightingShader.setUniform("viewPos", viewPos);

        //Sky
        lightingShader.setUniform("dirLight.ambient", glm::vec3(0.015f, 0.025f, 0.05f));
        lightingShader.setUniform("dirLight.diffuse", moonColor);
        lightingShader.setUniform("dirLight.specular", glm::vec3(0.1f, 0.15f, 0.2f));
        lightingShader.setUniform("dirLight.direction", lightDirection);

        //Lanterns
        for (int i = 0; i < 9; i++) {
            std::string number = std::to_string(i);
            std::string prefix = "pointLights[" + number + "].";
            glm::vec3 currentLanternPos = lanternPositions[i];
            currentLanternPos.y += 0.5f;

            float t = (float)currentTime;
            float phase = (float)i * 1.5f;

            float flicker = 0.8f
                + 0.35f * std::sin(t * 3.1f + phase)
                + 0.15f * std::sin(t * 7.8f + phase * 1.3f)
                + 0.10f * std::sin(t * 15.3f + phase * 2.7f);

            flicker = (std::max)(flicker, 0.1f);

            glm::vec3 animatedDiffuse = lanternColor * flicker;
            glm::vec3 animatedSpecular = glm::vec3(0.3f, 0.2f, 0.1f) * flicker;

            lightingShader.setUniform((prefix + "position").c_str(), currentLanternPos);
            lightingShader.setUniform((prefix + "ambient").c_str(), glm::vec3(0.05f, 0.05f, 0.05f));
            lightingShader.setUniform((prefix + "diffuse").c_str(), animatedDiffuse);
            lightingShader.setUniform((prefix + "specular").c_str(), animatedSpecular);
            lightingShader.setUniform((prefix + "constant").c_str(), 1.0f);
            lightingShader.setUniform((prefix + "linear").c_str(), 0.045f);
            lightingShader.setUniform((prefix + "exponent").c_str(), 0.016f);
        }

        //Chest
        lightingShader.setUniform("pointLights[9].position", glm::vec3(-10.0f, 0.8f, 6.0f));
        lightingShader.setUniform("pointLights[9].ambient", glm::vec3(0.0f));
        lightingShader.setUniform("pointLights[9].diffuse", glm::vec3(1.0f, 0.8f, 0.0f));
        lightingShader.setUniform("pointLights[9].specular", glm::vec3(1.0f, 0.9f, 0.5f));
        lightingShader.setUniform("pointLights[9].constant", 1.0f);
        lightingShader.setUniform("pointLights[9].linear", 0.09f);
        lightingShader.setUniform("pointLights[9].exponent", 0.032f);

        //Canon
        lightingShader.setUniform("pointLights[10].position", glm::vec3(35.0f, 1.5f, 0.8f));
        lightingShader.setUniform("pointLights[10].ambient", glm::vec3(0.0f));
        lightingShader.setUniform("pointLights[10].diffuse", glm::vec3(3.0f, 1.2f, 0.4f));
        lightingShader.setUniform("pointLights[10].specular", glm::vec3(1.0f, 0.5f, 0.2f));
        lightingShader.setUniform("pointLights[10].constant", 1.0f);
        lightingShader.setUniform("pointLights[10].linear", 0.09f);
        lightingShader.setUniform("pointLights[10].exponent", 0.032f);

        // Render the scene
        for (int i = 0; i < numModels; i++)
        {
            model = glm::translate(glm::mat4(1.0), modelPos[i]) * glm::scale(glm::mat4(1.0), modelScale[i]);
            if (i == 3 || i == 4 || i == 5 || i == 6 || i == 7 || i == 8 || i == 12) {
                model *= glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
            }
            if (i == 9 || i == 11  || i == 71) {
                model *= glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
            }
            if (i == 10 || i == 59 || i == 14) {
                model *= glm::rotate(glm::mat4(1.0f), glm::radians(-45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
            }
            if (i == 15 || i == 72) {
                model *= glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
            }
            if (i == 12) {
                model *= glm::rotate(glm::mat4(1.0f), glm::radians(-10.0f), glm::vec3(0.0f, 0.0f, 1.0f));
            }
            if (i == 56) {
                model *= glm::rotate(glm::mat4(1.0f), glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
            }
            if (i == 57 || i == 13) {
                model *= glm::rotate(glm::mat4(1.0f), glm::radians(135.0f), glm::vec3(0.0f, 1.0f, 0.0f));
            }
            if (i == 58) {
                model *= glm::rotate(glm::mat4(1.0f), glm::radians(-135.0f), glm::vec3(0.0f, 1.0f, 0.0f));
            }
            if (i == 75) {
                model *= glm::rotate(glm::mat4(1.0f), glm::radians(-110.0f), glm::vec3(0.0f, 1.0f, 0.0f));
            }
            if (i == 65) {
                model *= glm::rotate(glm::mat4(1.0f), glm::radians(60.0f), glm::vec3(0.0f, 1.0f, 0.0f));
            }
            if (i == 0) {
                model *= glm::rotate(glm::mat4(1.0f), glm::radians(110.0f), glm::vec3(0.0f, 1.0f, 0.0f));
            }
            if ((i >= 16 && i <= 55) || (i >= 78 && i <= 97)) {
                float randomAngle = (float)((i * 113) % 360);
                model *= glm::rotate(glm::mat4(1.0f), glm::radians(randomAngle), glm::vec3(0.0f, 1.0f, 0.0f));
            }

            lightingShader.use();

            lightingShader.setUniform("model", model);

            lightingShader.setUniform("material.ambient", glm::vec3(0.1f, 0.1f, 0.1f));
            lightingShader.setUniformSampler("material.diffuseMap", 0);
            lightingShader.setUniform("material.specular", glm::vec3(0.5f, 0.5f, 0.5f));
            lightingShader.setUniform("material.shininess", 32.0f);

            texture[i].bind(0);		// set the texture before drawing.  Our simple OBJ mesh loader does not do materials yet.
            mesh[i].draw();			// Render the OBJ mesh
            texture[i].unbind(0);
        }
   

        // Swap front and back buffers
        glfwSwapBuffers(gWindow);

        lastTime = currentTime;
    }
    ma_engine_uninit(&engine);
    glfwTerminate();

    return 0;
}

bool initOpenGL() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    
    gWindow = glfwCreateWindow(WIDTH, HEIGHT, TITLE, NULL, NULL);

    if (gWindow == NULL) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(gWindow);

    glfwSetKeyCallback(gWindow, glfw_onKey);
    glfwSetCursorPosCallback(gWindow, glfw_onMouseMove);
    glfwSetScrollCallback(gWindow, glfw_onMouseScroll);

    glfwSetInputMode(gWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPos(gWindow, WIDTH / 2.0, HEIGHT / 2.0);
    
    glewExperimental = GL_TRUE;
    if(glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        glfwTerminate();
        return false;
    }

    glClearColor(0.02f, 0.04f, 0.1f, 1.0f);

    glViewport(0, 0, WIDTH, HEIGHT);

	glEnable(GL_DEPTH_TEST);

    return true;
}

void glfw_onKey(GLFWwindow* window, int key, int scancode, int action, int mode) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }

    if (key == GLFW_KEY_T && action == GLFW_PRESS) {
        gWireFrame = !gWireFrame;
        if (gWireFrame) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        } else {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }
    }

    if (key == GLFW_KEY_F && action == GLFW_PRESS) {
        gFlashlightOn = !gFlashlightOn;
    }

    
}

void glfw_OnFrameBufferSize(GLFWwindow* window, int width, int height) {
    WIDTH = width;
    HEIGHT = height;
    glViewport(0, 0, WIDTH, HEIGHT);
}

void glfw_onMouseMove(GLFWwindow* window, double posX, double posY) {

    //static glm::vec2 lastMousePos = glm::vec2(0, 0);

    //if (glfwGetMouseButton(gWindow, GLFW_MOUSE_BUTTON_LEFT) == 1) {
    //    gYaw -= ((float)posX - lastMousePos.x) * MOUSE_SENSITIVITY;
    //    gPitch += ((float)posY - lastMousePos.y) * MOUSE_SENSITIVITY;
    //}

    //if (glfwGetMouseButton(gWindow, GLFW_MOUSE_BUTTON_RIGHT) == 1) {
    //    float dx = 0.01f * ((float)posX - lastMousePos.x);
    //    float dy = 0.01f * ((float)posY - lastMousePos.y);
    //    gRadius += dx - dy;
    //}

    //lastMousePos.x = (float)posX;
    //lastMousePos.y = (float)posY;
}

void glfw_onMouseScroll(GLFWwindow* window, double deltaX, double deltaY) {
    double fov = fpsCamera.getFOV() + deltaY * ZOOM_SENSITIVITY;

    fov = glm::clamp(fov, 1.0, 120.0);

    fpsCamera.setFOV((float)fov);
}

void update(double elapsedTime) {
    double currentTime = glfwGetTime();

    if (gCinematicMode) {
        float t = (float)(currentTime - cinematicStartTime);

        if (t < 5.0f) {
            glm::vec3 pos = cinematicMoveTo(glm::vec3(0.0f, 15.0f, -40.0f), glm::vec3(0.0f, 5.0f, -10.0f), 0.0f, 5.0f, t);
            fpsCamera.setPosition(pos);

            float yaw, pitch;
            cinematicLookAt(pos, glm::vec3(33.0f, -0.2f, 0.0f), yaw, pitch);
            fpsCamera.setRotation(yaw, pitch);
        }
        else if (t < 12.0f) {
            glm::vec3 shipPos(33.0f, 5.0f, 0.0f);
            glm::vec3 orbitPos = cinematicOrbit(shipPos, 20.0f, 0.5f, t - 5.0f);

            orbitPos.y += (t - 5.0f) * 2.0f;

            fpsCamera.setPosition(orbitPos);

            float yaw, pitch;
            cinematicLookAt(orbitPos, shipPos, yaw, pitch);
            fpsCamera.setRotation(yaw, pitch);
        }
        else if (t < 20.0f) {
            glm::vec3 pos = cinematicMoveTo(glm::vec3(43.0f, 2.5f, 0.0f), glm::vec3(11.0f, 1.0f, 0.0f), 12.0f, 8.0f, t);
            fpsCamera.setPosition(pos);

            float yaw, pitch;
            cinematicLookAt(pos, glm::vec3(9.0f, 1.0f, 0.0f), yaw, pitch);
            fpsCamera.setRotation(yaw, pitch);
        }
        else if (t < 26.0f) {
            glm::vec3 bonePos(-11.0f, 1.0f, 6.0f);
            glm::vec3 orbitPos = cinematicOrbit(bonePos, 10.0f, 0.5f, t - 20.0f);

            orbitPos.y += (t - 20.0f) * 2.0f;

            fpsCamera.setPosition(orbitPos);

            float yaw, pitch;
            cinematicLookAt(orbitPos, bonePos, yaw, pitch);
            fpsCamera.setRotation(yaw, pitch);
        }
        else if (t < 33.0f) {
            glm::vec3 pos = cinematicMoveTo(glm::vec3(-31.0f, 3.0f, -2.0f), glm::vec3(-12.0f, 0.8f, 6.0f), 26.0f, 7.0f, t);
            fpsCamera.setPosition(pos);

            float yaw, pitch;
            cinematicLookAt(pos, glm::vec3(-10.0f, 0.8f, 6.0f), yaw, pitch);
            fpsCamera.setRotation(yaw, pitch);
        }
        else if (t < 40.0f) {
            glm::vec3 sharkPos(-19.0f, 5.0f, -15.0f);
            glm::vec3 orbitPos = cinematicOrbit(sharkPos, 20.0f, 0.5f, t - 33.0f);

            orbitPos.y += (t - 33.0f) * 2.0f;

            fpsCamera.setPosition(orbitPos);

            float yaw, pitch;
            cinematicLookAt(orbitPos, sharkPos, yaw, pitch);
            fpsCamera.setRotation(yaw, pitch);
        }
        else if (t < 46.0f) {
            glm::vec3 pos = cinematicMoveTo(glm::vec3(-34.0f, -0.8f, -20.0f), glm::vec3(-20.0f, 1.1f, -14.0f), 40.0f, 6.0f, t);
            fpsCamera.setPosition(pos);

            float yaw, pitch;
            cinematicLookAt(pos, glm::vec3(-15.0f, 1.7f, -12.0f), yaw, pitch);
            fpsCamera.setRotation(yaw, pitch);
        }
        else if (t < 60.0f) {
            glm::vec3 centerPos(0.0f, 0.0f, 0.0f);
            glm::vec3 orbitPos = cinematicOrbit(centerPos, 40.0f, 0.5f, t - 46.0f);

            orbitPos.y += (t - 46.0f) * 2.0f;

            fpsCamera.setPosition(orbitPos);

            float yaw, pitch;
            cinematicLookAt(orbitPos, centerPos, yaw, pitch);
            fpsCamera.setRotation(yaw, pitch);
        }
        else {
            gCinematicMode = false;
        }
    }
    else {
        double mouseX, mouseY;

        glfwGetCursorPos(gWindow, &mouseX, &mouseY);

        fpsCamera.rotate((float)(WIDTH / 2.0 - mouseX) * MOUSE_SENSITIVITY, (float)(HEIGHT / 2.0 - mouseY) * MOUSE_SENSITIVITY);

        glfwSetCursorPos(gWindow, WIDTH / 2.0, HEIGHT / 2.0);

        if (glfwGetKey(gWindow, GLFW_KEY_W) == GLFW_PRESS) {
            fpsCamera.move(MOVE_SPEED * (float)elapsedTime * fpsCamera.getLook());
        }
        else if (glfwGetKey(gWindow, GLFW_KEY_S) == GLFW_PRESS) {
            fpsCamera.move(MOVE_SPEED * (float)elapsedTime * -fpsCamera.getLook());
        }

        if (glfwGetKey(gWindow, GLFW_KEY_A) == GLFW_PRESS) {
            fpsCamera.move(MOVE_SPEED * (float)elapsedTime * -fpsCamera.getRight());
        }
        else if (glfwGetKey(gWindow, GLFW_KEY_D) == GLFW_PRESS) {
            fpsCamera.move(MOVE_SPEED * (float)elapsedTime * fpsCamera.getRight());
        }

        if (glfwGetKey(gWindow, GLFW_KEY_SPACE) == GLFW_PRESS) {
            fpsCamera.move(MOVE_SPEED * (float)elapsedTime * fpsCamera.getUp());
        }
        else if (glfwGetKey(gWindow, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
            fpsCamera.move(MOVE_SPEED * (float)elapsedTime * -fpsCamera.getUp());
        }
    }
}   



void showFPS(GLFWwindow* window) {
    static double previousSeconds = 0.0;
    static int frameCount = 0;
    double elapsedSeconds;
    double currentSeconds = glfwGetTime(); //returns number of seconds since GLFW started, as double 

    elapsedSeconds = currentSeconds - previousSeconds;
    // Limit text updates to 4 times per second
    if (elapsedSeconds > 0.25) {
        previousSeconds = currentSeconds;
        double fps = (double)frameCount / elapsedSeconds;
        double msPerFrame = 1000.0 / fps;

        glm::vec3 v;
        v.x = fpsCamera.getPosition().x;
        v.y = fpsCamera.getPosition().y;
        v.z = fpsCamera.getPosition().z;

        std::ostringstream outs;
        outs.precision(3); //decimal places
        outs << std::fixed
            << TITLE << "   "
            << "FPS: " << fps << "   "
            << "Frame Time: " << msPerFrame << " (ms)     "
            << v.x << " / " << v.y << " / " << v.z;
        glfwSetWindowTitle(window, outs.str().c_str());

        frameCount = 0;
    }

    frameCount++;
}

glm::vec3 cinematicMoveTo(glm::vec3 startPos, glm::vec3 endPos, float startTime, float duration, float currentTime) {
    float t = (currentTime - startTime) / duration;
    t = glm::clamp(t, 0.0f, 1.0f);
    t = t * t * (3.0f - 2.0f * t);
    return glm::mix(startPos, endPos, t);
}

void cinematicLookAt(glm::vec3 cameraPos, glm::vec3 targetPos, float& outYaw, float& outPitch) {
    glm::vec3 dir = glm::normalize(targetPos - cameraPos);
    outPitch = glm::degrees(asin(dir.y));
    outYaw = glm::degrees(atan2(dir.x, dir.z));
}

glm::vec3 cinematicOrbit(glm::vec3 center, float radius, float speed, float currentTime) {
    float x = center.x + radius * std::sin(currentTime * speed);
    float z = center.z + radius * std::cos(currentTime * speed);
    return glm::vec3(x, center.y, z);
}

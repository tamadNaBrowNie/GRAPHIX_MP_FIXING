#include "main.h"
#include "Classes/Classes.h"
/*
    Global Variables
*/
// Screen width and height
float screenWidth = 1000.0f;
float screenHeight = 1000.0f;

// Camera Positioning and Movement
float cameraSpeed = 0.1f;
glm::vec3 tps_cameraPos = glm::vec3(0.0f, 0.0f, 1.0f);
glm::vec3 fps_cameraPos = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 playerPos = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 td_cameraPos = playerPos; // NOTE: Top-down camera resets to player position on toggle
glm::vec3 worldUp = glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f));

// Camera Rotation (yaw and pitch)
bool    firstMouse = true;
float   lastX = 500.0f, lastY = 500.0f,
yaw = -90.0f, pitch = 0.0f,
sensitivity = 0.1f;

void Key_Callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    
}

void Mouse_Callback(GLFWwindow* window, double xpos, double ypos)
{
    /*
        Placing the cursor to the center of the screen (500.0f, 500.0f)
        when the user clicks on the window
    */
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    /*
        Calculating offsets on the X and Y-axis movement (according to a set sensitivity),
        as well as saving the last X and Y position of the cursor in case the window becomes out of focus.
    */
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;
    xoffset *= sensitivity;
    yoffset *= sensitivity;
    yaw += xoffset;
    pitch += yoffset;

    /*
        Setting limits on how tilted upwards or downwards can the camera be.
    */
    if (pitch > 89.0f) {
        pitch = 89.0f;
    }

    if (pitch < -89.0f) {
        pitch = -89.0f;
    }

    /*
        Updating the position of the camera.
    */
    glm::vec3 direction = glm::vec3(
        (cos(glm::radians(yaw)) * cos(glm::radians(pitch))),
        (sin(glm::radians(pitch))),
        (sin(glm::radians(yaw)) * cos(glm::radians(pitch)))
    );

    tps_cameraPos = playerPos - glm::normalize(direction);
}

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(screenWidth, screenHeight, "No Man's Submarine", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    // -------------------------------------------------------
    // LOADING OBJECTS

    ModelClass playerSub = ModelClass("3D/submarine/submarine.obj");
    ModelClass donut = ModelClass("3D/donut/Donut.obj");
    
    // -------------------------------------------------------
    // SETTING SKYBOX VERTICES AND INDICES
    /*
        These vertices and indices were taken from previous individual submissions
        and class hands-on activities
    */
    // Vertices for the cube
    float skyboxVertices[]{
        -1.f, -1.f, 1.f, 
        1.f, -1.f, 1.f,  
        1.f, -1.f, -1.f, 
        -1.f, -1.f, -1.f,
        -1.f, 1.f, 1.f,  
        1.f, 1.f, 1.f,   
        1.f, 1.f, -1.f,  
        -1.f, 1.f, -1.f  
    };

    // Skybox Indices
    unsigned int skyboxIndices[]{
        1,2,6,
        6,5,1,

        0,4,7,
        7,3,0,

        4,5,6,
        6,7,4,

        0,3,2,
        2,1,0,

        0,1,5,
        5,4,0,

        3,7,6,
        6,2,3
    };

    // -------------------------------------------------------

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    // Initialize GLAD
    gladLoadGL();

    // Setting up input reading:
    // - for keyboard inputs
    glfwSetKeyCallback(window, Key_Callback);
    // - for mouse movement inputs
    glfwSetCursorPosCallback(window, Mouse_Callback);
    // Disables cursor when mouse input is used
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // -------------------------------------------------------
    // LOADING TEXTURES
    /*
        In calling the attachTexture() and attachNormalTexture() function, take note of the order:
            1st - base textures
            2nd - normal textures
    */
    // Player submarine base textures
    playerSub.attachTexture("3D/submarine/submarine_submarine_BaseColor.png", GL_RGB);

    // Player submarine normal textures
    playerSub.attachNormalTexture("3D/submarine/submarine_submarine_Normal.png", GL_RGB);

    // Donut base textures
    donut.attachTexture("3D/donut/Gus.png", GL_RGBA);

    // Enable depth test
    glEnable(GL_DEPTH_TEST);

    // -------------------------------------------------------
    // CREATING OBJECT SHADERS

    ShaderClass obj_shaderProgram = ShaderClass("Shaders/objVert.vert", "Shaders/objFrag.frag");

    // -------------------------------------------------------
    // CREATING SKYBOX SHADERS

    ShaderClass skybox_shaderProgram = ShaderClass("Shaders/skybox.vert", "Shaders/skybox.frag");

    // -------------------------------------------------------
    // LOADING SKYBOX TEXTURES

    std::string facesSkybox[]{
        "Skybox/uw_rt.jpg",    // RIGHT
        "Skybox/uw_lf.jpg",    // LEFT
        "Skybox/uw_up.jpg",    // UP
        "Skybox/uw_dn.jpg",    // DOWN
        "Skybox/uw_bk.jpg",    // BACK
        "Skybox/uw_ft.jpg",    // FRONT
    };

    unsigned int skyboxTex;

    glGenTextures(1, &skyboxTex);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTex);

    // Prevent pixelating
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    // Prevents tiling
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    for (unsigned int i = 0; i < 6; i++) {
        int w, h, skyCChannel;
        stbi_set_flip_vertically_on_load(false);

        unsigned char* data = stbi_load(
            facesSkybox[i].c_str(),
            &w,
            &h,
            &skyCChannel,
            0
        );

        if (data) {
            glTexImage2D(
                GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                0,
                GL_RGB,
                w,
                h,
                0,
                GL_RGB,
                GL_UNSIGNED_BYTE,
                data
            );
        }

        stbi_image_free(data);
    }

    stbi_set_flip_vertically_on_load(true);

    // -------------------------------------------------------
    // CREATING OBJECT VAOs and VBOs

    playerSub.createVAO_VBO();
    donut.createVAO_VBO();

    // -------------------------------------------------------
    // CREATING SKYBOX VAO, VBO, and EBO

    unsigned int skyboxVAO, skyboxVBO, skyboxEBO;
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glGenBuffers(1, &skyboxEBO);

    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(skyboxVertices),
        &skyboxVertices,
        GL_STATIC_DRAW
    );

    glVertexAttribPointer(0,
        3,
        GL_FLOAT,
        GL_FALSE,
        3 * sizeof(GL_FLOAT),
        (void*)0
    );

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, skyboxEBO);
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        sizeof(GL_INT) * 36,
        &skyboxIndices,
        GL_STATIC_DRAW
    );

    glEnableVertexAttribArray(0);

    // -------------------------------------------------------
    // OTHER VARIABLES

    long int    before = 0;
    long int    after;
    int         elapsed = 0;

    before = GetTickCount64();

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Initialize projection matrix
        glm::mat4 projectionMatrix = glm::perspective(glm::radians(90.0f), screenWidth / screenHeight, 0.1f, 100.0f);
        unsigned int projectionLoc = glGetUniformLocation(obj_shaderProgram.getShader(), "projection");
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

        // Initialize view matrix
        glm::mat4 viewMatrix = glm::lookAt(tps_cameraPos, playerPos, worldUp);
        unsigned int viewLoc = glGetUniformLocation(obj_shaderProgram.getShader(), "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(viewMatrix));

        // -----------------------------------------------------------------
        // RENDERING SKYBOX

        glDepthMask(GL_FALSE);
        glDepthFunc(GL_LEQUAL);

        glUseProgram(skybox_shaderProgram.getShader());
        glBindVertexArray(skyboxVAO);

        glm::mat4 skybox_view = glm::mat4(1.0f);
        skybox_view = glm::mat4(glm::mat3(viewMatrix));

        // unsigned int skybox_projectionLoc = glGetUniformLocation(skybox_shaderProgram, "projection");
        unsigned int skybox_projectionLoc = glGetUniformLocation(skybox_shaderProgram.getShader(), "projection");
        glUniformMatrix4fv(skybox_projectionLoc, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

        // unsigned int skybox_viewLoc = glGetUniformLocation(skybox_shaderProgram, "view");
        unsigned int skybox_viewLoc = glGetUniformLocation(skybox_shaderProgram.getShader(), "view");
        glUniformMatrix4fv(skybox_viewLoc, 1, GL_FALSE, glm::value_ptr(skybox_view));

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTex);

        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        glDepthMask(GL_TRUE);
        glDepthFunc(GL_LESS);

        // -----------------------------------------------------------------
        // RENDERING OBJECTS
        
        playerSub.draw(
            playerPos,                          // Object Position
            obj_shaderProgram.getShader(),      // Shader Program to use
            0.15f,                              // Scale
            90.0f, glm::vec3(0.0f, 1.0f, 0.0f)  // Rotation value and axis, respectively
        );

        donut.draw(
            glm::vec3(0.0f, -10.0f, -5.0f),
            obj_shaderProgram.getShader(),
            5.0f,    
            90.0f, glm::vec3(0.0f, 1.0f, 0.0f)
        );

        after = GetTickCount64();
        int temp = (after - before) / 1000;
        if (!(elapsed == temp)) {
            elapsed = temp;
            // Show elapsed time in console (refreshes after object spawn)
            cout << "Camera Position: (" << fps_cameraPos.x << ", " << fps_cameraPos.y << ", " << fps_cameraPos.z << ")\n";
        }

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    // Cleanup
    glDeleteVertexArrays(1, &skyboxVAO);
    glDeleteBuffers(1, &skyboxVBO);
    glDeleteBuffers(1, &skyboxEBO);

    glfwTerminate();
    return 0;
}
#include "main.h"
#include "Classes/ShaderClass.h"
/*
    Global Variables
*/
// Screen width and height
float screenWidth = 1000.0f;
float screenHeight = 1000.0f;

// Camera Positioning and Movement
float cameraSpeed = 0.1f;
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 1.0f);
glm::vec3 playerPos = glm::vec3(0.0f, 0.0f, 0.0f);
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
        Updating the position of the front of the camera.
    */
    glm::vec3 direction = glm::vec3(
        (cos(glm::radians(yaw)) * cos(glm::radians(pitch))),
        (sin(glm::radians(pitch))),
        (sin(glm::radians(yaw)) * cos(glm::radians(pitch)))
    );

    cameraPos = playerPos - glm::normalize(direction);
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

    // Loading .obj file
    std::string path = "3D/submarine/submarine.obj";
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warning, error;
    tinyobj::attrib_t attributes;

    bool mainObj_success = tinyobj::LoadObj(
        &attributes,
        &shapes,
        &materials,
        &warning,
        &error,
        path.c_str()
    );

    // Loading tangents and bitangents
    std::vector<glm::vec3> tangents;
    std::vector<glm::vec3> bitangents;

    for (int i = 0; i < shapes[0].mesh.indices.size(); i += 3) {
        tinyobj::index_t vData1 = shapes[0].mesh.indices[i];
        tinyobj::index_t vData2 = shapes[0].mesh.indices[i + 1];
        tinyobj::index_t vData3 = shapes[0].mesh.indices[i + 2];

        glm::vec3 v1 = glm::vec3(
            attributes.vertices[vData1.vertex_index * 3],
            attributes.vertices[vData1.vertex_index * 3 + 1],
            attributes.vertices[vData1.vertex_index * 3 + 2]
        );

        glm::vec3 v2 = glm::vec3(
            attributes.vertices[vData2.vertex_index * 3],
            attributes.vertices[vData2.vertex_index * 3 + 1],
            attributes.vertices[vData2.vertex_index * 3 + 2]
        );

        glm::vec3 v3 = glm::vec3(
            attributes.vertices[vData3.vertex_index * 3],
            attributes.vertices[vData3.vertex_index * 3 + 1],
            attributes.vertices[vData3.vertex_index * 3 + 2]
        );

        glm::vec2 uv1 = glm::vec2(
            attributes.texcoords[vData1.texcoord_index * 2],
            attributes.texcoords[vData1.texcoord_index * 2 + 1]
        );

        glm::vec2 uv2 = glm::vec2(
            attributes.texcoords[vData2.texcoord_index * 2],
            attributes.texcoords[vData2.texcoord_index * 2 + 1]
        );

        glm::vec2 uv3 = glm::vec2(
            attributes.texcoords[vData3.texcoord_index * 2],
            attributes.texcoords[vData3.texcoord_index * 2 + 1]
        );

        glm::vec3 deltaPos1 = v2 - v1;
        glm::vec3 deltaPos2 = v3 - v1;

        glm::vec2 deltaUV1 = uv2 - uv1;
        glm::vec2 deltaUV2 = uv3 - uv1;

        float r = 1.0f / ((deltaUV1.x * deltaUV2.y) - (deltaUV1.y * deltaUV2.x));

        glm::vec3 tangent = r * (deltaPos1 * deltaUV2.y - deltaPos2 * deltaUV1.y);
        glm::vec3 bitangent = r * (deltaPos2 * deltaUV1.x - deltaPos1 * deltaUV2.x);

        tangents.push_back(tangent);
        tangents.push_back(tangent);
        tangents.push_back(tangent);

        bitangents.push_back(bitangent);
        bitangents.push_back(bitangent);
        bitangents.push_back(bitangent);
    }

    // Loading vertex data
    std::vector<GLfloat> vertexData;
    for (int i = 0; i < shapes[0].mesh.indices.size(); i++) {
        tinyobj::index_t vData = shapes[0].mesh.indices[i];
        int vertexIndex = vData.vertex_index * 3;
        int normalIndex = vData.normal_index * 3;
        int uvIndex = vData.texcoord_index * 2;

        // ---------------------------------------------------
        // POSITION
        vertexData.push_back(
            attributes.vertices[vertexIndex]
        );

        vertexData.push_back(
            attributes.vertices[vertexIndex + 1]
        );

        vertexData.push_back(
            attributes.vertices[vertexIndex + 2]
        );

        // ---------------------------------------------------
        // NORMALS
        vertexData.push_back(
            attributes.normals[normalIndex]
        );

        vertexData.push_back(
            attributes.normals[normalIndex + 1]
        );

        vertexData.push_back(
            attributes.normals[normalIndex + 2]
        );

        // ---------------------------------------------------
        // TEXTURE COORDINATES
        vertexData.push_back(
            attributes.texcoords[uvIndex]
        );

        vertexData.push_back(
            attributes.texcoords[uvIndex + 1]
        );

        // ---------------------------------------------------
        // TANGENTS
        vertexData.push_back(
            tangents[i].x
        );
        vertexData.push_back(
            tangents[i].y
        );
        vertexData.push_back(
            tangents[i].z
        );

        // ---------------------------------------------------
        // BITANGENTS
        vertexData.push_back(
            bitangents[i].x
        );
        vertexData.push_back(
            bitangents[i].y
        );
        vertexData.push_back(
            bitangents[i].z
        );
    }

    // -------------------------------------------------------
    // SETTING SKYBOX VERTICES AND INDICES

    /*
      7--------6
     /|       /|
    4--------5 |
    | |      | |
    | 3------|-2
    |/       |/
    0--------1
    */
    // Vertices for the cube
    float skyboxVertices[]{
        -1.f, -1.f, 1.f, //0
        1.f, -1.f, 1.f,  //1
        1.f, -1.f, -1.f, //2
        -1.f, -1.f, -1.f,//3
        -1.f, 1.f, 1.f,  //4
        1.f, 1.f, 1.f,   //5
        1.f, 1.f, -1.f,  //6
        -1.f, 1.f, -1.f  //7
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
     
    // Flip image vertically on load
    stbi_set_flip_vertically_on_load(true);

    // Initialize variables for loading the texture
    int img_width, img_height, colorChannels;

    unsigned char* tex_bytes = stbi_load(
        "3D/submarine/submarine_submarine_BaseColor.png",
        &img_width, &img_height, &colorChannels,
        0
    );

    // Initialize texture variable
    GLuint texture;
    glGenTextures(1, &texture);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    // Attach loaded image to texture variable
    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGB,
        img_width,
        img_height,
        0,
        GL_RGB,
        GL_UNSIGNED_BYTE,
        tex_bytes
    );
    // Generate Mipmap
    glGenerateMipmap(GL_TEXTURE_2D);

    // Free up loaded bytes
    stbi_image_free(tex_bytes);

    unsigned char* norm_bytes = stbi_load(
        "3D/submarine/submarine_submarine_Normal.png",
        &img_width, &img_height, &colorChannels,
        0
    );

    // Initialize texture variable
    GLuint norm_tex;
    glGenTextures(1, &norm_tex);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, norm_tex);

    // Attach loaded image to texture variable
    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGB,
        img_width,
        img_height,
        0,
        GL_RGB,
        GL_UNSIGNED_BYTE,
        norm_bytes
    );
    // Generate Mipmap
    glGenerateMipmap(GL_TEXTURE_2D);

    // Free up loaded bytes
    stbi_image_free(norm_bytes);

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

    GLintptr normalPtr = 3 * sizeof(GL_FLOAT);
    GLintptr uvPtr = 6 * sizeof(GL_FLOAT);
    GLintptr tangentPtr = 8 * sizeof(GL_FLOAT);
    GLintptr bitangentPtr = 11 * sizeof(GL_FLOAT);

    GLuint VAO, VBO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(GL_FLOAT) * vertexData.size(),
        vertexData.data(),
        GL_STATIC_DRAW
    );

    // Vertices
    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        14 * sizeof(GL_FLOAT),
        (void*)0
    );

    // Normals
    glVertexAttribPointer(
        1,
        3,
        GL_FLOAT,
        GL_FALSE,
        14 * sizeof(GL_FLOAT),
        (void*)normalPtr
    );

    // Texture coordinates
    glVertexAttribPointer(
        2,
        2,
        GL_FLOAT,
        GL_FALSE,
        14 * sizeof(GL_FLOAT),
        (void*)uvPtr
    );

    // Tangents
    glVertexAttribPointer(
        3,
        3,
        GL_FLOAT,
        GL_FALSE,
        14 * sizeof(GL_FLOAT),
        (void*)tangentPtr
    );

    // Bitangents
    glVertexAttribPointer(
        4,
        3,
        GL_FLOAT,
        GL_FALSE,
        14 * sizeof(GL_FLOAT),
        (void*)bitangentPtr
    );

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);
    glEnableVertexAttribArray(4);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

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
        glm::mat4 viewMatrix = glm::lookAt(cameraPos, playerPos, worldUp);
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
        
        glUseProgram(obj_shaderProgram.getShader());
        glBindVertexArray(VAO);

        // Initialize transformation matrix, and assign position, scaling, and rotation
        glm::mat4 transformationMatrix = glm::translate(glm::mat4(1.0f), playerPos);
        transformationMatrix = glm::scale(transformationMatrix, glm::vec3(0.15f, 0.15f, 0.15f));
        transformationMatrix = glm::rotate(transformationMatrix, glm::radians(90.0f), glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f)));

        // Initialize transformation location, and assign transformation
        unsigned int transformationLoc = glGetUniformLocation(obj_shaderProgram.getShader(), "transform");
        glUniformMatrix4fv(transformationLoc, 1, GL_FALSE, glm::value_ptr(transformationMatrix));

        glActiveTexture(GL_TEXTURE0);
        GLuint tex0Address = glGetUniformLocation(obj_shaderProgram.getShader(), "tex0");
        glBindTexture(GL_TEXTURE_2D, texture);
        glUniform1i(tex0Address, 0);

        glActiveTexture(GL_TEXTURE1);
        GLuint tex1Address = glGetUniformLocation(obj_shaderProgram.getShader(), "norm_tex");
        glBindTexture(GL_TEXTURE_2D, norm_tex);
        glUniform1i(tex1Address, 1);

        // Draw
        glDrawArrays(GL_TRIANGLES, 0, vertexData.size() / 5);

        after = GetTickCount64();
        int temp = (after - before) / 1000;
        if (!(elapsed == temp)) {
            elapsed = temp;
            // Show elapsed time in console (refreshes after object spawn)
            cout << "Camera Position: (" << cameraPos.x << ", " << cameraPos.y << ", " << cameraPos.z << ")\n";
        }

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    // Cleanup
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &skyboxVAO);
    glDeleteBuffers(1, &skyboxVBO);
    glDeleteBuffers(1, &skyboxEBO);

    glfwTerminate();
    return 0;
}
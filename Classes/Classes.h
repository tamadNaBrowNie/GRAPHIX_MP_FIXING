#pragma once
#include "main.h"
enum class Mode {
    TPS, FPS, TD
};

class ShaderClass {
private:
    GLuint shaderProgram;

public:
    ShaderClass(std::string vertPath, std::string fragPath) {
        // Load .vert file
        std::fstream vertSrc(vertPath);
        std::stringstream vertBuff;
        vertBuff << vertSrc.rdbuf();
        std::string vertS = vertBuff.str();
        const char* v = vertS.c_str();

        // Load .frag file
        std::fstream fragSrc(fragPath);
        std::stringstream fragBuff;
        fragBuff << fragSrc.rdbuf();
        std::string fragS = fragBuff.str();
        const char* f = fragS.c_str();

        // Compile vertex shader
        GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertShader, 1, &v, NULL);
        glCompileShader(vertShader);

        // Compile fragment shader
        GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragShader, 1, &f, NULL);
        glCompileShader(fragShader);

        // Create shader program and link both vertex and fragment shaders
        this->shaderProgram = glCreateProgram();
        glAttachShader(this->shaderProgram, vertShader);
        glAttachShader(this->shaderProgram, fragShader);
        glLinkProgram(this->shaderProgram);
    }

    void use() {
        glUseProgram(shaderProgram);
    }

    GLuint getShader() {
        return this->shaderProgram;
    }

    GLint findUloc(const GLchar* src) {
        GLint unif = glGetUniformLocation(this->shaderProgram, src);
        std::cout << glGetError() << src<<'\n';
        return unif;
    }
};

class MyCamera {
protected:
    glm::vec3 cameraPos;
    glm::vec3 cameraCenter;
    glm::vec3 worldUp;
    glm::mat4 projectionMatrix;
    glm::mat4 viewMatrix;
    Mode mode;
public:  
    void setMode(Mode mode) {
        this->mode = mode;
    }
    Mode getMode() {
        return mode;
    }
    void setCameraPos(glm::vec3 c_Pos) {
        this->cameraPos = c_Pos;
    }

    void setCameraCenter(glm::vec3 c_Center) {
        this->cameraCenter = c_Center;
    }

    void setWorldUp(glm::vec3 w_Up) {
        this->worldUp = w_Up;
    }

    void setView() {
        this->viewMatrix = glm::lookAt(this->cameraPos, this->cameraCenter, this->worldUp);
    }

    glm::vec3 getCameraPos() {
        return this->cameraPos;
    }

    glm::vec3 getCameraCenter() {
        return this->cameraCenter;
    }

    glm::mat4 getProjectionMatrix() {
        return this->projectionMatrix;
    }

    glm::mat4 getViewMatrix() {
        return this->viewMatrix;
    }
    virtual void kbCallBack(GLFWwindow* window, int key, int scancode, int action, int mods) = 0;
};

class OrthoCamera : public MyCamera {
public:
    void setProjection(float left, float right, float bottom, float top, float zNear, float zFar) {
        //cant we make this a mat4 in player then set it to the base camera class?
        this->projectionMatrix = glm::ortho(left, right, bottom, top, zNear, zFar);
    }
    void kbCallBack(GLFWwindow* window, int key, int scancode, int action, int mods) {

    }
};

class PerspectiveCamera : public MyCamera {
public:
    
    void setProjection(float fov, float width, float height) {
        this->projectionMatrix = glm::perspective(glm::radians(fov), width / height, 0.1f, 100.0f);
    }
    void kbCallBack(GLFWwindow* window, int key, int scancode, int action, int mods) {

        if (key == GLFW_KEY_1 && action == GLFW_PRESS) {
            switch (mode)
            {
            case Mode::TPS:

                mode = Mode::FPS;
                //manipulate center and position here
                break;
            case Mode::FPS:

                mode = Mode::TPS;
                //manipulate center and position here
                break;
            }

        }
        
        // make 0.1 a constant
        if (GLFW_KEY_W == key ) {

                this->cameraPos.z -= 0.1f;
                this->cameraCenter.z -= 0.1f;

            
            //this->cameraPos.x += 0.1f;
        }
            
        else if (key == GLFW_KEY_S )
        {
            this->cameraPos.z += 0.1f;
            this->cameraCenter.z += 0.1f;
            //this->cameraPos.x += 0.1f;
        }

        if (key == GLFW_KEY_Q) {
            if (this->cameraPos.y <= 0) {
                this->cameraPos.y += 0.1f;
                this->cameraCenter.y += 0.1f;
            }
        }
        else if (key == GLFW_KEY_E) {
            this->cameraPos.y -= 0.1f;
            this->cameraCenter.y -= 0.1f;
        }

    }
};

class ModelClass {
protected:
    std::string objPath;
    std::vector<GLfloat> vertexData;
    std::vector<GLuint> textures;
    bool withNormals = false;
    GLuint VAO, VBO;

public:
    ModelClass(std::string path) : objPath(path) {}

    void loadObj() {
        // Loading .obj file
        std::vector<tinyobj::shape_t> shapes;
        std::vector<tinyobj::material_t> materials;
        std::string warning, error;
        tinyobj::attrib_t attributes;

        bool success = tinyobj::LoadObj(
            &attributes,
            &shapes,
            &materials,
            &warning,
            &error,
            this->objPath.c_str()
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
        for (int i = 0; i < shapes[0].mesh.indices.size(); i++) {
            tinyobj::index_t vData = shapes[0].mesh.indices[i];
            int vertexIndex = vData.vertex_index * 3;
            int normalIndex = vData.normal_index * 3;
            int uvIndex = vData.texcoord_index * 2;

            // ---------------------------------------------------
            // POSITION
            this->vertexData.push_back(
                attributes.vertices[vertexIndex]
            );

            this->vertexData.push_back(
                attributes.vertices[vertexIndex + 1]
            );

            this->vertexData.push_back(
                attributes.vertices[vertexIndex + 2]
            );

            // ---------------------------------------------------
            // NORMALS
            this->vertexData.push_back(
                attributes.normals[normalIndex]
            );

            this->vertexData.push_back(
                attributes.normals[normalIndex + 1]
            );

            this->vertexData.push_back(
                attributes.normals[normalIndex + 2]
            );

            // ---------------------------------------------------
            // TEXTURE COORDINATES
            this->vertexData.push_back(
                attributes.texcoords[uvIndex]
            );

            this->vertexData.push_back(
                attributes.texcoords[uvIndex + 1]
            );

            // ---------------------------------------------------
            // TANGENTS
            this->vertexData.push_back(
                tangents[i].x
            );
            this->vertexData.push_back(
                tangents[i].y
            );
            this->vertexData.push_back(
                tangents[i].z
            );

            // ---------------------------------------------------
            // BITANGENTS
            this->vertexData.push_back(
                bitangents[i].x
            );
            this->vertexData.push_back(
                bitangents[i].y
            );
            this->vertexData.push_back(
                bitangents[i].z
            );
        }
    }

    void attachTexture(std::string texPath, GLint format) {
        // Flip image vertically on load
        stbi_set_flip_vertically_on_load(true);

        // Initialize variables for loading the texture
        int img_width, img_height, colorChannels;

        unsigned char* tex_bytes = stbi_load(
            texPath.c_str(),
            &img_width, &img_height, &colorChannels,
            0
        );

        // Initialize texture variable
        GLuint tex;
        glGenTextures(1, &tex);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, tex);

        // Attach loaded image to texture variable
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            format,
            img_width,
            img_height,
            0,
            format,
            GL_UNSIGNED_BYTE,
            tex_bytes
        );
        // Generate Mipmap
        glGenerateMipmap(GL_TEXTURE_2D);

        // Free up loaded bytes
        stbi_image_free(tex_bytes);

        this->textures.push_back(tex);
    }

    void attachNormalTexture(std::string texPath, GLint format) {
        this->withNormals = true;
        attachTexture(texPath, format);
    }

    void createVAO_VBO() {
        GLintptr normalPtr = 3 * sizeof(GL_FLOAT);
        GLintptr uvPtr = 6 * sizeof(GL_FLOAT);
        GLintptr tangentPtr = 8 * sizeof(GL_FLOAT);
        GLintptr bitangentPtr = 11 * sizeof(GL_FLOAT);

        glGenVertexArrays(1, &this->VAO);
        glGenBuffers(1, &this->VBO);

        glBindVertexArray(this->VAO);

        glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
        glBufferData(
            GL_ARRAY_BUFFER,
            sizeof(GL_FLOAT) * this->vertexData.size(),
            this->vertexData.data(),
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
    }

    GLuint getVAO() {
        return this->VAO;
    }

    GLuint getBaseTexture() {
        return this->textures[0];
    }

    GLuint getNormals() {
        return this->textures[1];
    }

    std::vector<GLfloat> getVertexData() {
        return this->vertexData;
    }
};

class EnemyClass : public ModelClass {
public:
    glm::vec3 enemyPos;
    glm::vec3 enemyRot;
    float enemyScale;

    EnemyClass(std::string path,
        glm::vec3 pos,
        glm::vec3 rot,
        float scale) :
            ModelClass(path),
                enemyPos(pos),
                enemyRot(rot),
                enemyScale(scale) {}

    void draw(GLuint shaderProgram) {
        glUseProgram(shaderProgram);
        glBindVertexArray(this->VAO);

        // Initialize transformation matrix, and assign position, scaling, and rotation
        glm::mat4 transformationMatrix = glm::translate(glm::mat4(1.0f), this->enemyPos);
        transformationMatrix = glm::scale(transformationMatrix, glm::vec3(this->enemyScale));

        // X axis rotation
        transformationMatrix = glm::rotate(transformationMatrix,
                                    glm::radians(this->enemyRot.x),
                                    glm::normalize(glm::vec3(1.0f, 0.0f, 0.0f)));
        // Y axis rotation
        transformationMatrix = glm::rotate(transformationMatrix,
                                    glm::radians(this->enemyRot.y),
                                    glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f)));
        // Z axis rotation
        transformationMatrix = glm::rotate(transformationMatrix,
                                    glm::radians(this->enemyRot.z),
                                    glm::normalize(glm::vec3(0.0f, 0.0f, 1.0f)));

        // Initialize transformation location, and assign transformation
        unsigned int transformationLoc = glGetUniformLocation(shaderProgram, "transform");
        glUniformMatrix4fv(transformationLoc, 1, GL_FALSE, glm::value_ptr(transformationMatrix));

        glActiveTexture(GL_TEXTURE0);
        GLuint tex0Address = glGetUniformLocation(shaderProgram, "tex0");
        glBindTexture(GL_TEXTURE_2D, this->textures[0]);
        glUniform1i(tex0Address, 0);

        if (withNormals) {
            glActiveTexture(GL_TEXTURE1);
            GLuint tex1Address = glGetUniformLocation(shaderProgram, "norm_tex");
            glBindTexture(GL_TEXTURE_2D, this->textures[1]);
            glUniform1i(tex1Address, 1);
        }

        // Draw
        glDrawArrays(GL_TRIANGLES, 0, this->vertexData.size() / 5);
    }
};
/// <summary>
/// builder classes allow you to chain methods. similar to java builder classes
/// </summary>
class lightBuilder {
private:
    float specPhong;
    float specStr;
    float ambStr;
    float lumens;
    glm::vec3 ambRGB;
    glm::vec3 ray;
    glm::vec3 lightRGB;

public:
    lightBuilder() :
        specPhong(0), specStr(0), ambStr(0), lumens(0), ambRGB(glm::vec3(0)),
        ray(glm::vec3(0)), lightRGB(glm::vec3(0))
    {}

    lightBuilder(lightBuilder* lb) :
        specPhong(lb->specPhong), specStr(lb->specStr), ambStr(lb->ambStr), lumens(lb->lumens),
        ambRGB(lb->ambRGB), ray(lb->ray), lightRGB(lb->lightRGB)
    {}

    inline lightBuilder* setSpecStr(float str) {
        specStr = str;
        return this;
    }
    inline float getSpecStr() {
        return this->specStr;
    }

    inline lightBuilder* setSpecPhong(float phong) {
        specPhong = phong;
        return this;
    }
    inline float getSpecPhong() {
        return this->specPhong;
    }

    inline lightBuilder* setAmbStr(float str) {
        ambStr = str;
        return this;
    }
    inline float getAmbStr() {
        return ambStr;
    }

    inline lightBuilder* setLumens(float str) {
        lumens = str;
        return this;
    }
    inline float getLumens() {
        return lumens;
    }

    inline lightBuilder* setAmbColor(glm::vec3* color) {
        ambRGB = *color;
        return this;
    }
    inline glm::vec3 getAmbRGB() {
        return ambRGB;
    }
    inline lightBuilder* setLightColor(glm::vec3* color) {

        lightRGB = *color;
        return this;
    }
    inline glm::vec3 getLightRGB() {
        return lightRGB;
    }
    inline lightBuilder* setLightVec(glm::vec3* dir) {
        ray = *dir;
        return this;
    }
    inline glm::vec3 getLightVec() {
        return ray;
    }

    inline void placeLight(GLint unif) {
        glUniform3fv(unif, 1, glm::value_ptr(ray));
    }

    virtual void placeUnifs(GLint* uniforms) {
        glUniform1f(uniforms[0], specPhong);

        glUniform1f(uniforms[1], specStr);

        glUniform1f(uniforms[2], ambStr);

        glUniform1f(uniforms[3], lumens);

        glUniform3fv(uniforms[4], 1, glm::value_ptr(ambRGB));

        glUniform3fv(uniforms[5], 1, glm::value_ptr(lightRGB));

        placeLight(uniforms[6]);
    }
};

class PlayerClass : public ModelClass {
public:
    glm::vec3 playerPos;
    glm::vec3 playerRot;
    float playerScale;
    lightBuilder* bulb;
    glm::mat4 transformationMatrix;

    inline PlayerClass(std::string path,
        glm::vec3 pos,
        glm::vec3 rot,
        float scale) :
            ModelClass(path),
                playerPos(pos),
                playerRot(rot),
                playerScale(scale),bulb(new lightBuilder()), transformationMatrix(glm::mat4(1.0)) {
        glm::vec3 src = pos;
        src.z -= 0.7;
        bulb
            ->setLumens(1)
            ->setAmbStr(1)
            ->setSpecPhong(10)
            ->setSpecStr(1)
            ->setLightVec(&src)
            ->setLightColor(new glm::vec3(1))
            ->setAmbColor(new glm::vec3(1));
    
    }
    inline void placeUnifs(GLint* unifs) {
        bulb->placeUnifs(unifs);
    }
    inline void placeLight(GLint unif) {
        bulb->placeLight(unif);
    }
    void draw(GLuint shaderProgram) {
        glUseProgram(shaderProgram);
        glBindVertexArray(this->VAO);

        // Initialize transformation matrix, and assign position, scaling, and rotation
        transformationMatrix = glm::translate(glm::mat4(1.0f),
                                            this->playerPos);

        // Scale
        transformationMatrix = glm::scale(transformationMatrix, glm::vec3(this->playerScale));

        // X axis rotation
        transformationMatrix = glm::rotate(transformationMatrix,
                                    glm::radians(this->playerRot.x),
                                    glm::normalize(glm::vec3(1.0f, 0.0f, 0.0f)));
        // Y axis rotation
        transformationMatrix = glm::rotate(transformationMatrix,
                                    glm::radians(this->playerRot.y),
                                    glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f)));
        // Z axis rotation
        transformationMatrix = glm::rotate(transformationMatrix,
                                    glm::radians(this->playerRot.z),
                                    glm::normalize(glm::vec3(0.0f, 0.0f, 1.0f)));

        // Initialize transformation location, and assign transformation
        unsigned int transformationLoc = glGetUniformLocation(shaderProgram, "transform");
        glUniformMatrix4fv(transformationLoc, 1, GL_FALSE, glm::value_ptr(transformationMatrix));

        glActiveTexture(GL_TEXTURE0);
        GLuint tex0Address = glGetUniformLocation(shaderProgram, "tex0");
        glBindTexture(GL_TEXTURE_2D, this->textures[0]);
        glUniform1i(tex0Address, 0);

        if (withNormals) {
            glActiveTexture(GL_TEXTURE1);
            GLuint tex1Address = glGetUniformLocation(shaderProgram, "norm_tex");
            glBindTexture(GL_TEXTURE_2D, this->textures[1]);
            glUniform1i(tex1Address, 1);
        }

        // Draw
        glDrawArrays(GL_TRIANGLES, 0, this->vertexData.size() / 5);
    }

    float getDepth() {
        return this->playerPos.y;
    }
    void forward() {
        this->playerPos.z -= 0.1;
    }
    void back() {
        this->playerPos.z += 0.1;
    }
    void up() {
        playerPos.y += 0.1f;

    }
    void down() {
        playerPos.y -= 0.1f;
    }
void kbCallBack(GLFWwindow* window, int key, int scancode, int action, int mods) {
     

            if (key == GLFW_KEY_W)forward();
            else if (key == GLFW_KEY_S)back();
            if (key == GLFW_KEY_Q && playerPos.y + 0.1f <= 0)up();
            else if (key == GLFW_KEY_E) down();
            if (key == GLFW_KEY_A) this->transformationMatrix = glm::rotate(this->transformationMatrix,glm::radians(90.f),glm::vec3(1,1,0));

        
    }
};
/// handler class holds pointers for classes that need to use callbacks
class Handler {
public:
    PlayerClass* player;
    MyCamera* cam;
};
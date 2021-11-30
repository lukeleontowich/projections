#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "stb_image.h"

#include <string>
#include <iostream>

//  Need to define max number of textures
constexpr unsigned NUM_TEXTURES = 10;

class Texture {
private:
    //  Storage for textures
    unsigned int* textures = nullptr;

    //  Current number of textures initialized
    unsigned int n = 0;

public:
    Texture();
    ~Texture();

    /** addTexture
    ** @param file path to texture
    ** @return returns 0 if texture adding fails
    ** Adds a texture to the textures array
    **/
    int addTexture(std::string file_path);

    /** bind
    ** @param index that will bind (become the current texture in openGL)
    ** @return Returns 0 if error
    ** Binds a texture to openGL
    **/
    int bind(unsigned int index);

    /** destroy
    ** @param none
    ** @return void
    ** Need to deallocate the memory
    **/
    void destroy();
};

#endif // TEXTURE_H

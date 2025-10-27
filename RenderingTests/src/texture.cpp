#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "texture.h"
#include <SDL2/SDL.h>
#include <iostream>

Texture::Texture(const char* path)
{

	glGenTextures(1, &ID_);
	glBindTexture(GL_TEXTURE_2D, ID_);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
		GL_LINEAR_MIPMAP_LINEAR);
	//Dont use mipmap options for magnification
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


	int nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(path, & width_, &height_, &nrChannels, 0);
	if (!data)
	{
		std::cout << "ERROR::TEXTURE::TEXTURE_NOT_SUCCESFULLY_READ" << std::endl;
		return;
	}
	//checking how many color channels
	switch (nrChannels)
	{
	case 1:
		internalFormat_ = GL_RED;
		imageFormat_ = GL_RED;
		break;
	case 4:
		internalFormat_ = GL_RGBA;
		imageFormat_ = GL_RGBA;
		break;
	default:
		internalFormat_ = GL_RGB;
		imageFormat_ = GL_RGB;
		break;
	}

	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat_, width_, height_, 0, imageFormat_, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
	stbi_image_free(data);
}

Texture::~Texture()
{
	glDeleteTextures(1, &ID_);
}

void Texture::bind(unsigned int slot)
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, ID_);
}

void Texture::unbind()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}











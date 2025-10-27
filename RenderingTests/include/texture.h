#pragma once
#include <glad/glad.h>

class Texture
{
public:
	unsigned int ID_;
	int width_;
	int height_;
	GLenum internalFormat_;
	GLenum imageFormat_;

	Texture(const char* path);
	~Texture();
	void bind(unsigned int slot);
	void unbind();

};
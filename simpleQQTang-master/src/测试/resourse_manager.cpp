/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#include "resource_manager.h"

#include <iostream>
#include <sstream>
#include <fstream>

#include <SOIL/SOIL.h>

// Instantiate static variables
std::map<std::string, Texture2D>    ResourceManager::Textures;
std::map<std::string, Shader>       ResourceManager::Shaders;


Shader ResourceManager::LoadShader(const GLchar *vShaderFile, const GLchar *fShaderFile, const GLchar *gShaderFile, std::string name)
{
	Shaders[name] = loadShaderFromFile(vShaderFile, fShaderFile, gShaderFile);
	return Shaders[name];
}

Shader ResourceManager::GetShader(std::string name)
{
	return Shaders[name];
}

Texture2D ResourceManager::LoadTexture(const GLchar *file, GLboolean alpha, std::string name)
{
	Textures[name] = loadTextureFromFile(file, alpha);
	return Textures[name];
}

Texture2D ResourceManager::GetTexture(std::string name)
{
	return Textures[name];
}

void ResourceManager::Clear()
{
	// (Properly) delete all shaders	
	for (auto iter : Shaders)
		glDeleteProgram(iter.second.ID);
	// (Properly) delete all textures
	for (auto iter : Textures)
		glDeleteTextures(1, &iter.second.ID);
}

Shader ResourceManager::loadShaderFromFile(const GLchar *vShaderFile, const GLchar *fShaderFile, const GLchar *gShaderFile)
{
    std::string assetDir = "./"; // Use "./" or "assets/" if you move assets
    std::string vShaderPath = assetDir + std::string(vShaderFile);
    std::string fShaderPath = assetDir + std::string(fShaderFile);
    std::string gShaderPath = gShaderFile ? assetDir + std::string(gShaderFile) : "";
    std::string vertexCode;
    std::string fragmentCode;
    std::string geometryCode;
    try
    {
        std::ifstream vertexShaderFile(vShaderPath);
        std::ifstream fragmentShaderFile(fShaderPath);
        std::stringstream vShaderStream, fShaderStream;
        vShaderStream << vertexShaderFile.rdbuf();
        fShaderStream << fragmentShaderFile.rdbuf();
        vertexShaderFile.close();
        fragmentShaderFile.close();
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
        if (gShaderFile != nullptr)
        {
            std::ifstream geometryShaderFile(gShaderPath);
            std::stringstream gShaderStream;
            gShaderStream << geometryShaderFile.rdbuf();
            geometryShaderFile.close();
            geometryCode = gShaderStream.str();
        }
        std::cout << "[DEBUG] Loaded shader files: " << vShaderPath << ", " << fShaderPath;
        if (gShaderFile) std::cout << ", " << gShaderPath;
        std::cout << std::endl;
        if (vertexCode.empty()) std::cout << "[ERROR] Vertex shader file empty: " << vShaderPath << std::endl;
        if (fragmentCode.empty()) std::cout << "[ERROR] Fragment shader file empty: " << fShaderPath << std::endl;
        if (gShaderFile && geometryCode.empty()) std::cout << "[ERROR] Geometry shader file empty: " << gShaderPath << std::endl;
    }
    catch (std::exception e)
    {
        std::cout << "ERROR::SHADER: Failed to read shader files: " << vShaderPath << ", " << fShaderPath;
        if (gShaderFile) std::cout << ", " << gShaderPath;
        std::cout << std::endl;
    }
    const GLchar *vShaderCode = vertexCode.c_str();
    const GLchar *fShaderCode = fragmentCode.c_str();
    const GLchar *gShaderCode = geometryCode.c_str();
    Shader shader;
    shader.Compile(vShaderCode, fShaderCode, gShaderFile != nullptr ? gShaderCode : nullptr);
    return shader;
}

Texture2D ResourceManager::loadTextureFromFile(const GLchar *file, GLboolean alpha)
{
    std::string assetDir = "./"; // Use "./" or "assets/" if you move assets
    std::string filePath = assetDir + std::string(file);
    Texture2D texture;
    if (alpha)
    {
        texture.Internal_Format = GL_RGBA;
        texture.Image_Format = GL_RGBA;
    }
    int width = 0, height = 0;
    unsigned char* image = SOIL_load_image(filePath.c_str(), &width, &height, 0, texture.Image_Format == GL_RGBA ? SOIL_LOAD_RGBA : SOIL_LOAD_RGB);
    if (!image || width == 0 || height == 0) {
        std::cout << "[ERROR] Failed to load texture: " << filePath << " (" << file << ")" << std::endl;
    } else {
        std::cout << "[DEBUG] Loaded texture: " << filePath << " (" << width << "x" << height << ")" << std::endl;
    }
    texture.Generate(width, height, image);
    SOIL_free_image_data(image);
    return texture;
}

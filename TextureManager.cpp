#include "TextureManager.h"

unordered_map<string, sf::Texture> TextureManager::textures;
string TextureManager::imagesDirectory;

void TextureManager::LoadTexture(string textureName)
{
	string path = "images/";
	path += textureName + ".png";

	textures[textureName].loadFromFile(path);
}

sf::Texture& TextureManager::GetTexture(string textureName)
{
	if (textures.find(textureName) == textures.end())
	{
		LoadTexture(textureName);
	}
	
	return textures[textureName];
}

void TextureManager::SetImagesDirectory(string directoryPath)
{
	imagesDirectory = directoryPath;
}

void TextureManager::Clear()
{
	textures.clear();
}



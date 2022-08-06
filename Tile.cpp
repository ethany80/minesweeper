#include "Tile.h"

Tile::Tile(bool isBomb, float x, float y) : isBomb(isBomb), adjacentBombs(0)
{
	layer1.setTexture(TextureManager::GetTexture("tile_revealed"));
	layer1.setPosition(sf::Vector2f(x, y));
	
	if (isBomb)
	{
		layer2.setTexture(TextureManager::GetTexture("mine"));
	}
	layer2.setPosition(sf::Vector2f(x, y));
	layer3.setTexture(TextureManager::GetTexture("tile_hidden"));
	layer3.setPosition(sf::Vector2f(x, y));
	layer4.setTexture(TextureManager::GetTexture("flag"));
	layer4.setPosition(sf::Vector2f(x, y));
	layer4.setColor(sf::Color(255, 255, 255, 0));
	layer5.setTexture(TextureManager::GetTexture("mine"));
	layer5.setPosition(sf::Vector2f(x, y));
	layer5.setColor(sf::Color(255, 255, 255, 0));

	isFlag = false;
	isReveal = false;
	isDebug = false;
}

void Tile::draw(sf::RenderWindow& window)
{
	window.draw(layer1);
	if (isBomb || adjacentBombs > 0)
	{
		window.draw(layer2);
	}
	window.draw(layer3);
	window.draw(layer4);
	window.draw(layer5);
}

void Tile::flag()
{
	if (!isReveal)
	{
		if (isFlag)
		{
			layer4.setColor(sf::Color(255, 255, 255, 0));
		}
		else
		{
			layer4.setColor(sf::Color(255, 255, 255, 255));
		}
	}
	isFlag = !isFlag;
}

void Tile::reveal()
{
	isReveal = true;
	layer3.setColor(sf::Color(255, 255, 255, 0));
}

void Tile::setAdjacentTexture(int num)
{	
	adjacentBombs = num;
	if (num > 0)
	{
		string num_file = "number_" + to_string(num);
		layer2.setTexture(TextureManager::GetTexture(num_file));
	}
}

void Tile::debug(sf::RenderWindow& window)
{
	if (!isDebug)
	{
		isDebug = true;
		layer5.setColor(sf::Color(255, 255, 255, 255));
	}
	else
	{
		isDebug = false;
		layer5.setColor(sf::Color(255, 255, 255, 0));
	}
}

void Tile::loseReveal()
{
	layer3.setColor(sf::Color(255, 255, 255, 0));
	layer5.setColor(sf::Color(255, 255, 255, 255));
}

bool Tile::getIsBomb() { return isBomb; }

bool Tile::getIsReveal() { return isReveal; }

bool Tile::getIsFlag() { return isFlag; }

int Tile::getAdjacentBombs() { return adjacentBombs; }


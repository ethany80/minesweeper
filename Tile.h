#include <SFML/Graphics.hpp>
#include "TextureManager.h"
using namespace std;

class Tile
{
	sf::Sprite layer1;
	sf::Sprite layer2;
	sf::Sprite layer3;
	sf::Sprite layer4;
	sf::Sprite layer5;

	bool isBomb;
	bool isFlag;
	bool isReveal;
	bool isDebug;
	int adjacentBombs;
public:
	Tile(bool isBomb, float x, float y);
	vector<Tile*> adjacent;
	void draw(sf::RenderWindow& window);
	void setAdjacentTexture(int num);
	void reveal();
	void flag();
	void debug(sf::RenderWindow& window);
	void loseReveal();
	bool getIsBomb();
	bool getIsReveal();
	bool getIsFlag();
	int getAdjacentBombs();
};


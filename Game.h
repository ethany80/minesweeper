#include <vector>
#include <fstream>
#include <iostream>
#include <cstdlib>
#include "Tile.h"
#include "Random.h"
using namespace std;

class Game
{
	vector<vector<Tile>> tiles;
	sf::Sprite restart, bDebug, test_1, test_2, test_3;
	vector<sf::Sprite> digits;
	vector<sf::Texture> digitTexts;
	int columns, rows, mines, mineCount, revealCount;
	bool gameOver, isDebug;
public:
	Game(sf::RenderWindow& window);
	void newGame(sf::RenderWindow& window, int gameType);
	void update(sf::RenderWindow& window);
	void draw(sf::RenderWindow& window);
	void updateMineCount(sf::RenderWindow& window);
	void recursiveReveal(Tile& tile);
	void debug(sf::RenderWindow& window);
	void endGame(sf::RenderWindow& window, bool endState);
};


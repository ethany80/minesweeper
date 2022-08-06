#include "Game.h"

Game::Game(sf::RenderWindow& window)
{
	//create intial board
	ifstream inFile("boards/config.cfg");
	inFile >> columns >> rows >> mines;
	window.create(sf::VideoMode((columns * 32), (rows * 32) + 100), "Minesweeper");
	
	//this->mineCount = mineCount;
	//load textures for buttons and digits for mine count,
	//button and digit textures dont change from board to board
	restart.setTexture(TextureManager::GetTexture("face_happy"));
	bDebug.setTexture(TextureManager::GetTexture("debug"));
	test_1.setTexture(TextureManager::GetTexture("test_1"));
	test_2.setTexture(TextureManager::GetTexture("test_2"));
	test_3.setTexture(TextureManager::GetTexture("test_3"));

	for (int i = 0; i < 4; ++i)
	{
		sf::Sprite digit;
		digits.push_back(digit);
	}

	for (int i = 0; i < 11; ++i)
	{
		sf::Texture text;
		text.loadFromFile("images/digits.png", sf::IntRect(i * 21, 0, 21, 32));
		digitTexts.push_back(text);
	}

	isDebug = false;

	newGame(window, 0);
}

void Game::newGame(sf::RenderWindow& window, int gameType)
{
	//reset game variables
	tiles.clear();
	gameOver = false;
	restart.setTexture(TextureManager::GetTexture("face_happy"));

	//2d vector same size as tiles vector made
	//to designate bombs to tiles
	vector<vector<bool>> bombs;

	//new game is either random or based
	//on test cases.

	//0 creates new random game
	if (gameType == 0)
	{
		//reset config board
		ifstream inFile("boards/config.cfg");
		inFile >> columns >> rows >> mines;

		//intialize 2d bombs vector
		for (int i = 0; i < rows; ++i)
		{
			bombs.push_back(vector<bool>());
			for (int j = 0; j < columns; ++j)
			{
				bombs[i].push_back(false);
			}
		}

		//set bobms in random order using
		//user-created random class
		for (int i = 0; i < mines; ++i)
		{
			int rowIndex = Random::Int(0, rows - 1);
			int columnIndex = Random::Int(0, columns - 1);
			if (bombs[rowIndex][columnIndex])
			{
				--i;
				continue;
			}
			bombs[rowIndex][columnIndex] = true;
		}
	}
	//load test cases
	else
	{
		//load testboard based on num in gameType
		//and read lines into string vector
		string fileName = "boards/testboard" + to_string(gameType) + ".brd";
		ifstream inFile(fileName);
		string iter;
		vector<string> fileLines;

		while (!inFile.eof())
		{
			inFile >> iter;
			fileLines.push_back(iter);
		}

		//set dimensions and mine count,
		//and fill bombs vector
		rows = fileLines.size();
		columns = fileLines[0].size();
		mines = 0;
		
		for (int i = 0; i < rows; ++i)
		{
			bombs.push_back(vector<bool>());
			for (int j = 0; j < columns; ++j)
			{
				if (fileLines[i].at(j) == '0')
				{
					bombs[i].push_back(false);
				}
				else if (fileLines[i].at(j) == '1')
				{
					bombs[i].push_back(true);
					++mines;
				}
			}
		}
	}

	//resize window if dimensions change
	//between boards
	window.setSize(sf::Vector2u((columns * 32), (rows * 32) + 100));

	//count of revealed tiles to know when game
	//is won, and mineCount to display count of mines
	//(changes through flags)
	revealCount = columns * rows - mines;
	mineCount = mines;

	//create tiles vector using bombs vector
	for (int i = 0; i < rows; ++i)
	{
		tiles.push_back(vector<Tile>());
		for (int j = 0; j < columns; ++j)
		{
			Tile newTile(bombs[i][j], float(j) * 32.0, float(i) * 32.0);
			tiles[i].push_back(newTile);
		}
	}

	//set adjacent tiles
	/*
	adjacent bombs format: 7 0 1
						   6 T 2
					       5 4 3
	*/
	for (int i = 0; i < rows; ++i)
	{
		for (int j = 0; j < columns; ++j)
		{
			for (int u = 0; u < 8; ++u)
			{
				tiles[i][j].adjacent.push_back(nullptr);
			}

			if (i > 0)
			{
				tiles[i][j].adjacent[0] = &tiles[i - 1][j];
				if (j < columns - 1)
				{
					tiles[i][j].adjacent[1] = &tiles[i - 1][j + 1];
				}
			}
			if (j < columns - 1)
			{
				tiles[i][j].adjacent[2] = &tiles[i][j + 1];
				if (i < rows - 1)
				{
					tiles[i][j].adjacent[3] = &tiles[i + 1][j + 1];
				}
			}
			if (i < rows - 1)
			{
				tiles[i][j].adjacent[4] = &tiles[i + 1][j];
				if (j > 0)
				{
					tiles[i][j].adjacent[5] = &tiles[i + 1][j - 1];
				}
			}
			if (j > 0)
			{
				tiles[i][j].adjacent[6] = &tiles[i][j - 1];
				if (i > 0)
				{
					tiles[i][j].adjacent[7] = &tiles[i - 1][j - 1];
				}
			}
		}
	}

	//set adjacent tile count texture for
	//each tile
	for (int i = 0; i < rows; ++i)
	{
		for (int j = 0; j < columns; ++j)
		{
			if (!tiles[i][j].getIsBomb())
			{
				int num = 0;
				for (int u = 0; u < 8; ++u)
				{
					if (tiles[i][j].adjacent[u] != nullptr && tiles[i][j].adjacent[u]->getIsBomb())
					{
						++num;
					}
				}

				tiles[i][j].setAdjacentTexture(num);
			}
		}
	}

	//set button positions based on dimensions of board
	float halfWidth = float(columns) * 32.0 / 2.0;
	float yPos = float(rows) * 32.0;

	restart.setPosition(sf::Vector2f(halfWidth - 32.0, yPos));
	bDebug.setPosition(sf::Vector2f(halfWidth + (3 * 32.0), yPos));
	test_1.setPosition(sf::Vector2f(halfWidth + (5 * 32.0), yPos));
	test_2.setPosition(sf::Vector2f(halfWidth + (7 * 32.0), yPos));
	test_3.setPosition(sf::Vector2f(halfWidth + (9 * 32.0), yPos));

	//set digit positions (and negative
	//texture)
	digits[0].setPosition(sf::Vector2f(0.0, yPos));
	digits[0].setTexture(digitTexts[10]);
	digits[1].setPosition(sf::Vector2f(21.0, yPos));
	digits[2].setPosition(sf::Vector2f(42.0, yPos));
	digits[3].setPosition(sf::Vector2f(63.0, yPos));

	//debug stays on between boards to
	//allow ease of checking bomb placement
	//between random boards
	if (isDebug)
	{
		debug(window);
	}

	//draw tiles, buttons, and mine count to board
	window.clear(sf::Color(189, 189, 189, 255));
	draw(window);
	window.display();
}

//function draws sprites to window
void Game::draw(sf::RenderWindow& window)
{
	//nested for-loop draws each tile to
	//the window
	for (int i = 0; i < rows; ++i)
	{
		for (int j = 0; j < columns; ++j)
		{
			tiles[i][j].draw(window);
		}
	}

	//draw smile-face restart button, debug
	//button, and test buttons. call
	//updateMineCount to draw current mine
	//count
	window.draw(restart);
	window.draw(bDebug);
	window.draw(test_1);
	window.draw(test_2);
	window.draw(test_3);
	updateMineCount(window);
}

//update function called whenever mouse is
//clicked
void Game::update(sf::RenderWindow& window)
{	
	//2d vector creater for mouse position
	sf::Vector2i mousePos = sf::Mouse::getPosition(window);

	int x = mousePos.x / 32;
	int y = mousePos.y / 32;

	//clicks are within dimensions of window
	if (mousePos.x > 0 && mousePos.x < columns * 32 && mousePos.y > 0 && mousePos.y < rows * 32 + 100)
	{
		//if-else branch made for each left
		//click interaction
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			//tiles
			if (mousePos.y < rows * 32 && !gameOver)
			{
				//lose condition
				if (tiles[y][x].getIsBomb() && !tiles[y][x].getIsFlag())
				{
					endGame(window, false);
				}
				if (!tiles[y][x].getIsReveal())
				{
					recursiveReveal(tiles[y][x]);
				}
				//win condition
				if (revealCount == 0)
				{
					endGame(window, true);
				}
			}
			//buttons
			else if (mousePos.y > rows * 32 && mousePos.y < (rows * 32) + 64)
			{
				//smiley
				if (mousePos.x > columns * 32 / 2 - 32 && mousePos.x < columns * 32 / 2 + 32)
				{
					newGame(window, 0);
				}
				else if (mousePos.x > columns * 32 / 2 + 96 && mousePos.x < columns * 32 / 2 + 352)
				{
					//debug
					if (mousePos.x < columns * 32 / 2 + 160)
					{
						if (!gameOver)
						{
							debug(window);
							isDebug = !isDebug;
						}
					}
					//test1
					else if (mousePos.x < columns * 32 / 2 + 224)
					{
						newGame(window, 1);
					}
					//test2
					else if (mousePos.x < columns * 32 / 2 + 288)
					{
						newGame(window, 2);
					}
					//test3
					else if (mousePos.x < columns * 32 / 2 + 352)
					{
						newGame(window, 3);
					}
				}
			}
		}
		//flags
		if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
		{
			if (mousePos.y < rows * 32 && !gameOver)
			{
				if (!tiles[y][x].getIsReveal())
				{
					tiles[y][x].flag();
					if (tiles[y][x].getIsFlag())
					{
						--mineCount;
					}
					else
					{
						++mineCount;
					}
				}
			}
		}
	}
	draw(window);
}

//function updates displayed mine count
void Game::updateMineCount(sf::RenderWindow& window)
{
	if (mineCount < 0)
	{
		window.draw(digits[0]);
	}
	digits[1].setTexture(digitTexts[abs(mineCount) / 100]);
	window.draw(digits[1]);
	digits[2].setTexture(digitTexts[abs(mineCount) % 100 / 10]);
	window.draw(digits[2]);
	digits[3].setTexture(digitTexts[abs(mineCount) % 10]);
	window.draw(digits[3]);
}

//recursive reveal function reveals tile
//if 0 surrounding bombs present, then
//recursion re-enters function with 4 tiles
//in each non-diagonal direction
void Game::recursiveReveal(Tile& tile)
{
	if (!tile.getIsReveal() && !tile.getIsFlag())
	{
		tile.reveal();
		--revealCount;
		if (tile.getAdjacentBombs() == 0 && !tile.getIsBomb())
		{
			for (int i = 0; i < 8; ++i)
			{
				if (tile.adjacent[i] && !tile.adjacent[i]->getIsBomb())
				{
					recursiveReveal(*tile.adjacent[i]);
				}
			}
		}
	}
}

//function called to add bomb layer
//to top of ever tile with bomb
void Game::debug(sf::RenderWindow& window)
{
	for (int i = 0; i < rows; ++i)
	{
		for (int j = 0; j < columns; ++j)
		{
			if (tiles[i][j].getIsBomb())
			{
				tiles[i][j].debug(window);
			}
		}
	}
}

//actions to be completed if game
//is won (endState = true) or lost
//(endState = false)
void Game::endGame(sf::RenderWindow& window, bool endState)
{
	gameOver = true;
	if (isDebug)
	{
		debug(window);
		isDebug = false;
	}
	if (endState)
	{
		for (int i = 0; i < rows; ++i)
		{
			for (int j = 0; j < columns; ++j)
			{
				if (tiles[i][j].getIsBomb() && !tiles[i][j].getIsFlag())
				{
					tiles[i][j].flag();
					--mineCount;
				}
			}
		}
		restart.setTexture(TextureManager::GetTexture("face_win"));
	}
	else
	{
		for (int i = 0; i < rows; ++i)
		{
			for (int j = 0; j < columns; ++j)
			{
				if (tiles[i][j].getIsBomb())
				{
					if (tiles[i][j].getIsFlag())
					{
						tiles[i][j].loseReveal();
					}
					else
					{
						tiles[i][j].reveal();
					}
				}
			}
		}
		restart.setTexture(TextureManager::GetTexture("face_lose"));
	}
}
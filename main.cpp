#include <iostream>
#include "Game.h"
using namespace std;

int main()
{
    sf::RenderWindow window;
    Game game(window);

    while (window.isOpen())
    {
        sf::Event event;

        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }

            window.clear(sf::Color(189, 189, 189, 255));
            
            game.update(window);

            window.display();
        }

    }

    return 0;
}
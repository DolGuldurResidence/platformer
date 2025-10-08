#pragma once 
#include <SFML/Graphics.hpp>
#include <player.hpp>
#include <obstruction.hpp>
#include <string>
#include <animation.hpp>

const unsigned SCREEN_WIDTH = 800;
const unsigned SCREEN_HEIGHT = 600;
 
class Game {
private:
    sf::Clock clock;
    sf::RenderWindow window;
    std::string title;
    Player player;
    Obstruction floor;
    
public:

        Game(unsigned int width, unsigned int height, const std::string& title) :
        window(sf::VideoMode({SCREEN_WIDTH, SCREEN_HEIGHT}), title), player(0, SCREEN_HEIGHT - 100),
        floor(0, SCREEN_HEIGHT - 50, SCREEN_WIDTH, 50)
        {

            this->title = title;
            window.create(sf::VideoMode({width, height}), title);
            window.setFramerateLimit(60);
            player = Player(0, SCREEN_HEIGHT - 100);


        }

        void lifeCycle(){
            while (window.isOpen()) {
                while (const std::optional event = window.pollEvent()) {
                    if (event->is<sf::Event::Closed>()) {
                        window.close();
                    }
                }

                float dt = clock.restart().asSeconds();

                Obstruction jumpPad(300, SCREEN_HEIGHT - 150, 100, 20);

                // Handle player input and movement
                player.handleInput(dt);
                player.update(dt);
                
                // Apply collisions
                player.applyObstructionCollision(jumpPad.getPositionX(), jumpPad.getPositionY(), 100, 20);
                player.applyFloorCollision(SCREEN_HEIGHT);
                player.applyWallCollision(0, SCREEN_WIDTH);
            
                window.clear();
                window.draw(player.getShape());
                window.draw(jumpPad.getShape());
                window.display();
            }

        }
};


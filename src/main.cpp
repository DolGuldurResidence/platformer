#include <SFML/Graphics.hpp>
#include <iostream>
#include <player.hpp>
#include <obstruction.hpp>
#include <game.hpp>

int main()
{
    const unsigned screenWidth = 800;
    const unsigned screenHeight = 600;
    
    auto window = sf::RenderWindow(sf::VideoMode({screenWidth, screenHeight}), "Yo");
    window.setFramerateLimit(60);

    // Create player in the center of screen
    Player player(0,screenHeight - 100);

    sf::Clock clock;

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
        }

        float dt = clock.restart().asSeconds();

        Obstruction floor(0, screenHeight - 50, screenWidth, 50);
        Obstruction jumpPad(300, screenHeight - 150, 100, 20);

        // Handle player input and movement
        player.handleInput();
        player.move(dt);
        
        // Apply collisions
        player.applyObstructionCollision(jumpPad.getPositionX(), jumpPad.getPositionY(), 100, 20);
        player.applyFloorCollision(screenHeight);
        player.applyWallCollision(0, screenWidth);

        window.clear();
        window.draw(player.getShape());
        window.draw(floor.getShape());
        window.draw(jumpPad.getShape());
        window.display();
    }
}
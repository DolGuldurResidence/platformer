#include <SFML/Graphics.hpp>
#include <iostream>
#include <player.hpp>
#include <obstruction.hpp>
#include <game.hpp>
#include <animation.hpp>


int main()
{
    Game game( SCREEN_WIDTH, SCREEN_HEIGHT, "My Game");
    game.lifeCycle();
}
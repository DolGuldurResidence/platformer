#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>


using namespace std;

const int screenWidth = 1050; //constant of screen Width
const int screenHeight = 1080;

class Player
{
private:
    int posX;
    int posY;
    int velX;
    int velY;
    float gravity;
    float maxSpeedX;
    float acceleration;
    float friction;
public:
    Player() = default;

    Player(int posX, int posY, int velX, int velY, float gravity, float maxSpeedX, float acceleration, float friction)
        : posX(posX), posY(posY), velX(velX), velY(velY), gravity(gravity), maxSpeedX(maxSpeedX), acceleration(acceleration), friction(friction)
        {}

    void Jump(){
        velY += gravity; //increase velocity

    }
};


int main()
{
    auto window = sf::RenderWindow(sf::VideoMode({screenWidth, screenHeight}), "Yo"); //init the window
    window.setFramerateLimit(60); //limit of FPS

    sf::CircleShape shape(50.f); //init shape circle with 50 pixels radius
    shape.setOrigin(sf::Vector2f(50.f, 50.f)); //set anchor of shape;

    float posX = screenHeight / 2, posY = screenWidth / 2; //init position in the center of screen;
    float velX = 0, velY = 30; //init Velocity 
    float gravity = 9.8f; // graviti
    float maxSpeedX = 700.f; // максимальная скорость по X
    float acceleration = 0.f; //init acceleration
    float friction = 0.9f; //init friction

    shape.setPosition(sf::Vector2f(posX, posY)); //set position of the shape before game loop

    sf::Clock clock; //init clock
    bool isJumping = false; //init is Jumping flag

    sf::Clock jumpClock; // //init Jumping time
    float jumpPower = -400.f; // pover of jump

    while (window.isOpen()) { //main game loop works while window open
        while (const std::optional event = window.pollEvent()) { //optional - container keep events, pollEvent() - take one event from queue of events 
            if (event->is<sf::Event::Closed>()){ //is<T>() - check if even T type, -> call method throw pointer, event is object
                window.close(); //close the window
            }
        }

        float dt = clock.restart().asSeconds(); //measure time since last frame in seconds

        velY += gravity; //increase velocity
        
        //jumping 
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space) && !isJumping) { //space pressed and Jumping false
            isJumping = true; //make flag jumping true
            jumpClock.restart(); // jumpST = time since game works
            velY = jumpPower; // set velocity of jump 200 pixels
        }

        // Завершение прыжка через  2  секунд или когда падаем вниз
        if (isJumping && (jumpClock.getElapsedTime().asSeconds()>= 1.f)) { 
            isJumping = false; // make flag false
            cout << "parasha" << clock.getElapsedTime().asSeconds() << endl;
            cout << "huynya " << jumpClock.getElapsedTime().asSeconds()   << endl;
        }



        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)){
            acceleration = 50.f;
            velX += acceleration;
        }// if right arrow pressed

        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)){
            acceleration = -50.f;
            velX += acceleration;
        } // if left arrow pressed
        else
             velX *= friction; // apply friction

    
        if (velX > maxSpeedX) velX = maxSpeedX; //speed limit right
        if (velX < -maxSpeedX) velX = -maxSpeedX; //speed limit left

        posY += velY * dt; //move by Y axis
        posX += velX * dt; //move by X axis
        
        // Пол (чтобы не провалиться)
        if (posY > screenHeight - 100) { 
            posY = screenHeight - 100;
            velY = 0;
            isJumping = false;
        }

        shape.setPosition(sf::Vector2f(posX, posY)); //set new position for for shape
         
        
        window.clear(); // clear the window
        window.draw(shape); //draw the shape
        window.display(); //Display on screen what has been rendered to the window so far.
    }
}
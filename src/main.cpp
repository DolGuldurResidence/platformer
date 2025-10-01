#include <SFML/Graphics.hpp>
#include <iostream>

using namespace std;

class Player {
private:
    sf::CircleShape shape;
    float posX, posY;
    float velX, velY;
    float gravity;
    float maxSpeedX;
    float acceleration;
    float friction;
    float jumpPower;
    
    bool isJumping;
    bool isSecJump;
    sf::Clock jumpClock;
    
    static constexpr float shapeRadius = 50.f;
    static constexpr float jumpDuration = 1.f;

public:
    Player(float startX, float startY) 
        : posX(startX), posY(startY), 
          velX(0), velY(0),
          gravity(9.8f),
          maxSpeedX(700.f),
          acceleration(0.f),
          friction(0.9f),
          jumpPower(-400.f),
          isJumping(false),
          isSecJump(false)
    {
        shape.setRadius(shapeRadius);
        shape.setOrigin(sf::Vector2f(shapeRadius, shapeRadius));
        shape.setPosition(sf::Vector2f(posX, posY));
    }
    
    void jump() {
        if (!isJumping) {
            isJumping = true;
            jumpClock.restart();
            velY = jumpPower;
        }
    }
    
    void handleInput() {
        // Handle jumping
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space) && !isJumping) {
            jump();
        }
        
        // Handle horizontal movement
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) {
            acceleration = 50.f;
            velX += acceleration;
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) {
            acceleration = -50.f;
            velX += acceleration;
        }
        else {
            velX *= friction; // apply friction when no key is pressed
        }
        
        // Speed limits
        if (velX > maxSpeedX) velX = maxSpeedX;
        if (velX < -maxSpeedX) velX = -maxSpeedX;
        
        // Handle jump completion
        if (isJumping && jumpClock.getElapsedTime().asSeconds() >= jumpDuration && !isSecJump) {
            isJumping = false;
            isSecJump = true;
            cout << "Jump completed at: " << jumpClock.getElapsedTime().asSeconds() << endl;
        }
    }
    
    void move(float dt) {
        // Apply gravity
        velY += gravity;
        
        // Update position based on velocity
        posY += velY * dt;
        posX += velX * dt;
        
        shape.setPosition(sf::Vector2f( posX, posY));
    }
    
    void applyFloorCollision(float floorHeight) {
        if (posY > floorHeight - 100) {
            posY = floorHeight - 100;
            velY = 0;
            isJumping = false;
            isSecJump = false;
        }
    }
    
    void applyWallCollision(float leftBound, float rightBound) {
        if (posX > rightBound - 100) {
            posX = rightBound - 100;
            velX = 0;
        }
        
        if (posX < leftBound + 100) {
            posX = leftBound + 100;
            velX = 0;
        }
    }
    
    sf::CircleShape& getShape() { return shape; }
    float getPositionX() const { return posX; }
    float getPositionY() const { return posY; }
    void setPosition(float x, float y) {
        posX = x;
        posY = y;
        shape.setPosition(sf::Vector2f(posX, posY));
    }
};

int main()
{
    const unsigned screenWidth = 800;
    const unsigned screenHeight = 600;
    
    auto window = sf::RenderWindow(sf::VideoMode({screenWidth, screenHeight}), "Yo");
    window.setFramerateLimit(60);

    // Create player in the center of screen
    Player player(screenWidth / 2, screenHeight / 2);

    sf::Clock clock;

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
        }

        float dt = clock.restart().asSeconds();

        // Handle player input and movement
        player.handleInput();
        player.move(dt);
        
        // Apply collisions
        player.applyFloorCollision(screenHeight);
        player.applyWallCollision(0, screenWidth);

        window.clear();
        window.draw(player.getShape());
        window.display();
    }
}
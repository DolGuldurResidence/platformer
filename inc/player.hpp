#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
    


class Player {
private:
    sf::CircleShape shape; // Player represented as a circle 
    float posX, posY;   
    float velX, velY;   
    float gravity;
    float maxSpeedX;
    float acceleration;
    float friction;
    float jumpPower;
    
    bool onObstruction;
    bool isJumping;
    bool isSecJump;
    sf::Clock jumpClock;
    
    float shapeRadius = 50.f;
    float jumpDuration = 500.f;

public:
    Player(float startX, float startY) 
        : posX(startX), posY(startY), //init position 
          velX(0), velY(0), // init velocity as zero (no movement at start)
          gravity(50.f),    // gravity force, using in jump and fall;
          maxSpeedX(600.f), // max horizontal speed <- ->
          acceleration(0.f), //init acceleration for smooth start/stop
          friction(0.9f),  // friction factor for air resistance
          jumpPower(-1000.f), // initial jump velocity
          isJumping(false), // is player in the air after jump
          isSecJump(false), // is second jump used, for double jump
          onObstruction(false) // is player on the ground or platform    
    {
        shape.setRadius(shapeRadius); // Player size
        shape.setOrigin(sf::Vector2f(shapeRadius, shapeRadius)); // Center origin for rotation and positioning
        shape.setPosition(sf::Vector2f(posX, posY)); // Initial position of the shpape
    }

    void jump() {
            isJumping = true; // mark as jumping
            jumpClock.restart(); // restart jump timer
            velY = jumpPower; // apply jump power    
    }
    
    void handleInput() {

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space) && !isJumping) {
            jump();
        }
        
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) {
            goRight();
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) {
            goLeft();
        }
        else {
            velX *= friction; // apply friction when no key is pressed
        }
        
        // Speed limits
        if (velX > maxSpeedX) velX = maxSpeedX;
        if (velX < -maxSpeedX) velX = -maxSpeedX;
        
        // Handle jump completion
        if (isJumping && jumpClock.getElapsedTime().asMilliseconds() >= jumpDuration && !isSecJump) {
            isJumping = false;
            isSecJump = true;
            std::cout << isSecJump << std::endl;    
        }
    }
    

    //TODO
    std::vector<int> boundigBox() const {
        return {
            static_cast<int>(posX - shapeRadius),
            static_cast<int>(posY - shapeRadius),
            static_cast<int>(shapeRadius * 2),
            static_cast<int>(shapeRadius * 2)
        };
    }
    
    void move(float dt) {
        // Apply gravity if not on ground or platform 
        if(!onObstruction) velY += gravity;
        
        // Update position based on velocity
        posY += velY * dt;
        posX += velX * dt;
        
        shape.setPosition(sf::Vector2f( posX, posY));
    }

    void goRight() {
        acceleration = 50.f;
        velX += acceleration;
    };
    
    void goLeft() {
        acceleration = -50.f;
        velX += acceleration;
    };


    //TODO
    void applyObstructionCollision(float obsX, float obsY, float obsW, float obsH) {
        if (posX >= obsX && posX <= obsX + obsW && posY + shapeRadius >= obsY){
            posY = obsY - shapeRadius;
            velY = 0;
            isJumping = false;
            isSecJump = false;
            onObstruction = true;
        }
        else {
            onObstruction = false;
        }   
    }

    void applyFloorCollision(float floorHeight) {
        if (posY > floorHeight - 100) {
            posY = floorHeight - 100;
            velY = 0;
            isJumping = false;
            isSecJump = false;
            onObstruction = true;
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
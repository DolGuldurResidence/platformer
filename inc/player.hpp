#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

const float SHAPE_RADIUS = 50.f;
const float GRAVITY = 1500.f;        
const float MAX_SPEED_X = 600.f;    
const float ACCELERATION = 2000.f;   
const float FRICTION = 0.8f;    
const float JUMP_POWER = -700.f;     
const float JUMP_DURATION = 0.5f;    

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
    bool isOnGround;
    bool canDoubleJump;
    float jumpTimer;

public:

    Player(const float& startX, const float& startY) {
        setPosition(startX, startY);
        setVelocity(0,0);
        setGravity(GRAVITY);
        setMaxSpeed(MAX_SPEED_X);
        setAcceleration(ACCELERATION);
        setFriction(FRICTION);
        setJumpPower(JUMP_POWER);
        setJumpTimer(0.f);

        shape.setRadius(SHAPE_RADIUS);
        shape.setOrigin(sf::Vector2f(SHAPE_RADIUS, SHAPE_RADIUS));
        shape.setPosition(sf::Vector2f(posX, posY));
    }

    //обработчик кнопок, dt~0.016 
    void handleInput(const float& dt) {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) {
            velX += acceleration * dt; // velx ~ 630;
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) {
            velX -= acceleration * dt;
        }
        else {
            velX *= friction;
        }
        
        if (velX > maxSpeedX) velX = maxSpeedX;
        if (velX < -maxSpeedX) velX = -maxSpeedX;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)) {
            if (isOnGround) {
                jump();
            }
            else if (canDoubleJump && jumpTimer <= 0.f) {
                jump();
                canDoubleJump = false;
            }
        }

        if (jumpTimer > 0) {
            jumpTimer -= dt;
        }
    }
        
    void update(const float& dt) {
        if (!isOnGround) {
            velY += gravity * dt;
        }

        posX += velX * dt;
        posY += velY * dt;
        
        shape.setPosition(sf::Vector2f( posX, posY));
    }

    void jump() {
        velY = jumpPower;
        isOnGround = false;
        jumpTimer = JUMP_DURATION;
    }

    void applyObstructionCollision(const float& obsX, const float& obsY, 
        const float& obsW,const float& obsH) {
            
        float playerLeft = posX - SHAPE_RADIUS;
        float playerRight = posX + SHAPE_RADIUS;
        float playerTop = posY - SHAPE_RADIUS;
        float playerBottom = posY + SHAPE_RADIUS;
        
        float obstacleLeft = obsX;
        float obstacleRight = obsX + obsW;
        float obstacleTop = obsY;
        float obstacleBottom = obsY + obsH;
        
        if (playerRight > obstacleLeft && 
            playerLeft < obstacleRight && 
            playerBottom > obstacleTop && 
            playerTop < obstacleBottom) {

            float overlapLeft = playerRight - obstacleLeft;
            float overlapRight = obstacleRight - playerLeft;
            float overlapTop = playerBottom - obstacleTop;
            float overlapBottom = obstacleBottom - playerTop;
            
            float minOverlap = std::min(std::min(overlapLeft, overlapRight),
            std::min( overlapTop, overlapBottom));
            
            if (minOverlap == overlapTop) {
                posY = obstacleTop - SHAPE_RADIUS; 
                velY = 0;
                isOnGround = true;
                canDoubleJump = true;
            }
            else if (minOverlap == overlapBottom) {
                posY = obstacleBottom + SHAPE_RADIUS;
                velY = 0;
            }
            else if (minOverlap == overlapLeft) {
                posX = obstacleLeft - SHAPE_RADIUS;
                velX = 0;
            }
            else if (minOverlap == overlapRight) {
                posX = obstacleRight + SHAPE_RADIUS;
                velX = 0;
            }
        }
        else isOnGround = false; 
    }




    void applyFloorCollision(float floorHeight) {
        if (posY + SHAPE_RADIUS > floorHeight) {
            posY = floorHeight - SHAPE_RADIUS;
            velY = 0;
            isOnGround = true;
            canDoubleJump = true;
        }
    }
    
    void applyWallCollision(float leftBound, float rightBound) {
        if (posX - SHAPE_RADIUS < leftBound) {
            posX = leftBound + SHAPE_RADIUS;
            velX = 0;
        }
        else if (posX + SHAPE_RADIUS > rightBound) {
            posX = rightBound - SHAPE_RADIUS;
            velX = 0;
        }
    }
    

    sf::CircleShape& getShape() { return shape; }
    float getPositionX() const { return posX; }
    float getPositionY() const { return posY; }
    float getVelocityX() const { return velX; }
    float getVelocityY() const { return velY; }
    bool getIsOnGround() const { return isOnGround; }



    void setPosition(float x, float y) {
        posX = x;
        posY = y;
        shape.setPosition(sf::Vector2f(posX, posY));
    }
    void setVelocity(float vx, float vy) {
        velX = vx;
        velY = vy;
    }

    void setGravity(float gravity){
        this->gravity = gravity;
    }

    void setAcceleration(float acceleration) {
        this->acceleration = acceleration;
    }

    void setMaxSpeed(float maxSpeed){
        this->maxSpeedX = maxSpeed;
    }

    void setFriction(float friction){
        this->friction = friction;
    }

    void setJumpPower(float jumpPower){
        this->jumpPower = jumpPower;
    }

    void setJumpTimer(float jumpTimer){
        this->jumpTimer = jumpTimer;
    } 
};
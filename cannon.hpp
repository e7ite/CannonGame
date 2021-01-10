#pragma once

#include "level.hpp"

struct Cannon
{
    // The position of the cannon
    float x, y;
    // The current ball that has been shot. If no ball has been shot, this will
    // be nullptr
    std::unique_ptr<CannonBall> projectile;
    // The wheel of the cannon. Currently just used to draw it to screen, but
    // does not move
    sf::CircleShape wheel;
    // The launcher where the cannon ball should spawn from. Rotates based
    // on player input
    sf::RectangleShape launcher;

    Cannon(float x = 0.0f, float y = 0.0f) 
        : x(x), y(y), wheel(sf::CircleShape()), launcher(sf::RectangleShape()) {}
    Cannon(float x, float y, float width, float height);
    ~Cannon();

    /**
     * Used to initalize the cannonball. Called when the user clicks and 
     * issues an initial velocity with velocity bar
     * 
     * @param velocity The velocity the ball was shot at 
     */
    void InitProjectile(float velocity);

    /**
     * Deletes a projectile that was previously active. Called when the ball
     * collides with the barriers
     */
    void DestroyProjectile();

    /**
     * Main method that renders the application
     * @param window The SFML window which everything is being rendered from
     */
    void Render(sf::RenderWindow &window);
};

struct CannonBall
{
    // Keeps track of the shape of the projectily and its position
    sf::CircleShape shape;
    // The current velocity of the ball when it was shot
    sf::Vector2f velocity;
    // The initial shot velocity of the ball. Needed for the kinematic equation
    float shotVelocity;
    // The initial shot position of the ball
    sf::Vector2f shotPosition;
    // How current time the ball has been in the air 
    sf::Clock airTime;
    // The initial angle when the ball was shot. Needed for the kinematic 
    // equation. This will be in degrees
    float shotAngle;

    CannonBall() = delete;
    CannonBall(const Cannon &cannon, float velocity);

    /**
     * Used to update the current position of the ball. This is where
     * the kinematic equation is being used to update the ball
     * 
     * @param level The current stage of the game. This method grabs the 
     *              boundaries from here
     * @param cannon The cannon where the ball was shot from
     * @return If the projectile collides, return true. else return false
     */
    bool UpdateTrajectory(Level &level);
};

/**
 * Montitors the keyboard and mouse for input, and updates the game
 * if the user requested to shoot the ball.
 * @param level The current level. Currently only level
 * @param cannon The 
 */
void HandleControls(Level &level, Cannon &cannon);
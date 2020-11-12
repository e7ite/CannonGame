#pragma once

#include <SFML/Graphics.hpp>
#include <cmath>
#include <cstring>
#include <sstream>
#include <iomanip>

struct CannonBall;

class Level
{
    // Used to determine if we should draw the stall the input for a few
    // milliseconds in order to slow down quick user input response from SFML
    int timer;
    // The current time of the level
    sf::Clock gameTime;
    // The background image of the level
    sf::Sprite background;
    // The positions where collisions should occur
    float xBoundaries[2], yBoundaries[2];

    // The possible states of the velocity bar
    enum VelocityBarState
    {
        SPAWNED   = 0,
        SELECTING = 1 << 1,
        DEAD      = 1 << 2
    };
    // Used to dictate if the velocity bar has just been spawned(0), selecting a
    // velocity (1 << 1), or is not spawned (1 << 2)
    char velBarState;
    // The HUD for the progress bar
    std::unique_ptr<sf::RectangleShape> velBorder, velProgress;
    // Indexes for each position statistic
    enum StatsIndex
    {
        ANGLE, VELOCITY, POSX, POSY, TIME, RESULTX, RESULTY
    };
    // The HUDs for each info statistic
    std::vector<std::unique_ptr<sf::Text>> statText;

public:
    Level() = delete;
    Level(const sf::Texture &texture, float x1 = 0.0f, float y1 = 0.0f,
          float x2 = 0.0f, float y2 = 0.0f);
    ~Level();

    /**
     * Used to delay the response from user input in order to prevent too quick
     * response to user input
     * 
     * @param ms Amount of milliseconds to wait for next response to user input
     */
    
    void Wait(int ms);
    /**
     * Returns if the time from last issued wait has elapsed and is ready to
     * receive the next user input
     */
    bool Ready();
    
    /**
     * Used to initialize the velocity bar and get the next velocity to fire
     * the next projectile
     */
    void InitVelocityBar();

    /**
     * Used to remove the velocity bar and all its resources
     */
    void DestroyVelocityBar();

    /**
     * Used to show all the statistics of the current projectile
     * 
     * @param font The font to show the statistics of the projectile
     */
    void InitPositionStats(const sf::Font &font);

    /**
     * Used to remove all the projectile stats
     */
    void DestroyPositionStats();

    /**
     * Used to update the projectile status based on the current time
     * 
     * @param projectile The current projectile in the air
     */
    void UpdatePositionStats(const CannonBall *projectile);

    /**
     * Used to detect if something has gone out-of-bounds within the level.
     * Return true if has gone out-of-bounds
     * 
     * @param x The x position of something to check if collision occured
     * @param y The y position of something to check if collision occured
     */
    bool Collision(float x, float y);

    /**
     * Used to change the velocity bar state whenever the mouse has been clicked
     * Increments the bar state by 1
     */
    void ChangeVelocityBarState();

    /**
     * Gets the velocity from the velocity bar and stores it whereever the
     * velocity pointer points to. If the user has finished choosing a velocity,
     * return true and store the result in velocity 
     * 
     * @param velocity Where to store the velocity chosen from the user
     * @return If the user has finished selecting a velocity and the result
     *         was stored in velocity, return true. Else return false
     */
    bool GetVelocity(float *velocity);

    /**
     * Used to draw all the HUDS for the level.
     * 
     * @param window The SFML window to draw all the HUDS in
     */
    void Render(sf::RenderWindow &window);
};

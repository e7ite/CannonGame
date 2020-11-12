#include "cannon.hpp" 

Cannon::Cannon(float x, float y, float width, float height)
    : x(x), y(y)
{
    this->wheel = sf::CircleShape(width / 2.0f);
    this->wheel.setPosition(this->x, this->y);
    this->wheel.setFillColor(sf::Color::Black);
    this->projectile = nullptr;
    this->launcher = sf::RectangleShape();
    this->launcher.setSize(sf::Vector2f(width, height));
    this->launcher.setPosition(this->x + 25.0f, this->y + 25.0f);
    this->launcher.setOrigin(0, width / 4.0f);
}

Cannon::~Cannon()
{
    this->DestroyProjectile();
}

void Cannon::InitProjectile(float velocity)
{
     this->projectile = std::make_unique<CannonBall>(*this, velocity);
}

void Cannon::DestroyProjectile()
{
    if (this->projectile) 
    {
        CannonBall *projectile = this->projectile.release();
        if (projectile)
            delete projectile;
    }
}

void Cannon::Render(sf::RenderWindow &window)
{
    if (this->projectile)
        window.draw(this->projectile->shape);
    window.draw(this->launcher);
    window.draw(this->wheel);
}

CannonBall::CannonBall(const Cannon &cannon, float velocity)
{
    this->shotVelocity = velocity;
    this->shape = sf::CircleShape(10.0f);
    this->airTime = sf::Clock();
    this->velocity = sf::Vector2f(velocity, velocity);
    this->shape.setOrigin(10.0f, 10.0f);
    this->shape.setFillColor(sf::Color::Black);
    this->shotAngle = cannon.launcher.getRotation() * M_PI / 180.0f;

    sf::Vector2f cannonPos = cannon.launcher.getPosition();
    float hyp = cannon.launcher.getSize().x;
    float adj = cos(this->shotAngle);
    float opp = sin(this->shotAngle);
    this->velocity.x *= adj;
    this->velocity.y *= opp;
    this->shape.setPosition(cannonPos.x + adj * hyp, cannonPos.y + opp * hyp);
    this->shotPosition = this->shape.getPosition();
}

bool CannonBall::UpdateTrajectory(Level &level, Cannon &cannon)
{
    //Position equation = X(t) = 0.5at^2 + Vit + Xi
    float seconds = this->airTime.getElapsedTime().asSeconds() * 4.0f;
    this->shape.setPosition(
        /*0.5f * 0 * pow(seconds, 2) + */
            this->velocity.x * seconds + this->shotPosition.x,
        0.5f * 9.8f * pow(seconds, 2) 
            + this->velocity.y * seconds + this->shotPosition.y
    );

    sf::Vector2f position = this->shape.getPosition();
    return level.Collision(position.x, position.y);
}

void HandleControls(Level &level, Cannon &cannon)
{
    if (level.Ready())
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        {
            cannon.launcher.rotate(-0.5f);
            level.Wait(8);
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        {
            cannon.launcher.rotate(0.5f);
            level.Wait(8);
        }

        if (!cannon.projectile && sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
            level.ChangeVelocityBarState();
            level.Wait(100);
        }
    }
}
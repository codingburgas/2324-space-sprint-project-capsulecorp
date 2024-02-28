#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <vector>

using namespace sf;

// Bullet class
class Bullet {
public:
    RectangleShape shape;
    Vector2f velocity;

    Bullet(float speed = 1.0f) : velocity(0, -speed) {
        shape.setSize(Vector2f(5, 10));
        shape.setFillColor(Color::White);
    }
};

int main()
{
    srand(static_cast<unsigned int>(time(NULL)));

    std::cout << "Press Enter to play !" << std::endl;
    std::cin.get(); // Wait for any key press

    std::cout << "Collected facts about Milky Way:" << std::endl; // Display initial message

    RenderWindow window(VideoMode(800, 700), "Save The Earth");

    // Create player spaceship
    RectangleShape player(Vector2f(50, 50)); // Rectangle shape
    player.setFillColor(Color::Green);
    player.setPosition(375.0f, 550.0f);

    // Player speed
    float playerSpeed = 0.04f; // Adjusted player speed

    // Enemies
    const int numEnemies = 10;
    RectangleShape enemies[numEnemies];
    float enemySpeed = 0.03f; // Initial enemy speed
    float speedIncreaseRate = 0.0009f; // Rate of speed increase per frame
    int enemiesDestroyed = 0; // Track the number of enemies destroyed

    for (int i = 0; i < numEnemies; ++i)
    {
        enemies[i] = RectangleShape(Vector2f(30.0f, 30.0f));
        enemies[i].setFillColor(Color::Red);
        enemies[i].setPosition(rand() % (window.getSize().x - 30), -30);
    }

    // Bullets
    std::vector<Bullet> bullets;

    float bulletSpeed = 0.06f;

    Clock clock;

    while (window.isOpen())
    {
        // Event handling
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();
            // Shoot bullets
            if (event.type == Event::KeyPressed && event.key.code == Keyboard::Space) {
                Bullet bullet(bulletSpeed);
                bullet.shape.setPosition(player.getPosition().x + player.getSize().x / 2 - bullet.shape.getSize().x / 2,
                    player.getPosition().y - bullet.shape.getSize().y);
                bullets.push_back(bullet);
            }
        }

        // Player movement
        if (Keyboard::isKeyPressed(Keyboard::Left) && player.getPosition().x > 0)
        {
            player.move(-playerSpeed, 0);
        }
        if (Keyboard::isKeyPressed(Keyboard::Right) && player.getPosition().x + player.getSize().x < window.getSize().x)
        {
            player.move(playerSpeed, 0);
        }

        // Move enemies
        for (int i = 0; i < numEnemies; ++i)
        {
            enemies[i].move(0, enemySpeed);
            if (enemies[i].getPosition().y > window.getSize().y)
            {
                enemies[i].setPosition(rand() % (window.getSize().x - 30), -30);
            }
        }

        // Check for collision with player
        bool playerHit = false; // Flag to indicate if player is hit
        for (int i = 0; i < numEnemies; ++i)
        {
            if (enemies[i].getGlobalBounds().intersects(player.getGlobalBounds())) {
                // End the game if there's a collision
                playerHit = true;
                break;
            }
        }

        // If player is hit, display "Game Over" and close the window
        if (playerHit) {
            std::cout << "Game Over" << std::endl;
            window.close();
        }

        // Increase enemy speed over time
        float deltaTime = clock.restart().asSeconds();
        enemySpeed += speedIncreaseRate * deltaTime;

        // Move bullets
        for (size_t i = 0; i < bullets.size(); ++i) {
            // Move the bullet
            bullets[i].shape.move(bullets[i].velocity);

            // Check if the bullet is out of the screen
            if (bullets[i].shape.getPosition().y < 0 ||
                bullets[i].shape.getPosition().y > window.getSize().y ||
                bullets[i].shape.getPosition().x < 0 ||
                bullets[i].shape.getPosition().x > window.getSize().x) {
                // Remove the bullet if it's out of the screen
                bullets.erase(bullets.begin() + i);
                // Adjust index to account for removed element
                --i;
            }
        }

        // Check bullet-enemy collision
        for (size_t i = 0; i < bullets.size(); ++i) {
            for (size_t j = 0; j < numEnemies; ++j) {
                if (bullets[i].shape.getGlobalBounds().intersects(enemies[j].getGlobalBounds())) {
                    // Remove enemy and bullet
                    enemies[j].setPosition(rand() % (window.getSize().x - 30), -30);
                    bullets.erase(bullets.begin() + i);
                    enemiesDestroyed++;

                    // Display facts based on the number of enemies destroyed
                    if (enemiesDestroyed == 5) {
                        std::cout << "Fact 1: Earth is the only known celestial body where life as we know it exists." << std::endl;
                    }
                    else if (enemiesDestroyed == 10) {
                        std::cout << "Fact 2: The Sun holds 99.8% of the solar system's mass. " << std::endl;
                    }
                    else if (enemiesDestroyed == 15) {
                        std::cout << "Fact 3: The solar system comprises the Sun, planets, moons, asteroids, and comets, interconnected by gravity." << std::endl;
                    }
                    else if (enemiesDestroyed == 20) {
                        std::cout << "Fact 4: Neptune is the eighth planet from the Sun, discovered in 1846." << std::endl;
                    }
                    else if (enemiesDestroyed == 25) {
                        std::cout << "Fact 5: Mars has the largest volcano(Olympus Mons) and deepest canyon(Valles Marineris) in the solar system." << std::endl;
                    }
                    else if (enemiesDestroyed == 30) {
                        std::cout << "Congrats you won the GAME !!!" << std::endl;
                        window.close();
                    }
                    break;
                }
            }
        }

        // Drawing
        window.clear(Color::Black);
        window.draw(player);
        for (int i = 0; i < numEnemies; ++i)
        {
            window.draw(enemies[i]);
        }
        for (auto& bullet : bullets) {
            window.draw(bullet.shape);
        }
        window.display();
    }

    // Close the window, leave the console open
    return 0;
}

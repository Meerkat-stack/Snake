#include <SFML/Graphics.hpp>
#include <vector>
#include <optional>
#include <random>

// Stałe gry
const int TILE_SIZE = 20;
const int GRID_SIZE = 37; // 37 * 20 = 740
const int WINDOW_SIZE = 740;

struct Position {
    int x, y;
    bool operator==(const Position& other) const { return x == other.x && y == other.y; }
};

int main() {
    // SFML 3.0 używa sf::VideoMode ze strukturą sf::Vector2u
    sf::RenderWindow window(sf::VideoMode({WINDOW_SIZE, WINDOW_SIZE}), "Snake SFML 3.0 - Milord Edition");
    window.setFramerateLimit(12);

    // Inicjalizacja węża
    std::vector<Position> snake = {{18, 18}, {18, 19}, {18, 20}};
    Position direction = {0, -1}; // Start w górę

    // Jedzenie
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, GRID_SIZE - 1);
    Position food = {dis(gen), dis(gen)};

    while (window.isOpen()) {
        // --- OBSŁUGA ZDARZEŃ (SFML 3.0 Style) ---
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
        }

        // Sterowanie (Enums w SFML 3.0 są silniej typowane)
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) && direction.y != 1)  direction = {0, -1};
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S) && direction.y != -1) direction = {0, 1};
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) && direction.x != 1)  direction = {-1, 0};
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D) && direction.x != -1) direction = {1, 0};

        // --- LOGIKA ---
        Position newHead = {snake.front().x + direction.x, snake.front().y + direction.y};

        // Kolizja ze ścianami (740x740)
        if (newHead.x < 0 || newHead.x >= GRID_SIZE || newHead.y < 0 || newHead.y >= GRID_SIZE) {
            window.close(); // Prosty koniec gry
        }

        // Kolizja z samym sobą
        for (const auto& segment : snake) {
            if (newHead == segment) window.close();
        }

        snake.insert(snake.begin(), newHead);

        // Jedzenie owocu
        if (newHead == food) {
            food = {dis(gen), dis(gen)};
        } else {
            snake.pop_back();
        }

        // --- RYSOWANIE ---
        window.clear(sf::Color(44, 62, 80)); // Nowoczesny granatowy

        // Rysuj jedzenie
        sf::RectangleShape foodRect(sf::Vector2f({TILE_SIZE - 2.0f, TILE_SIZE - 2.0f}));
        foodRect.setFillColor(sf::Color::Red);
        foodRect.setPosition(sf::Vector2f({(float)food.x * TILE_SIZE, (float)food.y * TILE_SIZE}));
        window.draw(foodRect);

        // Rysuj węża
        for (size_t i = 0; i < snake.size(); ++i) {
            sf::RectangleShape segmentRect(sf::Vector2f({TILE_SIZE - 2.0f, TILE_SIZE - 2.0f}));
            segmentRect.setFillColor(i == 0 ? sf::Color::Cyan : sf::Color::White);
            segmentRect.setPosition(sf::Vector2f({(float)snake[i].x * TILE_SIZE, (float)snake[i].y * TILE_SIZE}));
            window.draw(segmentRect);
        }

        window.display();
    }

    return 0;
}
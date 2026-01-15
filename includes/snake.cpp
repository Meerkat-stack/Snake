#include <SFML/Graphics.hpp>
#include "player.h"
#include <deque>
#include <vector>
#include <random>
#include <string>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <algorithm> 
#include <iostream>
#include "colors.h"

extern Player player;

// --- Konfiguracja gry ---
const int TILE_SIZE = 46;//Rozmiar kafelków na planszy
const int WINDOW_WIDTH = 740;//1000;
const int UI_HEIGHT = 60;//Wysokość tego paska na dane
const int GAP = 30;
const int WINDOW_HEIGHT = 740;//800 + UI_HEIGHT;

const float BASE_SPEED = 0.13f;//Prędkość, większa wartość-wolniejszy wąż
const float MAX_SPEED_CAP = 0.04f;
int APPLES_TO_MAX = 150;//Maksymalna ilość zjedzonych objektó, definiuje jak szybko waż będzie przyspieszał

enum class Direction { Up, Down, Left, Right };


struct SnakeGame {

    //Piotr
    constant currentConstant; // Przechowuje aktualną stałą

    int currentMode;

    // Funkcja do ustawienia stałej 
    void setGameConstant(const constant& c,int& gamemode) 
    {
    currentConstant = c;
    currentMode = gamemode; // <--- Tutaj ją "zapamiętujesz"
        
        // Możesz też od razu ustawić trudność, o którą pytałeś wcześniej
        APPLES_TO_MAX = 150 - (currentMode * 15);
    }


    sf::RenderWindow* window;//Dodany wskaźnik (zmiana to *)

    std::deque<sf::Vector2i> snake;
    sf::Vector2i food;
    Direction dir;
    bool isGameOver;
    bool consolePrinted; // Flaga, żeby wypisać wynik w konsoli tylko raz

    sf::Clock moveClock;
    sf::Clock gameTimeClock;
    float moveTimer;

    int score;
    int highScore;
    float totalTime;

    std::mt19937 rng;

    // --- UI i Teksty ---
    sf::Font font;
    sf::Font math_font;
    sf::Text scoreLabel, scoreValue;
    sf::Text highLabel, highValue;
    sf::Text timeLabel, timeValue;

    SnakeGame(sf::RenderWindow* existingWindow)//Dodany argument
        : window(existingWindow),
        dir(Direction::Right),
        isGameOver(false),
        consolePrinted(false),
        moveTimer(0),
        score(0),
        highScore(0),
        totalTime(0),
        rng(std::random_device{}()),
        scoreLabel(font), scoreValue(font),
        highLabel(font), highValue(font),
        timeLabel(font), timeValue(font)
    {
        // window.setFramerateLimit(60);

        if (!font.openFromFile("fonts/BebasNeue-Regular.ttf")) {
            // Jeśli ta nie zadziała, spróbuj systemowego ariala jako plan awaryjny
            if (!font.openFromFile("C:/Windows/Fonts/arial.ttf")) {
                std::cout << "NIE ZNALEZIONO ZADNEJ CZCIONKI DLA SNAKE!" << std::endl;
            }
        }

        if (!math_font.openFromFile("fonts/arial/arial.ttf")) {
            // Jeśli ta nie zadziała, spróbuj systemowego ariala jako plan awaryjny
            if (!font.openFromFile("C:/Windows/Fonts/arial.ttf")) {
                std::cout << "NIE ZNALEZIONO ZADNEJ CZCIONKI DLA SNAKE!" << std::endl;
            }
        }

        loadHighScore();
        setupHUD();
        resetGame();
    }

    // Zwraca adres w pamięci (float*), pod którym znajduje się totalTime
    float* getTimePointer() {
        return &totalTime;
    }

    void setupHUD() {//Tworzy ekran ze statystykami
        unsigned int charSize = 45;
        sf::Color labelColor = sf::Color(Dark_color);
        sf::Color valueColor = sf::Color(Dark_color);
        float uiY = 20.f;

        scoreLabel.setString("SCORE:"); scoreLabel.setCharacterSize(charSize); scoreLabel.setFillColor(labelColor); scoreLabel.setPosition({ 1.0f/15.0f*WINDOW_WIDTH, uiY*1.04f });
        scoreValue.setCharacterSize(charSize); scoreValue.setFillColor(valueColor); scoreValue.setPosition({ 1.0f/15.0f*WINDOW_WIDTH+ 105.0f, uiY*1.04f });

        highLabel.setString("HI-SCORE:"); highLabel.setCharacterSize(charSize); highLabel.setFillColor(labelColor); highLabel.setPosition({6.0f/15.0f*WINDOW_WIDTH-30.0f, uiY*1.04f });
        highValue.setCharacterSize(charSize); highValue.setFillColor(valueColor); highValue.setPosition({ 6.0f/15.0f*WINDOW_WIDTH+ 115.0f, uiY*1.04f });

        timeLabel.setString("TIME:"); timeLabel.setCharacterSize(charSize); timeLabel.setFillColor(labelColor); timeLabel.setPosition({ 11.0f/15.0f*WINDOW_WIDTH-20.0f, uiY*1.04f });
        timeValue.setCharacterSize(charSize); timeValue.setFillColor(valueColor); timeValue.setPosition({ 11.0f/15.0f*WINDOW_WIDTH+ 70.0f, uiY*1.04f });

    }

    void loadHighScore() {
        std::ifstream inputFile("userdata/highscore.txt");
        if (inputFile.is_open()) { inputFile >> highScore; inputFile.close(); }
        else { highScore = 0; }
    }

    void saveHighScore() {
        std::ofstream outputFile("userdata/highscore.txt");
        if (outputFile.is_open()) { outputFile << highScore; outputFile.close(); }
    }

    void resetGame() {
        snake.clear();//Położenie początkowe węża
        snake.push_back({ 5, 4 });
        snake.push_back({ 4, 4 });
        snake.push_back({ 3, 4 });

        dir = Direction::Right;
        score = 0;
        isGameOver = false;
        consolePrinted = false; // Resetujemy flagę konsoli

        spawnFood();
        gameTimeClock.restart();
        totalTime = 0;
    }

    void spawnFood() {
        int cols = WINDOW_WIDTH / TILE_SIZE;
        int rows = (WINDOW_HEIGHT - UI_HEIGHT) / TILE_SIZE;

        std::uniform_int_distribution<int> distX(0, cols - 1);
        std::uniform_int_distribution<int> distY(0, rows - 1);

        bool validPosition = false;
        while (!validPosition) {
            food = { distX(rng), distY(rng) };
            validPosition = true;
            for (const auto& segment : snake) {
                if (segment == food) { validPosition = false; break; }
            }
        }
    }

    void processInput() {
        while (const auto event = window->pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window->close();
            }
        }

        if (!isGameOver) {
            if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))&& dir != Direction::Down) dir = Direction::Up;
            else if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))&& dir != Direction::Up) dir = Direction::Down;
            else if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))&& dir != Direction::Right) dir = Direction::Left;
            else if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))&& dir != Direction::Left) dir = Direction::Right;
        }

        if (isGameOver && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)) {
            resetGame();
        }
    }

    void update() {
        //Odblokowuje poziomy
        if(player.unlocked_maps_count==1&&player.total_play_time_sec>30.0f)player.unlocked_maps_count++;
        if(player.unlocked_maps_count==2&&player.total_play_time_sec>90.0f)player.unlocked_maps_count++;
        if(player.unlocked_maps_count==3&&player.total_play_time_sec>210.0f)player.unlocked_maps_count++;
        if(player.unlocked_maps_count==4&&player.total_play_time_sec>420.0f)player.unlocked_maps_count++;
        if(player.unlocked_maps_count==5&&player.total_play_time_sec>720.0f)player.unlocked_maps_count++;
        if(player.unlocked_maps_count==6&&player.total_play_time_sec>1200.0f)player.unlocked_maps_count++;
        if(player.unlocked_maps_count==7&&player.total_play_time_sec>1800.0f)player.unlocked_maps_count++;

        
        // Jeśli gra się skończyła, sprawdź czy już wypisaliśmy info w konsoli
        if (isGameOver) {
            if (!consolePrinted) {
                //Wypisuje wskaźnik
                // float* ptr = getTimePointer(); // Pobieramy wskaźnik

                // std::cout << "--- KONIEC GRY ---" << std::endl;
                // std::cout << "Adres zmiennej czasu (w pamieci): " << ptr << std::endl;
                // std::cout << "Wartosc pod tym adresem (sekundy): " << *ptr << std::endl;
                // std::cout << "------------------" << std::endl;

                //Dodaje spędzony czas na grze
                player.total_play_time_sec += static_cast<int>(totalTime);
                
                //Sprawdza czy wąż osiągnął rekordową długość
                if ((snake.size()-3 )> player.snake_max_length) {
                    player.snake_max_length = snake.size()-3;
                    std::cout<<snake.size()-3;//Debug
                }

                consolePrinted = true; // Blokada, żeby nie wypisywać w kółko
            }
            return;
        }

        float dt = moveClock.restart().asSeconds();
        moveTimer += dt;
        totalTime = gameTimeClock.getElapsedTime().asSeconds();

        // UI Update, edytuje statystyki
        scoreValue.setString(std::to_string(score));
        highValue.setString(std::to_string(highScore));
        int totalSeconds = static_cast<int>(totalTime);
        std::ostringstream timeStream;
        timeStream << std::setfill('0') << std::setw(2) << (totalSeconds / 60) << ":"
            << std::setfill('0') << std::setw(2) << (totalSeconds % 60);
        timeValue.setString(timeStream.str());

        // Prędkość
        float currentSpeed = BASE_SPEED;
        float progress = std::min(static_cast<float>(score), static_cast<float>(APPLES_TO_MAX)) / static_cast<float>(APPLES_TO_MAX);
        currentSpeed = BASE_SPEED - (progress * (BASE_SPEED - MAX_SPEED_CAP));

        if (moveTimer >= currentSpeed) {//Parametry węża
            moveTimer = 0;

            //Dodaje przebyty dystans do statystyk gracza
            player.total_pixels_traveled += static_cast<float>(TILE_SIZE);

            sf::Vector2i newHead = snake.front();
            switch (dir) {
            case Direction::Up:    newHead.y -= 1; break;
            case Direction::Down:  newHead.y += 1; break;
            case Direction::Left:  newHead.x -= 1; break;
            case Direction::Right: newHead.x += 1; break;
            }

            int cols = WINDOW_WIDTH / TILE_SIZE;
            int rows = (WINDOW_HEIGHT - UI_HEIGHT) / TILE_SIZE;

            if (newHead.x < 0 || newHead.x >= cols || newHead.y < 0 || newHead.y >= rows) {
                isGameOver = true;
                player.deaths_wall++;//Zwiększanie satystyk
                if (score > highScore) { highScore = score; saveHighScore(); }
                return;
            }
            for (const auto& segment : snake) {
                if (segment == newHead) {
                    isGameOver = true;
                    player.deaths_self++;//Zwiększanie statystyk
                    if (score > highScore) { highScore = score; saveHighScore(); }
                    return;
                }
            }

            snake.push_front(newHead);

            if (newHead == food) {
                score++;
                if (score > highScore) { highScore = score; saveHighScore(); }
                spawnFood();
            }
            else {
                snake.pop_back();
            }
        }
    }


    //Piotr
void render() {
        window->clear(sf::Color(Basil_color));

        int cols = WINDOW_WIDTH / TILE_SIZE;
        int rows = (WINDOW_HEIGHT - UI_HEIGHT) / TILE_SIZE;
        sf::RectangleShape bgRect(sf::Vector2f(static_cast<float>(TILE_SIZE), static_cast<float>(TILE_SIZE)));

        for (int y = 0; y < rows; ++y) {
            for (int x = 0; x < cols; ++x) {

                bgRect.setPosition({
                    static_cast<float>(x * TILE_SIZE),
                    static_cast<float>(y * TILE_SIZE + UI_HEIGHT + GAP)
                    });

                if ((x + y) % 2 == 0){
                    bgRect.setFillColor(sf::Color(Basil_color));
                    bgRect.setOutlineThickness(2);
                    bgRect.setOutlineColor(sf::Color(Olive_color));
                }
                else{
                    bgRect.setFillColor(sf::Color(Basil_color));
                    bgRect.setOutlineThickness(2);
                    bgRect.setOutlineColor(sf::Color(Dark_color));
                }

                window->draw(bgRect);
            }
        }

        sf::RectangleShape gameRect(sf::Vector2f(static_cast<float>(TILE_SIZE), static_cast<float>(TILE_SIZE)));
        gameRect.setSize({ static_cast<float>(TILE_SIZE - 1), static_cast<float>(TILE_SIZE - 1) });


        sf::Text snakeText(math_font); 
        snakeText.setFillColor(sf::Color(Dark_color));
        snakeText.setCharacterSize(TILE_SIZE - 10);

        for (size_t i = 0; i < snake.size(); ++i) {

            gameRect.setPosition({
                static_cast<float>(snake[i].x * TILE_SIZE),
                static_cast<float>(snake[i].y * TILE_SIZE + UI_HEIGHT + GAP)
                });

            if (i == 0){ gameRect.setFillColor(sf::Color(Olive_color));gameRect.setOutlineColor(sf::Color(Dark_color));gameRect.setOutlineThickness(2);}//Tło snake
            else {gameRect.setFillColor(sf::Color(Olive_color));gameRect.setOutlineColor(sf::Color(Dark_color));gameRect.setOutlineThickness(2);}//Tło snake
            window->draw(gameRect);

            //Wyświetla znai
            sf::String textDisplay = "";
            if (i == 0) {
                textDisplay = currentConstant.symbol;
            }
            else if (i == 1) {
                textDisplay = (L"\u2248");
            }
            else {
                size_t digitIndex = i - 2; 
                if (digitIndex < currentConstant.digits.length()) {
                    textDisplay = currentConstant.digits[digitIndex];
                }
            }

            snakeText.setString(textDisplay);
            
            // left -> position.x
            // top -> position.y
            // width -> size.x
            // height -> size.y
            sf::FloatRect textRect = snakeText.getLocalBounds();
            snakeText.setOrigin({
                textRect.position.x + textRect.size.x / 2.0f, 
                textRect.position.y + textRect.size.y / 2.0f
            });
            //Pozycja snake
            snakeText.setPosition({
                gameRect.getPosition().x + TILE_SIZE/2.0f,
                gameRect.getPosition().y + TILE_SIZE/2.0f
            });

            window->draw(snakeText);
        }

        // Jedzenie
        gameRect.setPosition({
            static_cast<float>(food.x * TILE_SIZE),
            static_cast<float>(food.y * TILE_SIZE + UI_HEIGHT+GAP)
            });
        gameRect.setFillColor(sf::Color(Red_color));
        window->draw(gameRect);

        //Wyświetl cyfrę na jedzeniu (tę, która będzie następna)
        size_t nextDigitIndex = snake.size() - 2;
        if (nextDigitIndex >= 0 && nextDigitIndex < currentConstant.digits.length()) {
             snakeText.setString(sf::String(currentConstant.digits[nextDigitIndex]));
             sf::FloatRect textRect = snakeText.getLocalBounds();
             
             snakeText.setOrigin({
                textRect.position.x + textRect.size.x / 2.0f,
                textRect.position.y + textRect.size.y / 2.0f
             });
             snakeText.setPosition({
                gameRect.getPosition().x + TILE_SIZE/2.0f,
                gameRect.getPosition().y + TILE_SIZE/2.0f
             });
             window->draw(snakeText);
        }

        //UI Tło
        sf::RectangleShape uiBar(sf::Vector2f(static_cast<float>(WINDOW_WIDTH), static_cast<float>(UI_HEIGHT*1.55)));
        uiBar.setFillColor(sf::Color(Apatite_color));
        uiBar.setOutlineThickness(-4);
        uiBar.setOutlineColor(sf::Color(Dark_color));
        uiBar.setPosition({ 0.f, 0.f });
        window->draw(uiBar);

        //Podziałki UI
        sf::RectangleShape bar0({4.0f,static_cast<float>(UI_HEIGHT*1.55)});
        bar0.setOrigin(bar0.getGeometricCenter()); 
        bar0.setFillColor(sf::Color(Dark_color));
        bar0.setPosition({WINDOW_WIDTH*0.3f,20.0f*2.2f});

        sf::RectangleShape bar1({4.0f,static_cast<float>(UI_HEIGHT*1.55)});
        bar1.setOrigin(bar1.getGeometricCenter());
        bar1.setFillColor(sf::Color(Dark_color));
        bar1.setPosition({WINDOW_WIDTH*0.65f,20.0f*2.2f});

        window->draw(bar0);
        window->draw(bar1);

        // Teksty UI
        window->draw(scoreLabel); window->draw(scoreValue);
        window->draw(highLabel); window->draw(highValue);
        window->draw(timeLabel); window->draw(timeValue);

        // Ekran Game Over
        if (isGameOver) {
            sf::RectangleShape overlay(sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
            overlay.setFillColor(sf::Color(0, 0, 0, 75));
            window->draw(overlay);

            sf::RectangleShape frame({300.0f,200.0f});
            frame.setFillColor(sf::Color(Apatite_color));
            frame.setOrigin(frame.getGeometricCenter());
            frame.setOutlineColor(sf::Color(Dark_color));
            frame.setOutlineThickness(4);
            frame.setPosition({WINDOW_WIDTH/2.0f,WINDOW_HEIGHT/2.0f+5.0f});
            window->draw(frame);

            sf::Text overText(font);
            overText.setString("GAME OVER");
            overText.setCharacterSize(70);
            overText.setFillColor(sf::Color(Red_color));

            // Poprawka Rect dla SFML 3.0
            sf::FloatRect rect = overText.getLocalBounds();
            overText.setOrigin({ 
                rect.position.x + rect.size.x / 2.0f, 
                rect.position.y + rect.size.y / 2.0f 
            });
            overText.setPosition({ WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f - 40.f });

            sf::Text subText(font);
            subText.setString("Press SPACE to Restart");
            subText.setCharacterSize(30);
            subText.setFillColor(sf::Color(Dark_color));

            sf::Text subText2(font);
            subText2.setString("Press ESC for Menu");
            subText2.setCharacterSize(30);
            subText2.setFillColor(sf::Color(Dark_color));

            rect = subText.getLocalBounds();
            subText.setOrigin({ 
                rect.position.x + rect.size.x / 2.0f, 
                rect.position.y + rect.size.y / 2.0f 
            });
            subText.setPosition({ WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f + 30.f });

            rect = subText2.getLocalBounds();
            subText2.setOrigin({ 
                rect.position.x + rect.size.x / 2.0f, 
                rect.position.y + rect.size.y / 2.0f 
            });
            subText2.setPosition({ WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f + 70.f });

            window->draw(overText);
            window->draw(subText);
            window->draw(subText2);
        }
    }

    // void run() {
    //     while (window->isOpen()) {
    //         processInput();
    //         update();
    //         render();
    //     }
    // }

    void playStep() {
        processInput();
        update();
        render();
    }
};




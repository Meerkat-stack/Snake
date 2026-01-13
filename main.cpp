#include <iostream>
#include "includes/seciurity.h"
#include "includes/player.h"
#include "includes/button.h"
#include "includes/window_look.h"
#include "includes/loging_page.h"
#include "includes/signup_page.h"

#include <SFML/Graphics.hpp>
#include <deque>
#include <vector>
#include <random>
#include <string>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <algorithm> 


#define Basil_color 0x1A471BFF //Że w sęsie ciemny zielony
#define Piemontite_color  0x1A471BFF//0x964B67FF//Że w sęsie fioletowy      //Zamieniłem basil_color na ten bo różowy mi się przestął chyba podobać
#define Apatite_color 0xF0FC97FF //Że w sęsie żółty
#define Olive_color 0x92A22EFF //Że w sęsie jasny zielony
#define Dark_color 0x161616FF
#define Red_color 0xD7443EFF //kolor do ostrzeżeń

#define max_num_of_buttons 3

// --- Konfiguracja gry ---
const int TILE_SIZE = 25;
const int WINDOW_WIDTH = 1000;
const int UI_HEIGHT = 80;
const int WINDOW_HEIGHT = 800 + UI_HEIGHT;

const float BASE_SPEED = 0.1f;
const float MAX_SPEED_CAP = 0.04f;
const int APPLES_TO_MAX = 20;

enum class Direction { Up, Down, Left, Right };



struct SnakeGame {
    sf::RenderWindow window;

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
    sf::Text scoreLabel, scoreValue;
    sf::Text highLabel, highValue;
    sf::Text timeLabel, timeValue;

    SnakeGame()
        : window(sf::VideoMode({ WINDOW_WIDTH, WINDOW_HEIGHT }), "Snake SFML 3.0.2 - Pointers"),
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
        window.setFramerateLimit(60);

        if (!font.openFromFile("font.ttf")) {
            if (!font.openFromFile("C:\\Windows\\Fonts\\arial.ttf")) {}
        }

        loadHighScore();
        setupHUD();
        resetGame();
    }

    // --- 2. FUNKCJA ZWRACAJĄCA WSKAŹNIK ---
    // Zwraca adres w pamięci (float*), pod którym znajduje się totalTime
    float* getTimePointer() {
        return &totalTime;
    }

    void setupHUD() {
        unsigned int charSize = 30;
        sf::Color labelColor = sf::Color(255, 215, 0);
        sf::Color valueColor = sf::Color::White;
        float uiY = 20.f;

        scoreLabel.setString("SCORE:"); scoreLabel.setCharacterSize(charSize); scoreLabel.setFillColor(labelColor); scoreLabel.setPosition({ 40.f, uiY });
        scoreValue.setCharacterSize(charSize); scoreValue.setFillColor(valueColor); scoreValue.setPosition({ 180.f, uiY });

        highLabel.setString("HI-SCORE:"); highLabel.setCharacterSize(charSize); highLabel.setFillColor(labelColor); highLabel.setPosition({ 400.f, uiY });
        highValue.setCharacterSize(charSize); highValue.setFillColor(valueColor); highValue.setPosition({ 600.f, uiY });

        timeLabel.setString("TIME:"); timeLabel.setCharacterSize(charSize); timeLabel.setFillColor(labelColor); timeLabel.setPosition({ 800.f, uiY });
        timeValue.setCharacterSize(charSize); timeValue.setFillColor(valueColor); timeValue.setPosition({ 900.f, uiY });
    }

    void loadHighScore() {
        std::ifstream inputFile("highscore.txt");
        if (inputFile.is_open()) { inputFile >> highScore; inputFile.close(); }
        else { highScore = 0; }
    }

    void saveHighScore() {
        std::ofstream outputFile("highscore.txt");
        if (outputFile.is_open()) { outputFile << highScore; outputFile.close(); }
    }

    void resetGame() {
        snake.clear();
        snake.push_back({ 10, 10 });
        snake.push_back({ 9, 10 });
        snake.push_back({ 8, 10 });

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
        while (const auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
        }

        if (!isGameOver) {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up) && dir != Direction::Down) dir = Direction::Up;
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down) && dir != Direction::Up) dir = Direction::Down;
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) && dir != Direction::Right) dir = Direction::Left;
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right) && dir != Direction::Left) dir = Direction::Right;
        }

        if (isGameOver && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)) {
            resetGame();
        }
    }

    void update() {
        // Jeśli gra się skończyła, sprawdź czy już wypisaliśmy info w konsoli
        if (isGameOver) {
            if (!consolePrinted) {
                // --- 3. UŻYCIE WSKAŹNIKA I WYPISANIE W KONSOLI ---
                float* ptr = getTimePointer(); // Pobieramy wskaźnik

                std::cout << "--- KONIEC GRY ---" << std::endl;
                std::cout << "Adres zmiennej czasu (w pamieci): " << ptr << std::endl;
                std::cout << "Wartosc pod tym adresem (sekundy): " << *ptr << std::endl;
                std::cout << "------------------" << std::endl;

                consolePrinted = true; // Blokada, żeby nie wypisywać w kółko
            }
            return;
        }

        float dt = moveClock.restart().asSeconds();
        moveTimer += dt;
        totalTime = gameTimeClock.getElapsedTime().asSeconds();

        // UI Update
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

        if (moveTimer >= currentSpeed) {
            moveTimer = 0;

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
                if (score > highScore) { highScore = score; saveHighScore(); }
                return;
            }
            for (const auto& segment : snake) {
                if (segment == newHead) {
                    isGameOver = true;
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

    void render() {
        window.clear(sf::Color(20, 20, 20));

        int cols = WINDOW_WIDTH / TILE_SIZE;
        int rows = (WINDOW_HEIGHT - UI_HEIGHT) / TILE_SIZE;
        sf::RectangleShape bgRect(sf::Vector2f(static_cast<float>(TILE_SIZE), static_cast<float>(TILE_SIZE)));

        for (int y = 0; y < rows; ++y) {
            for (int x = 0; x < cols; ++x) {
                bgRect.setPosition({
                    static_cast<float>(x * TILE_SIZE),
                    static_cast<float>(y * TILE_SIZE + UI_HEIGHT)
                    });

                if ((x + y) % 2 == 0)
                    bgRect.setFillColor(sf::Color(18, 224, 37));
                else
                    bgRect.setFillColor(sf::Color(25, 191, 40));

                window.draw(bgRect);
            }
        }

        sf::RectangleShape gameRect(sf::Vector2f(static_cast<float>(TILE_SIZE), static_cast<float>(TILE_SIZE)));
        gameRect.setSize({ static_cast<float>(TILE_SIZE - 1), static_cast<float>(TILE_SIZE - 1) });

        for (size_t i = 0; i < snake.size(); ++i) {
            gameRect.setPosition({
                static_cast<float>(snake[i].x * TILE_SIZE),
                static_cast<float>(snake[i].y * TILE_SIZE + UI_HEIGHT)
                });
            if (i == 0) gameRect.setFillColor(sf::Color(0, 97, 255));
            else gameRect.setFillColor(sf::Color(0, 80, 220));
            window.draw(gameRect);
        }

        gameRect.setPosition({
            static_cast<float>(food.x * TILE_SIZE),
            static_cast<float>(food.y * TILE_SIZE + UI_HEIGHT)
            });
        gameRect.setFillColor(sf::Color::Red);
        window.draw(gameRect);

        sf::RectangleShape uiBar(sf::Vector2f(static_cast<float>(WINDOW_WIDTH), static_cast<float>(UI_HEIGHT)));
        uiBar.setFillColor(sf::Color(30, 30, 30));
        uiBar.setOutlineThickness(-2);
        uiBar.setOutlineColor(sf::Color(100, 100, 100));
        uiBar.setPosition({ 0.f, 0.f });
        window.draw(uiBar);

        window.draw(scoreLabel); window.draw(scoreValue);
        window.draw(highLabel); window.draw(highValue);
        window.draw(timeLabel); window.draw(timeValue);

        if (isGameOver) {
            sf::RectangleShape overlay(sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
            overlay.setFillColor(sf::Color(0, 0, 0, 150));
            window.draw(overlay);

            sf::Text overText(font);
            overText.setString("GAME OVER");
            overText.setCharacterSize(70);
            overText.setFillColor(sf::Color::Red);

            sf::FloatRect rect = overText.getLocalBounds();
            overText.setOrigin({ rect.position.x + rect.size.x / 2.0f, rect.position.y + rect.size.y / 2.0f });
            overText.setPosition({ WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f - 40.f });

            sf::Text subText(font);
            subText.setString("Press SPACE to Restart");
            subText.setCharacterSize(30);
            subText.setFillColor(sf::Color::White);

            rect = subText.getLocalBounds();
            subText.setOrigin({ rect.position.x + rect.size.x / 2.0f, rect.position.y + rect.size.y / 2.0f });
            subText.setPosition({ WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f + 40.f });

            window.draw(overText);
            window.draw(subText);
        }

        window.display();
    }

    void run() {
        while (window.isOpen()) {
            processInput();
            update();
            render();
        }
    }
};





Player player;

/* to do int main trzeba dać żeby odpalić gre
SnakeGame game;
game.run();
*/


int main()
{
    //Rozmiary okna
    unsigned int width = 740;
    unsigned int height = 740;

    int state = 0;//Zmienna odpowiedzialna za wybór otwieranej strony
    /*
    0 - Przyciski na starcie, wybór logowania
    1 - Sign in
    2 - Sign up
    3 - Guess
    4 - Game menu
    5 - Play!
    */
   int error_log = 0;
   /*
   0 - niezalogowany
   -1 - błędne dane
   1 - zalogowany
   */
   int active_field = -1;//Zmienna określająca które pole ma przyjmować tekst, tam ustawia kursor
   unsigned int frame_count=0;//Zlicza klatki, przyda się do kursora


    //Torzy okno
    sf::RenderWindow * window = new sf::RenderWindow(sf::VideoMode({width,height}),"Irrationa Snake");

    window->setFramerateLimit(60);//Ustawia klatkarz

    //Ustawia ramkę wokoło ekranu
    sf::RectangleShape border({(float)width,(float)height});
    border = build_border(width,height,width/2.0f,height/2.0f,Dark_color);

    sf::Font font("fonts/BebasNeue-Regular.ttf");//ładuje czcionkę

    //Naprawia rozpikselizowany tekst
    const_cast<sf::Texture&>(font.getTexture((int)(0.5f*0.037f*height+3))).setSmooth(false);//Lepsza jakość tekstu
    const_cast<sf::Texture&>(font.getTexture(0.035f*height)).setSmooth(false);//Lepsza jakość tekstu

    //Tworzy przyciski na ekranie startowym
    sf::RectangleShape buttons[max_num_of_buttons];//Tablica przechowująca przyciski
    sf::Text buttons_labels[max_num_of_buttons]={sf::Text(font),sf::Text(font),sf::Text(font)};//Tablica przechowująca tekst

    build_start_bttons(width,height,Apatite_color,Dark_color,buttons,buttons_labels,font);
    //||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
    //state = 1 | Sign in
    sf::RectangleShape text_frames[4];//Tablica przechowująca przyciski
    sf::Text labels_log[2]={sf::Text(font),sf::Text(font)};//Przechowuje napisy przy polach do logowania
    sf::RectangleShape login_button({0.12f*width,0.05f*height});//Przycisk logowania
    sf::Text login_button_label(font, "Log in", 0.035f*height);//Etykieta przycisku logowania
    sf::RectangleShape back_button({0.12f*width,0.05f*height});//Przycisk logowania
    sf::Text back_button_label(font, "Back", 0.035f*height);//Etykieta przycisku logowania



    //Pola i ramki
    text_frames[0] = text_frame_bg(0.24f*width+10,0.04f*height+20,width/2.0f-5,height/2.0f+0.06f*height-10,Apatite_color,Dark_color);
    text_frames[1] = text_frame_bg(0.24f*width+10,0.04f*height+20,width/2.0f-5,height/2.0f-0.06f*height-10,Apatite_color,Dark_color);
    text_frames[2] = text_frame(0.24f*width,0.04f*height,width/2.0f,height/2.0f-0.06f*height,Dark_color);
    text_frames[3] = text_frame(0.24f*width,0.04f*height,width/2.0f,height/2.0f+0.06f*height,Dark_color);

    //Opis ramek
    labels_log[0] = label((int)(0.5f*0.037f*height+3),(int)( width/2.0f-0.1f*width),(int)(height/2.0f-0.06f*height-0.029f*height), "Login",Dark_color,font);
    labels_log[1] = label((int)(0.5f*0.037f*height+3), (int)(width/2.0f-0.1f*width+10),(int)(height/2.0f+0.06f*height-0.029f*height), "Password",Dark_color,font);

    //Przyciski
    login_button = build_button(0.12f*width,0.05f*height,width/2.0f+44,height/2.0f+0.155f*height,Apatite_color,Dark_color);
    login_button_label = label(0.035f*height, width/2.0f+44,height/2.0f+0.155f*height, "Log in",Dark_color,font);

    back_button = build_button(0.12f*width,0.05f*height,width/2.0f-55,height/2.0f+0.155f*height,Apatite_color,Dark_color);
    back_button_label = label(0.035f*height, width/2.0f-55,height/2.0f+0.155f*height, "Back",Dark_color,font);

    //Kursor
    sf::RectangleShape cursor({2.0f,29.0f});
    cursor.setFillColor(sf::Color(Dark_color));
    cursor.setOrigin(cursor.getGeometricCenter());
    int char_count[3];//Zlicza znaki aby przesuwać kursor

    //Wpisywanie danych do logowania
    std::string login_input = "";
    std::string password_input = "";
    std::string hide_password_input = "";

    //Tekst do wyświetlenia
    //Login
    sf::Text login_input_text(font, login_input, 0.035f*height);//Ustawia dla napisu, czcionkę, tekst, rozmiar
    login_input_text.setFillColor(sf::Color(Dark_color));
    sf::FloatRect bounds = login_input_text.getLocalBounds();
    login_input_text.setOrigin({bounds.position.x, 0.f});//Wyrównuje do lewej
    login_input_text.setPosition({width/2.0f-83,height/2.0f-0.06f*height-16});
    //Password
    sf::Text password_input_text(font, hide_password_input, 0.035f*height);//Ustawia dla napisu, czcionkę, tekst, rozmiar
    password_input_text.setFillColor(sf::Color(Dark_color));
    sf::FloatRect bounds2 = password_input_text.getLocalBounds();
    password_input_text.setOrigin({bounds.position.x, 0.f});//Wyrównuje do lewej
    password_input_text.setPosition({width/2.0f-83,height/2.0f+0.06f*height-12});

    //Ostrzeżenie o złych danych podanych przy logowaniu
    sf::RectangleShape error_log_frame({0.24f*width+10+20,(0.04f*height+20)*4.7f});
    error_log_frame.setOrigin(error_log_frame.getGeometricCenter());
    error_log_frame.setPosition({width/2.0f-5,height/2.0f+26});
    error_log_frame.setFillColor(sf::Color::Transparent);
    error_log_frame.setOutlineColor(sf::Color(Red_color));
    error_log_frame.setOutlineThickness(2);
    //Komunikat
    sf::Text error_log_text(font,"",(int)(0.5f*0.037f*height+3));
    error_log_text = label((int)(0.5f*0.037f*height+3),(int)(width/2.0f*0.918),(int)(height*0.36f), "Invalid username or password",Red_color,font);
    error_log_text.setOutlineColor(sf::Color(Dark_color));
    error_log_text.setOutlineThickness(-0.5);




    //||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
    //state = 3 || state == 4 | Game menu
    sf::RectangleShape statistic_bg({width*0.85f,height*0.4f});
    statistic_bg.setOrigin(statistic_bg.getGeometricCenter());
    statistic_bg.setPosition({width/2.0f,height/4.0f*1.05f});
    statistic_bg.setFillColor(sf::Color(Apatite_color));
    statistic_bg.setOutlineColor(sf::Color(Dark_color));
    statistic_bg.setOutlineThickness(2);
    

    sf::Text name(font,player.name,(int)(0.035f*height));
    name.setFillColor(sf::Color(Dark_color));
    // text.setOutlineColor(sf::Color(Dark_color));
    // text.setOutlineThickness(-1.f);

    //Ustawia punkt odniesienia napisu 
    sf::FloatRect bounds_name = name.getLocalBounds();
    // text.setOrigin({bounds.position.x, 0.f});//Wyrównuje do lewej
    // text.setOrigin({bounds.position.x+bounds.size.x, 0.f});//Wyrównuje do prawej
    name.setOrigin({bounds_name.position.x+bounds.size.x/2,//Wyrównuje do środka
                    (float)name.getCharacterSize()*0.6f});//To ustawia dolny punkt odniesienia na linijkę do pisania
                    //Tak samo jak masz linijki w zeszycie to tu the same
    name.setPosition({width/2.0f,height/4.0f});




    //Pętla gry, wykonuje się do puki do puty okno jest otwarte
    while(window->isOpen()){
        {//Zlicza klatki dla kursora
        if(frame_count> 60) frame_count = 0;
        else frame_count++;
        }

        //Czyta zdażenia, takie jak przyciski,ruchy myszką itd., otem wykonuje odpowiednie akcje
        //Pętla zdażeń
        while (const std::optional<sf::Event> event = window->pollEvent()){

            //Jak wcisnąć krzyżyk to sie zamyka
            if(event->is<sf::Event::Closed>()){
                save_player();
                window->close();//Bez tego krzyżyk nie działa
            }

            //Sprawdza klawisze z klawiatury
            else if(const auto* keyPress = event->getIf<sf::Event::KeyPressed>()){
                //Ustawia klawisz esc jako wyjście
                if(keyPress->scancode == sf::Keyboard::Scancode::Escape){
                    save_player();
                    window->close();
                }
                //Klawisz TAB
                if (keyPress->code == sf::Keyboard::Key::Tab) {
                    if(state==1||state==2){
                        if(active_field==0){
                            // Przełącza poa do pisania
                            active_field = 1;
                            // Resetuje frame_count, żeby kursor od razu się pojawił w nowym polu
                            frame_count = 0; 
                        }
                    }
                }
                //Klawisz ENTER
                if (keyPress->code == sf::Keyboard::Key::Enter) {
                    // std::cout<<player.name<<std::endl;//Debug
                    if(state==1||state==2){
                        if(active_field==1){//Sprawdza dane do logowania i przechodzi do gry / na razie przechodzi do gry
                            // button_animation(login_button,login_button_label,*window);//Tego pewnie nie widać
                            active_field = -1;
                            if(state==1) log(state,error_log,login_input,password_input,hide_password_input,login_input_text,password_input_text);
                            else if(state==2) sign_up(state,error_log,login_input,password_input,hide_password_input,login_input_text,password_input_text,active_field);
                        }
                        else if(active_field==0) active_field=1;//Przechodzi do następnego pola
                    }
                }
            }

            //Sprawdza kliknięcie guzików na starcie
            else if(state==0){
                for(int i=0;i<max_num_of_buttons;i++){
                    if(button_action(buttons[i],*event,*window)){
                        if(i==0){ state = 1; active_field = 0;}//Sign in
                        if(i==1) {state = 2; active_field = 0;}//Sign up
                        if(i==2) {state = 3;player.name = "Guest";}//Guess
                    }
                }
                //Animacja guzików
                for(int i=0;i<max_num_of_buttons;i++){
                    button_animation(buttons[i],buttons_labels[i],*window);
                }

            }
            else if(state==1||state==2){
                
                {
                if(state==1) login_button_label.setString("LOG IN");
                else if(state==2) login_button_label.setString("SIGNUP");
                }
                
                button_animation(login_button,login_button_label,*window);
                button_animation(back_button,back_button_label,*window);

                //Guzik logowania, potem sprawdzenie danych i ewentualne przejście do następnej strony
                if(button_action(login_button,*event,*window)){
                    // std::cout<<"LOG IN!!!   " << std::endl;//Debug
                    // std::cout<<active_field;//Debug
                    if (state==1) log(state,error_log,login_input,password_input,hide_password_input,login_input_text,password_input_text);              
                    else if (state==2) sign_up(state,error_log,login_input,password_input,hide_password_input,login_input_text,password_input_text,active_field);
                }
                //Guzik powrotu
                if(button_action(back_button,*event,*window)){
                    state = 0;//Powraca do menu startowego
                    active_field = -1;
                    //Kasuje dane logowania
                    login_input = "";
                    password_input = "";
                    hide_password_input = "";
                    login_input_text.setString("");
                    password_input_text.setString("");
                    error_log = 0;
                }
                {
                //Sprawdza w którym polu pisać
                if(button_action(text_frames[2],*event,*window)) active_field = 0;//Ustawia pole loginu na aktywne
                else if(button_action(text_frames[3],*event,*window)) active_field = 1;//Ustawia pole hasła na aktywne
                }
                //Wykrywa, wpisywanie teksu
                if(active_field!=-1){
                    switch (active_field){
                        case 0:
                            input_text(login_input,hide_password_input,*event,12,active_field);
                            login_input_text.setString(login_input);
                            break;
                        case 1:
                            input_text(password_input,hide_password_input,*event,15,active_field);                        
                            password_input_text.setString(hide_password_input);
                            break;
                    }
                }


            }
        }

        
        

        //Renderowanie
        window->clear(sf::Color(Piemontite_color));//Kolor okna, nie rozumiem do końca systemu zapisu kolorów

        switch (state){
        case 0://Start buttons
            for(int i=0;i<3;i++){window->draw(buttons[i]);}//wyświetla przyciski logowania
            for(int i=0;i<3;i++){window->draw(buttons_labels[i]);}//wyświetla etykiety przycisków logowania
            break;
        case 1:case 2://Sign in side
            for(int i=0;i<4;i++){window->draw(text_frames[i]);}//wyświetla przyciski logowania
            for(int i=0;i<2;i++){window->draw(labels_log[i]);}//Wyświetla opisy pól
            window->draw(login_button);
            window->draw(login_button_label);
            window->draw(back_button);
            window->draw(back_button_label);
            if(active_field!=-1){
                if(active_field==0){
                    // sf::Vector2f pozycja = login_input_text.findCharacterPos(login_input.length());
                    cursor.setPosition({
                        login_input_text.findCharacterPos(login_input.length()).x+2,
                        login_input_text.findCharacterPos(login_input.length()).y+16
                    });
                    if(frame_count>30) window->draw(cursor);
                }
                else if(active_field==1){
                    cursor.setPosition({
                        password_input_text.findCharacterPos(hide_password_input.length()).x+2,
                        password_input_text.findCharacterPos(hide_password_input.length()).y+12
                    });
                    if(frame_count>30) window->draw(cursor);
                }
                //Wyświetla wprowadzane dane do logowania
                window->draw(login_input_text);
                window->draw(password_input_text);
            }
            {
            if(error_log==-1) {//Wyświetla ostrzeżenie o nieporawnym logowaniu
                error_log_text.setString("Invalid username or password");
                window->draw(error_log_frame);
                window->draw(error_log_text);
            }
            else if(error_log==-2){
                error_log_text.setString("Username already taken");
                window->draw(error_log_frame);
                window->draw(error_log_text);
            }
            }

            break;            

            break;
        case 3: 
        case 4:
            window->draw(statistic_bg);
            name.setString(player.name);
            window->draw(name);

            break;
        }

        window->draw(border);//wyświetla ramkę

        window->display();
    }

    delete window;//Po zakończeniu programu, usuwa okno
    return 0;
}

#include <iostream>
#include "includes/seciurity.h"
#include "includes/player.h"
#include "includes/button.h"
#include "includes/window_look.h"
#include "includes/loging_page.h"
#include "includes/signup_page.cpp"
#include "includes/math_const.h"

#include <SFML/Graphics.hpp>

#define Basil_color 0x1A471BFF //Że w sęsie ciemny zielony
#define Piemontite_color  0x1A471BFF//0x964B67FF//Że w sęsie fioletowy      //Zamieniłem basil_color na ten bo różowy mi się przestął chyba podobać
#define Apatite_color 0xF0FC97FF //Że w sęsie żółty
#define Olive_color 0x92A22EFF //Że w sęsie jasny zielony
#define Dark_color 0x161616FF
#define Red_color 0xD7443EFF //kolor do ostrzeżeń

#define max_num_of_buttons 3

Player player;

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
    sf::Font math_font("fonts/arial/arial.ttf");//czcionka odpowiadająca za symbole matematyczne

    //Naprawia rozpikselizowany tekst
    const_cast<sf::Texture&>(font.getTexture((int)(0.5f*0.037f*height+3))).setSmooth(false);//Lepsza jakość tekstu
    const_cast<sf::Texture&>(font.getTexture(0.035f*height)).setSmooth(false);//Lepsza jakość tekstu
    const_cast<sf::Texture&>(font.getTexture((int)(1.5*(0.5f*0.037f*height+3)))).setSmooth(false);//Lepsza jakość tekstu
    const_cast<sf::Texture&>(font.getTexture((int)(4*1.5*(0.5f*0.037f*height+3)))).setSmooth(false);//Lepsza jakość tekstu
    
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

    //Tablica przechowująca kształty przy statystykach
    sf::RectangleShape statistic_player_bg[4];
    build_stat_bg(statistic_player_bg,width,height,Apatite_color,Dark_color);

    //Tablica przechowująca napisy ze statystykami gracza
    sf::Text statistic_player_text[9]{sf::Text(font),sf::Text(font),sf::Text(font),sf::Text(font),sf::Text(font),sf::Text(font),sf::Text(font),sf::Text(font),sf::Text(math_font)};
    build_player_stat(statistic_player_text,font,math_font,width,height,Dark_color);





    //Pętla gry, wykonuje się do puki do puty okno jest otwarte
    while(window->isOpen()){
        {
        if(frame_count> 60) frame_count = 0;
        else frame_count++;
        }

        //Czyta zdażenia, takie jak przyciski,ruchy myszką itd., otem wykonuje odpowiednie akcje
        //Pętla zdażeń
        while (const std::optional<sf::Event> event = window->pollEvent()){

            //Jak wcisnąć krzyżyk to sie zamyka
            if(event->is<sf::Event::Closed>()){
                window->close();//Bez tego krzyżyk nie działa
            }

            //Sprawdza klawisze z klawiatury
            else if(const auto* keyPress = event->getIf<sf::Event::KeyPressed>()){
                //Ustawia klawisz esc jako wyjście
                if(keyPress->scancode == sf::Keyboard::Scancode::Escape){

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
                    if(state==1||state==2){
                        if(active_field==1){//Sprawdza dane do logowania i przechodzi do gry / na razie przechodzi do gry
                            // button_animation(login_button,login_button_label,*window);//Tego pewnie nie widać
                            active_field = -1;
                            if(state==1) log(state,error_log,login_input,password_input,hide_password_input,login_input_text,password_input_text,statistic_player_text);
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
                        if(i==1){state = 2; active_field = 0;}//Sign up
                        if(i==2){state = 3;update_player_stat(statistic_player_text);}//Guess
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
                    if (state==1) log(state,error_log,login_input,password_input,hide_password_input,login_input_text,password_input_text,statistic_player_text);              
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
            for(int i=0;i<4;i++){
                window->draw(statistic_player_bg[i]);
            }
            for(int i=0;i<9;i++){
                window->draw(statistic_player_text[i]);
            }
            break;
        }

            window->draw(border);//wyświetla ramkę

        window->display();
    }

    save_player();//Zapisuje dane gracza
    delete window;//Po zakończeniu programu, usuwa okno
    return 0;
}
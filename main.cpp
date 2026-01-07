#include <iostream>
#include "includes/button.h"
#include "includes/window_look.h"
#include "includes/user.h"

#include <SFML/Graphics.hpp>

#define Basil_color 0x1A471BFF //Że w sęsie ciemny zielony
#define Piemontite_color  0x964B67FF//Że w sęsie fioletowy
#define Apatite_color 0xF0FC97FF //Że w sęsie żółty
#define Olive_color 0x92A22EFF //Że w sęsie jasny zielony
#define Dark_color 0x161616FF

#define max_num_of_buttons 3


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
    */
   int active_field = -1;//Zmienna określająca które pole ma przyjmować tekst, tam ustawia kursor
   unsigned int frame_count=0;//Zlicza klatki, przyda się do kursora


    //Torzy okno
    sf::RenderWindow * window = new sf::RenderWindow(sf::VideoMode({width,height}),"Irrationa Snake");

    window->setFramerateLimit(60);//Ustawia klatkarz

    //Ustawia ramkę wokoło ekranu
    sf::RectangleShape border({(float)width,(float)height});
    border = build_border(width,height,Dark_color);

    sf::Font font("fonts/BebasNeue-Regular.ttf");//ładuje czcionkę

    //Naprawia rozpikselizowany tekst
    const_cast<sf::Texture&>(font.getTexture((int)(0.5f*0.037f*height+3))).setSmooth(false);//Lepsza jakość tekstu
    const_cast<sf::Texture&>(font.getTexture(0.035f*height)).setSmooth(false);//Lepsza jakość tekstu

    //Tworzy przyciski na ekranie startowym
    sf::RectangleShape buttons[max_num_of_buttons];//Tablica przechowująca przyciski
    sf::Text buttons_labels[max_num_of_buttons]={sf::Text(font),sf::Text(font),sf::Text(font)};//Tablica przechowująca tekst

    build_start_bttons(width,height,Apatite_color,Dark_color,buttons,buttons_labels,font);

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




    //Pętla gry, wykonuje się do puki do puty okno jest otwarte
    while(window->isOpen()){
        {
        if(frame_count> 60) frame_count = 0;
        else frame_count++;
        }

        //Czyta zdażenia, takie jak przyciski,ruchy myszką itd., otem wykonuje odpowiednie akcje
        //Pętla zdażeń
        while (const std::optional<sf::Event> event = window->pollEvent()){

            //Jak wcisnąć krzyzyk to sie zamyka
            if(event->is<sf::Event::Closed>()){
                window->close();//Bez tego krzyżyk nie działa
            }

            //Ustawia klawisz esc jako wyjście
            else if(const auto* keyPress = event->getIf<sf::Event::KeyPressed>()){

                if(keyPress->scancode == sf::Keyboard::Scancode::Escape){

                    window->close();
                }
            }
            //Sprawdza kliknięcie guzików na starcie
            else if(state==0){
                for(int i=0;i<max_num_of_buttons;i++){
                    if(button_action(buttons[i],*event,*window)){
                        if(i==0) state = 1;//Sign in
                        if(i==1) state = 2;//Sign up
                        if(i==2) state = 3;//Guess
                    }
                }
            }
            else if(state==1){
                if(button_action(login_button,*event,*window)){
                    std::cout<<"Wypierdalaj!" << std::endl;
                    std::cout<<active_field;
                }
                if(button_action(back_button,*event,*window)){
                    state = 0;//Powraca do menu startowego
                    active_field = -1;
                }
                {
                if(button_action(text_frames[2],*event,*window)) active_field = 0;//Ustawia pole loginu na aktywne
                else if(button_action(text_frames[3],*event,*window)) active_field = 1;//Ustawia pole hasła na aktywne
                }

            }
        }
        //Animacja guzików
        if(state == 0){
            for(int i=0;i<max_num_of_buttons;i++){
                button_animation(buttons[i],buttons_labels[i],*window);
            }
        }
        else if(state==1){
            button_animation(login_button,login_button_label,*window);
            button_animation(back_button,back_button_label,*window);
        }
        

        //Renderowanie
        window->clear(sf::Color(Piemontite_color));//Kolor okna, nie rozumiem do końca systemu zapisu kolorów

        switch (state){
        case 0://Start buttons
            for(int i=0;i<3;i++){window->draw(buttons[i]);}//wyświetla przyciski logowania
            for(int i=0;i<3;i++){window->draw(buttons_labels[i]);}//wyświetla etykiety przycisków logowania
            break;
        case 1://Sign in side
            for(int i=0;i<4;i++){window->draw(text_frames[i]);}//wyświetla przyciski logowania
            for(int i=0;i<2;i++){window->draw(labels_log[i]);}//Wyświetla opisy pól
            window->draw(login_button);
            window->draw(login_button_label);
            window->draw(back_button);
            window->draw(back_button_label);
            if(active_field!=-1){
                if(active_field==0){
                    cursor.setPosition({width/2.0f-80,height/2.0f-0.06f*height});
                    if(frame_count>30) window->draw(cursor);
                }
                if(active_field==1){
                    cursor.setPosition({width/2.0f-80,height/2.0f+0.06f*height});
                    if(frame_count>30) window->draw(cursor);
                }

            }

            break;
        case 2:

            break;
        case 3: case 4:

            break;
        }

            window->draw(border);//wyświetla ramkę

        window->display();
    }

    delete window;//Po zakończeniu programu, usuwa okno
    return 0;
}
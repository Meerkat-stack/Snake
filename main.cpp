#include <iostream>
#include "includes/button.h"
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

    //Torzy okno
    sf::RenderWindow * window = new sf::RenderWindow(sf::VideoMode({width,height}),"Irrationa Snake");

    //Tworzy ramkę wokoło okna
    sf::RectangleShape border({(float)width,(float)height});
    border.setFillColor(sf::Color::Transparent);//Wnętrze przezroczyste
    border.setOutlineColor(sf::Color(Dark_color));
    border.setOutlineThickness(-(float)width*0.009);//Grubość ramki//-(float)width*0.008

    window->setFramerateLimit(60);//Ustawia klatkarz

    sf::Font font("fonts/BebasNeue-Regular.ttf");//ładuje czcionkę

    //Tworzy przyciski na ekranie startowym
    sf::RectangleShape buttons[max_num_of_buttons];//Tablica przechowująca przyciski
    buttons[0] = build_button(0.12f*width,0.05f*height,width/2.0f,height/2.0f-0.09f*height,Apatite_color,Dark_color);//sign in
    buttons[1] = build_button(0.12f*width,0.05f*height,width/2.0f,height/2.0f,Apatite_color,Dark_color);//sign up
    buttons[2] = build_button(0.12f*width,0.05f*height,width/2.0f,height/2.0f+0.09f*height,Apatite_color,Dark_color);//guest

    //Dodaje etykiety na przyciskach        
    sf::Text buttons_labels[max_num_of_buttons]={sf::Text(font),sf::Text(font),sf::Text(font)};//Tablica przechowująca przyciski
    buttons_labels[0] = label(0.035f*height,width/2.0f,height/2.0f-0.09f*height,"Sign in",Dark_color,font);
    buttons_labels[1] = label(0.035f*height,width/2.0f,height/2.0f,"Sign up",Dark_color,font);
    buttons_labels[2] = label(0.035f*height,width/2.0f,height/2.0f+0.09f*height,"Guest",Dark_color,font);
    



    //Pętla gry, wykonuje się do puki do puty okno jest otwarte
    while(window->isOpen()){
        
        //Czyta zdażenia, takie jak przyciski,ruchy myszką itd., otem wykonuje odpowiednie akcje
        while(const std::optional event = window->pollEvent()){

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
            //Sprawdza kliknięcie guzików
            for(int i=0;i<max_num_of_buttons;i++){
                if(button_action(buttons[i],*event,*window)){
                    if(i==0) std::cout<<"Sign in"<<std::endl;
                    if(i==1) std::cout<<"Sign up"<<std::endl;
                    if(i==2) std::cout<<"Guest"<<std::endl;
                }
            }
            
        }
        //Animacja guzików
        for(int i=0;i<max_num_of_buttons;i++){
            button_animation(buttons[i],buttons_labels[i],*window);
        }


        //Renderowanie
        window->clear(sf::Color(Piemontite_color));//Kolor okna, nie rozumiem do końca systemu zapisu kolorów

        for(int i=0;i<max_num_of_buttons;i++){window->draw(buttons[i]);}//wyświetla przyciski logowania
        for(int i=0;i<max_num_of_buttons;i++){window->draw(buttons_labels[i]);}//wyświetla etykiety przycisków logowania
        window->draw(border);

        window->display();
    }

    delete window;//Po zakończeniu programu, usuwa okno
    return 0;
}
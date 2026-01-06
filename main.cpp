#include <iostream>

#include <SFML/Graphics.hpp>


#define Basil_color 0x1A471BFF //Że w sęsie ciemny zielony
#define Piemontite_color  0x964B67FF//Że w sęsie fioletowy
#define Apatite_color 0xF0FC97FF //Że w sęsie żółty
#define Olive_color 0x92A22EFF //Że w sęsie jasny zielony
#define Dark_color 0x161616FF



int main()
{
    //Rozmiary okna
    unsigned int width = 740;
    unsigned int height = 740;

    //Tworzy okno, ustala rozmiar, tytuł okna
    //Potem można jeszcze ustawić styl okna, czyli czy np jest krzyżyk czy nie jak go zamknąć i czy da się zmieniać jego rozmiar
    sf::RenderWindow * window = new sf::RenderWindow(sf::VideoMode({width,height}),"Tutorial");
    window->setFramerateLimit(60);//Ustawia klatkarz

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
            
        }

        //Renderowanie
        window->clear(sf::Color(Piemontite_color));//Kolor okna, nie rozumiem do końca systemu zapisu kolorów

        window->display();
    }

    delete window;//Po zakończeniu programu, usuwa okno
    return 0;
}
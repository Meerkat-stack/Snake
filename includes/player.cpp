#include <iostream>
#include <fstream>
#include "seciurity.h"

// Struktura gracza
struct Player {
	std::string name = "Unknown";

	// Statystyki podstawowe
	float total_pixels_traveled = 0.0f;
	int total_play_time_sec = 0;
	int snake_max_length = 0;
	int snake_min_length = 0;

	// Statystyki rozszerzone (Przyczyny zgonów)
	int deaths_wall = 0;
	int deaths_self = 0;

	// Postępy
	int unlocked_maps_count = 1;
	int unlocked_skins_count = 1;
};

extern Player player;

void save_player(){
    if (player.name == "Guest" || player.name == "Unknown") return;//Dla gości albo niezalogowanych nie zapisuje nic
    else{
        std::ifstream original("userdata/users_data.txt");//Otwiera plik do odczytu
        std::ofstream temp("userdata/users_data_temp.txt");//Otwiera plik do zapisu

        std::string s_name, s_dist, s_time, s_maxL, s_minL, s_wall, s_self, s_maps, s_skins;

        std::string tmp_name = player.name;
        encrypt(tmp_name,16);

        while(original>>s_name>>s_dist>>s_time>>s_maxL>>s_minL>>s_wall>>s_self>>s_maps>>s_skins) {

            // decrypt(s_name,16);//Deszyfruje odczytaną nazwę gracza

            if(s_name==tmp_name){
                //Modyfikuje dane gracza przed zapisaniem        
                // std::cout<<"    |"<<s_name<<std::endl;//Debug
                // std::cout<<player.name<<std::endl;//Debug

                // s_name = player.name;
                // encrypt(s_name,16);//Szyfruje przed zapisem
                s_dist = std::to_string(player.total_pixels_traveled);//Zamienia int na string
                s_time = std::to_string(player.total_play_time_sec);
                s_maxL = std::to_string(player.snake_max_length);
                s_minL = std::to_string(player.snake_min_length);
                s_wall = std::to_string(player.deaths_wall);
                s_self = std::to_string(player.deaths_self);
                s_maps = std::to_string(player.unlocked_maps_count);
                s_skins = std::to_string(player.unlocked_skins_count);
            }

            temp<<s_name<<" "<<s_dist<<" "<<s_time<<" "<<s_maxL<<" "<<s_minL<<" "<<s_wall<<" "<<s_self<<" "<<s_maps<<" "<<s_skins<<"\n";
        }
        original.close();
        temp.close();

        //Usuwa stary plik, 0 oznacza powodzenie 
        if (std::remove("userdata/users_data.txt") != 0) {
            // std::cout<<"Old file canceled\n";//Debug
        }

        //Zmienia nazwę nowego pliku
        if (std::rename("userdata/users_data_temp.txt", "userdata/users_data.txt") != 0) {
            // std::cout<<"Error: zmiana nazwy nie powiodła się\n"//Debug
        }

    }
}

//Odczytuje dane gracza
void read_player() {
    std::ifstream file("userdata/users_data.txt");

    if (!file.is_open()) {
        std::cerr << "Błąd: Nie można otworzyć pliku users_data.txt do odczytu!\n";
        return;
    }

    std::string s_name, s_dist, s_time, s_maxL, s_minL, s_wall, s_self, s_maps, s_skins;

    std::string target_cipher = player.name;
    encrypt(target_cipher, 16); 

    while (file >> s_name >> s_dist >> s_time >> s_maxL >> s_minL >> s_wall >> s_self >> s_maps >> s_skins) {
        
        // Jeśli zaszyfrowane imię z pliku pasuje do naszego celu
        if (s_name == target_cipher) {
            // 4. Konwertujemy stringi z pliku na liczby i zapisujemy w strukturze player
            try {
                player.total_pixels_traveled = std::stof(s_dist);
                player.total_play_time_sec = std::stoi(s_time);
                player.snake_max_length = std::stoi(s_maxL);
                player.snake_min_length = std::stoi(s_minL);
                player.deaths_wall = std::stoi(s_wall);
                player.deaths_self = std::stoi(s_self);
                player.unlocked_maps_count = std::stoi(s_maps);
                player.unlocked_skins_count = std::stoi(s_skins);
            } catch (...) {
                std::cerr << "Bład konwersji danych dla gracza: " << player.name << "\n";
            }
            break; // Znaleźliśmy właściwego gracza, wychodzimy z pętli
        }
    }

    file.close();
}
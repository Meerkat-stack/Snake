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

        while(original>>s_name>>s_dist>>s_time>>s_maxL>>s_minL>>s_wall>>s_self>>s_maps>>s_skins) {
            
            // decrypt(s_name,16);//Deszyfruje odczytaną nazwę gracza

            if(s_name==player.name){
                //Modyfikuje dane gracza przed zapisaniem        
                std::cout<<"    |"<<s_name<<std::endl;//Debug
                std::cout<<player.name<<std::endl;//Debug

                s_name = player.name;
                encrypt(s_name,16);//Szyfruje przed zapisem
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
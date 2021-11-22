#include <iostream>
#include <ctime>
#include <string>
#include <vector>
#include <fstream>
#include <iomanip>

#define QUESTION "Input your request: play, pause, next, stop, exit."

class Track{
    std::string name;
    std::time_t date = std::time(nullptr);
    int duration;
public:
    bool playing = false;
    bool paused = false;
public:
    void setName(std::string someName){
        name = someName;
    }
    std::string getName(){
        return name;
    }
    void setDate(std::time_t someData){

       date = someData;
    }
    std::string getDate(){
        std::stringstream buf;
        std::tm local = *std::localtime(&date);
        buf << std::put_time(&local, "%Y/%m/%d");
        return buf.str();
    }
    void setDuration(int someDuration){
        duration = someDuration;
    }
    int getDuration(){
        return duration;
    }

private:


};

class Player{
    std::vector<Track> playlist;
public:
    void readFromFile(std::string filePath){
        std::ifstream pl(filePath);
        if(pl){
            while (!pl.eof()){
                std::string bufName;

                int bufDuration;
                pl >> bufName;
               // std::cout<<bufName;

                std::time_t bufDate = std::time(nullptr);
                std::tm local = *std::localtime(&bufDate);
                pl >> std::get_time(&local, "%Y/%m/%d");
                bufDate = mktime(&local);

                pl >> bufDuration;
                //std::cout<<bufDuration;


                auto* bufTrack = new Track;
                bufTrack->setName(bufName);
                bufTrack->setDate(bufDate);
                bufTrack->setDuration(bufDuration);
                playlist.push_back(*bufTrack);
                delete bufTrack;


            }
        } else {
            std::cerr << "File is not found!" << std::endl;
        }
        pl.close();

    }

    void showNames(){
        std::cout << "Available tracks:" << std::endl;
        for(int i = 0; i<playlist.size();i++) {
            std::cout << playlist[i].getName() << std::endl;
        }
    }
    void showPlaylist(){
        for(int i = 0; i<playlist.size();i++) {
            std::cout  << playlist[i].getName() << " " << playlist[i].getDate() << " "
                      << playlist[i].getDuration() <<" " << std::boolalpha << playlist[i].playing
                      << std::endl;
        }
    }
    void play(){
        std::string bufName;
        std::cout << "Input name of track you want to listen!" << std::endl;
        std::cin >> bufName;
        playTrack(bufName);

    }
    void pause(){
        int i = 0;
        bool flag = false;
        for(;i < playlist.size(); i++){
            if(playlist[i].playing){
                flag = true;
                break;
            }
        }
        if(flag && !playlist[i].paused){
            playlist[i].paused = true;
            std::cout << playlist[i].getName() << " paused" <<std::endl;
        } else if(flag){
            std::cout << "Track is already paused!" << std::endl;
        } else {
            std::cout << "No playing track found!" << std::endl;
        }

    }
    void stop(){
        int i = 0;
        bool flag = false;
        for(;i < playlist.size(); i++){
            if(playlist[i].playing){
                flag = true;
                break;
            }
        }
        if(flag){
            playlist[i].playing = false;
            std::cout << playlist[i].getName() << " is stopped!" <<std::endl;
        } else {
            std::cout << "No playing track found!" << std::endl;
        }

    }
    void next(){

        std::srand(std::time(nullptr));
        std::vector<int> available;
        for(int i = 0; i < playlist.size(); i++){
            if(!playlist[i].playing) available.push_back(i);
        }
        int recInd = rand() % available.size();
        playTrack(playlist[available[recInd]].getName());

    };

private:
    bool playTrack(std::string nameFromUser){
        int i = 0;
        int k = 0;
        bool flag = false;
        for(;i < playlist.size(); i++){
            if(playlist[i].getName() == nameFromUser){
                flag = true;
                break;
            }
        }


        if(flag) {
            for (; (k < playlist.size()) && (!playlist[k].playing); k++);
            if (!playlist[i].playing) {
                playlist[i].playing = true;
                playlist[k].playing = false;
                std::cout << "Playing... " << playlist[i].getName() << ", Date: " << playlist[i].getDate() << ", duration:"
                          << playlist[i].getDuration()
                          << std::endl;
            } else if(playlist[i].paused) {
                playlist[i].paused = false;
                std::cout << "Track is playing again!" << std::endl;
            } else {
                std::cout <<"Track is already playing!" << std::endl;
            }
        } else {
            std::cout << "Track not found!" << std::endl;
        }
        return flag;
    }

};

int main() {
    std::string request;
    Player* player = new Player;
    player->readFromFile("Playlist.txt");

    player->showNames();
    std::cout << QUESTION << std::endl;
    std::cin >> request;
    while (request!="exit"){
        if(request == "play"){
            player->showNames();
            player->play();
        } else if(request == "pause"){
            player->pause();
        } else if(request == "stop"){
            player->stop();
        } else if(request == "next"){
            player->next();
        }
        std::cout << QUESTION << std::endl;
        std::cin >> request;
    }
    delete player;
    return 0;
}

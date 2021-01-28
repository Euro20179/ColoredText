#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <string>
#include <cstdlib>
#include <sstream>
#include <vector>
#include "Window.h"
#include "Text.h"
#include "propertystructs.h"

bool run = true;

SDL_Color getRandomColor(){
    unsigned char r = rand() % 255;
    unsigned char g = rand() % 255;
    unsigned char b = rand() % 255;
    return {r, g, b, 255};
}

//splits a string by a delimiter
//split("string string string", ' ') results in std::vector<std::string>{"string", "string", "string"}
std::vector<std::string> split(const std::string& s, char delim){
    std::vector<std::string> res;
    std::stringstream ss (s);
    std::string item;

    while(std::getline(ss, item, delim)){
        res.push_back(item);
    }
    return res;
}

int main(int argc, const char* argv[]){
    TTF_Init();

    std::string fontPath = "/usr/share/fonts/truetype/ubuntu/UbuntuMono-R.ttf";
    int fontSize = 24;
    SDL_Color fontColor = {255, 0, 0, 255};
    bool rainbow = true;
    const char* title = "Text"; //title of the window
    SDL_Color backgroundColor = {0, 0, 0, 255};

    if(argc == 1){
        std::cout << "\033[1;31mError:\033[0;37m Not enough arguments\n";
        return 1;
    }    

    //variable for text for the message
    const char* m = argv[1];

    if(std::string(m) == "--help"){
        std::cout << "Usage: text <text> [OPTIONS...]\n -fsize: size of the font\n -f: font path to use (default: /usr/share/fonts/truetype/ubuntu/UbuntuMono-R.ttf\n -color: the color of the text (example: \"255 0 0\")\n -title: title of the  window\n -bg: background color (example: \"255 0 0\")\n --rainbowoff: makes it not rainbow text" << std::endl;
        return 0;
    }

    //parses the arguments
    for(int i = 2; i < argc; i++){
        std::string arg = argv[i];

        if(arg == "-fsize" || arg == "-fontsize"){
            //converts to int
            fontSize = std::atoi(argv[i + 1]);

            //warns if font size is large
            if(fontSize >= 100){
                std::cout << "\033[1;33mWarning:\033[0;37m Large font sizes can hog up a lot of memory and can cause system crashes\nContinue?(y/n): ";
                
                char i = '\0';
                //continue?
                std::cin.get(i);
                //0x4E is N, 0x6E is n
                if(i == 0x4E || i == 0x6E){
                    std::cout << "Stopped" << std::endl;
                    return 0;
                }
            }

            i++; //this is so that it skips the value provided

            continue;
        }
        else if(arg == "-title"){
            title = argv[i + 1];

            i++; //same as above

            continue;
        }
        else if(arg == "-f" || arg == "-font"){
            fontPath = argv[i + 1];

            i++; //same as above

            continue;
        }
        else if(arg == "-color"){
            std::string c = argv[i + 1];
            //gets list of r,g,b values
            std::vector<std::string> v = split(c, ' ');

            uint8_t r, g, b;
            //converts list of r,g,b values to ints
            r = std::atoi(v[0].c_str());
            g = std::atoi(v[1].c_str());
            b = std::atoi(v[2].c_str());

            fontColor = {r, g, b, 255};

            rainbow = false; // setting the color and leaving the rainbow on seems weird as the color will get updated IMMEDIATELY

            i++;

            continue;
        }
        else if(arg == "-bg"){
            std::string c = argv[i + 1];
            std::vector<std::string> v = split(c, ' ');

            uint8_t r, g, b;
            r = std::atoi(v[0].c_str());
            g = std::atoi(v[1].c_str());
            b = std::atoi(v[2].c_str());

            backgroundColor = {r, g, b, 255};

            i++;

            continue;
        }
        else if(arg == "--rainbowoff"){
            rainbow = false;

            i++;

            continue;
        }
    }

    //wrapper around SDL_Renderer and SDL_Window
    Window window = Window(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 10, 10, 0); //0 being NULL (gives warning for some reason even though NULL is literally #define 0)    
       
    Text message = Text(window.getRenderer(), m, fontPath.c_str(), fontSize, fontColor, 0, 0); //wrapper around creating a surface and texture for rendering text 
    SDL_Rect messageRect = message.getRect();

    SDL_SetWindowSize(window.getWindow(), messageRect.w, messageRect.h); //sets the window size to the size of the text

    //makes window resizeable
    SDL_SetWindowResizable(window.getWindow(), SDL_TRUE); //why does SDL have their own version of booleans like seriously just use true and false

    SDL_Event e;


    SDL_Window* win = window.getWindow(); //so getWindow() isn't called every frame
    SDL_Renderer* renderer = window.getRenderer(); // so getRenderer() isn't called every frame

    int winW, winH; //these are declared here because there no point to do so every frame

    while(run){
        SDL_PollEvent(&e);
        switch(e.type){
            case SDL_QUIT:
                SDL_Quit();
                run = false;
            case SDL_WINDOWEVENT: //SDL_WINDOWEVENT_RESIZE doesn't work
                SDL_GetWindowSize(win, &winW, &winH);
                message.setPos((winW / 2) - (messageRect.w / 2), (winH / 2) - (messageRect.h / 2));
        }

        window.clear(backgroundColor.r, backgroundColor.g, backgroundColor.b, backgroundColor.a);

        if(rainbow)
            message.setColor(renderer, getRandomColor());
            
        message.render(renderer);

        window.present();
    }
    return 0;
}
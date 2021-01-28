#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL.h>
#include "propertystructs.h"

class Text{
public:
    Text(SDL_Renderer* renderer, const char* message, const char* fontPath, int fontSize, SDL_Color color, int x, int y){
        _font = TTF_OpenFont(fontPath, fontSize);
        _messageSurface = TTF_RenderText_Solid(_font, message, color);
        _messageTexture = SDL_CreateTextureFromSurface(renderer, _messageSurface);
        _r.x = x;
        _r.y = y;
        _r.w = _messageSurface->w;
        _r.h = _messageSurface->h;
        _message = message;
    }
    ~Text(){
        SDL_FreeSurface(_messageSurface);
        SDL_DestroyTexture(_messageTexture);
    }
    void render(SDL_Renderer* renderer){
        SDL_RenderCopy(renderer, _messageTexture, NULL, &_r);
    }
    void setPos(int x, int y){
        _r.x = x;
        _r.y = y;
    }
    void setColor(SDL_Renderer* renderer, SDL_Color color){
        _messageSurface = TTF_RenderText_Solid(_font, _message, color);
        _messageTexture = SDL_CreateTextureFromSurface(renderer, _messageSurface);
        _color = color;
    }
    Dimention getDimentions(){
        return {_messageSurface->w, _messageSurface->h};
    }
    SDL_Rect& getRect(){
        return _r;
    }
private:
    TTF_Font* _font = nullptr;
    SDL_Surface* _messageSurface = nullptr;
    SDL_Texture* _messageTexture = nullptr;
    SDL_Color _color;
    SDL_Rect _r;
    const char* _message;
};
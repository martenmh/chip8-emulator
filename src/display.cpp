////
//// Created by marten on 17-12-19.
////
//#include <iostream>
//#include "display.h"
//
//Display::Display(std::string title, int x, int y, int w, int h, uint flags){
//    window = SDL_CreateWindow(title.c_str(), x, y, w, h, flags);
//
//    if(window == NULL)
//        std::cerr << "An error occurred while creating the window" << std::endl;
//}
//
//Display::Display(){
//
//}
//
//Display::Display(Display *pDisplay){
//    window = pDisplay->window;
//}
//
//Display::~Display(){
//    SDL_DestroyWindow(window);
//}
//
// Created by Xiaoyu Chen on 2025/4/12.
//

#ifndef WELCOMEWINDOW_H
#define WELCOMEWINDOW_H

//hfile
 #pragma once
#include <string>


class WelcomeWindow {
public:
    WelcomeWindow(int width, int height);
    std::string run();

private:
    int width;
    int height;
};


#endif //WELCOMEWINDOW_H


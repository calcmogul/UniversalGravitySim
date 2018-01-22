// Copyright (c) 2016-2018 Tyler Veness. All Rights Reserved.

#include "UIFont.hpp"

sf::Font& UIFont::segoeUI() {
    static sf::Font font = [] {
        sf::Font font;
        font.loadFromFile("Resources/segoeui.ttf");
        return font;
    }();
    return font;
}

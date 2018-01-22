// Copyright (c) 2016-2018 Tyler Veness. All Rights Reserved.

#include "UIFont.hpp"

sf::Font UIFont::m_segoeUI;

UIFont& UIFont::getInstance() {
    static UIFont instance;
    return instance;
}

sf::Font& UIFont::segoeUI() { return m_segoeUI; }

UIFont::UIFont() { m_segoeUI.loadFromFile("Resources/segoeui.ttf"); }

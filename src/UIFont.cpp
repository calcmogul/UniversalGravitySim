// Copyright (c) 2016-2018 Tyler Veness. All Rights Reserved.

#include "UIFont.hpp"

UIFont* UIFont::m_instance = NULL;
sf::Font UIFont::m_segoeUI;

UIFont* UIFont::getInstance() {
    if (m_instance == NULL) {
        m_instance = new UIFont;
    }

    return m_instance;
}

UIFont::UIFont() { m_segoeUI.loadFromFile("Resources/segoeui.ttf"); }

sf::Font& UIFont::segoeUI() { return m_segoeUI; }

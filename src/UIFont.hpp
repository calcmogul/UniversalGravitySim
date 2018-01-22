// Copyright (c) 2016-2018 Tyler Veness. All Rights Reserved.

#ifndef UNIVERSALGRAVITYSIM_SRC_UIFONT_HPP_
#define UNIVERSALGRAVITYSIM_SRC_UIFONT_HPP_

#include <SFML/Graphics/Font.hpp>

class UIFont {
public:
    UIFont(const UIFont&) = delete;
    UIFont& operator=(const UIFont&) = delete;

    static sf::Font& segoeUI();
};

#endif  // UNIVERSALGRAVITYSIM_SRC_UIFONT_HPP_

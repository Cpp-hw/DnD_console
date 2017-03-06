#pragma once

#include <SFML/Graphics.hpp>
#include <TGUI/TGUI.hpp>
#include "CharacterForm.hpp"


class PlayerForm
{

    enum _selected_menu
    {
        NONE,
        Character_MENU,
        BOARD_MENU,
        BACK,
    };

    sf::Event _event;
    tgui::Gui _gui;

    HttpClient *_http_client;

    //UI elements
    tgui::Theme::Ptr    _theme;
    tgui::Picture::Ptr  _main;
    tgui::Picture::Ptr  _menu;
    tgui::Picture::Ptr  _logo;
    tgui::Button::Ptr   _btn_character;
    tgui::Button::Ptr   _btn_board;
    tgui::Button::Ptr   _btn_back;
    std::string         _game_session;

    _selected_menu _menu_option;

    bool _character_button_click;
    bool _board_button_click;
    bool _back_button_click;

    CharacterForm *_character_window;

    void fClickedBoard();
    void fClickedCharacter();
    void fDisable();
    void fInitUIElements();

public:
    bool display_window;
    PlayerForm(const sf::Event &event, sf::RenderWindow &window, std::string game_session, HttpClient *http_client);

    void fUpdate(sf::RenderWindow  &window);
    void fDraw(sf::RenderWindow & window);
};
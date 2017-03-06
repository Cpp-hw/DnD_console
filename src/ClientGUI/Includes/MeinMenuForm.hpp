#pragma once

#include "DmForm.hpp"
#include "PlayerForm.hpp"


class MeinMenuForm
{
    enum _selected_menu
    {
        NONE,
        PLAYER_MODE,
        DM_MODE,
        EXIT,
    };

    sf::Event _event;
    tgui::Gui _gui;
    
    HttpClient *_http_client;

    DMForm *_dm_window;
    PlayerForm *_player_window;

    // UI elements
    tgui::Theme::Ptr    _theme;
    tgui::Picture::Ptr  _main;
    tgui::Picture::Ptr  _menu;
    tgui::Picture::Ptr  _logo;
    tgui::Button::Ptr   _btn_player;
    tgui::Button::Ptr   _btn_dm;
    tgui::Button::Ptr   _btn_exit;
    
    std::string _game_session;
    
    _selected_menu _menu_option;

    bool _exit_m_button_click;

    void fInitUIElements();
    void fExit_clicked();

public:
    bool display_window;

    MeinMenuForm(const sf::Event &event, sf::RenderWindow &window, std::string game_session, HttpClient *http_client);
    void fUpdate(sf::RenderWindow  &window);
    void fDraw(sf::RenderWindow & window);
};


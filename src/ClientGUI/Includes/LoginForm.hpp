#pragma once

#include "MeinMenuForm.hpp"
#include "RegisterForm.hpp"


class LoginForm
{
    enum _selected_menu
    {
        NONE,
        LOGIN,
        MAIN_MENU,
        REGISTER,
    };

    sf::Event _event;
    tgui::Gui _gui;
    HttpClient *_http_client;
    std::string _game_session;
    // UI elements
    tgui::Theme::Ptr   _theme;
    tgui::EditBox::Ptr _username;
    tgui::EditBox::Ptr _password;
    tgui::Button::Ptr  _login_button;
    tgui::Button::Ptr  _register_button;
    tgui::ListBox::Ptr _terrain_list;

    // avaliable menu options 
    RegisterForm *_registration_window;
    MeinMenuForm *_menu_window;

    _selected_menu _menu_option;

    // flags flags to check the button is pressed 
    bool _main_menu_button_click;
    bool _reg_button_click;

    // methods for changing the state of the button
    void fButtonRegisterClicked();

    void fInitUIElements();
    void fLogin(tgui::EditBox::Ptr username, tgui::EditBox::Ptr password);

public:
    LoginForm(const sf::Event & event, sf::RenderWindow &window, HttpClient *http_client);
    void fUpdate(sf::RenderWindow &window);
    void fDraw(sf::RenderWindow & window);
};
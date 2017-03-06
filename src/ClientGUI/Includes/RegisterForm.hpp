#pragma once

#include <SFML/Graphics.hpp>
#include <TGUI/TGUI.hpp>
#include "UserActions.hpp"
#include "HttpClient.hpp"
#include <thread>



class RegisterForm
{
private:
    sf::Event _event;
    tgui::Gui _gui;
    HttpClient *_http_client;


    //UI elems
    tgui::Theme::Ptr   _theme;
    tgui::EditBox::Ptr _editBoxUsername;
    tgui::EditBox::Ptr _editBoxEmail;
    tgui::EditBox::Ptr _editBoxPassword;
    tgui::EditBox::Ptr _editBoxPasswordRe;
    tgui::Button::Ptr  _register_button;
    tgui::Button::Ptr  _back;
    tgui::Button::Ptr  test;
    tgui::ChildWindow::Ptr information_window;
    tgui::Picture::Ptr _picture;


    void fRegistration(tgui::EditBox::Ptr username, tgui::EditBox::Ptr email, tgui::EditBox::Ptr password, tgui::EditBox::Ptr re_password);

    void fBack_Click();

    void fInitUIElements();

public:

    bool display_window;

    RegisterForm(const sf::Event &event, sf::RenderWindow &window, HttpClient *http_client);
    void fUpdate(sf::RenderWindow  &window);
    void fDraw(sf::RenderWindow & window);
};
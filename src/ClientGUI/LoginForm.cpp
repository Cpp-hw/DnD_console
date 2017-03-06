#include "Includes/LoginForm.hpp"


// methods for changing the state of the button
void LoginForm::fButtonRegisterClicked()
{
    _reg_button_click = true;
}

void LoginForm::fInitUIElements()
{
    _theme = std::make_shared<tgui::Theme>("Interface/Black.txt");

    auto windowWidth = tgui::bindWidth(_gui);
    auto windowHeight = tgui::bindHeight(_gui);

    tgui::Picture::Ptr picture = std::make_shared<tgui::Picture>("Interface/login.jpg");
    picture->setSize(windowWidth,windowHeight);
    _gui.add(picture);

    tgui::Picture::Ptr logo1 = std::make_shared<tgui::Picture>("Interface/log.png");
    logo1->setSize(400, 232);
    logo1->setPosition(10, -30);
    _gui.add(logo1);

    _username = _theme->load("EditBox");
    _username->setSize(400, 40);
    _username->setPosition(windowWidth/2+200, windowHeight/2-80);
    _username->setDefaultText("Username");
    _gui.add(_username, "Username");

    _password = _theme->load("EditBox");
    _password->setSize(400, 40);
    _password->setPosition(windowWidth / 2 + 200, windowHeight / 2 -15);
    _password->setPasswordCharacter('*');
    _password->setDefaultText("Password");
    _gui.add(_password, "Password");

    tgui::Button::Ptr _login_button = _theme->load("Button");
    _login_button->setSize(150, 35);
    _login_button->setPosition(windowWidth/2+200, windowHeight/2+50);
    _login_button->setText("Login");
    _gui.add(_login_button);

    tgui::Button::Ptr _register_button = _theme->load("Button");
    _register_button->setSize(240, 35);
    _register_button->setPosition(windowWidth/2+360, windowHeight / 2+50);
    _register_button->setText("Register");

    _gui.add(_register_button);

    _login_button->connect("pressed", &LoginForm::fLogin, this, _username, _password);
    _register_button->connect("pressed", &LoginForm::fButtonRegisterClicked, this);
}
void LoginForm::fLogin(tgui::EditBox::Ptr username, tgui::EditBox::Ptr password)
{
    std::string request = UserActions::fLogin(username->getText(), password->getText().toAnsiString()).dump();
    std::cout << request << std::endl;
    std::string response;
    _http_client->fSendRequest(HttpClient::_POST, "/api/userlogin", request);
    _http_client->fGetResponse(response);
    std::cout << "response" << response << std::endl;

    nlohmann::json json_response = json::parse(response);
    std::string status = json_response["status"];

    if (status == "success" || status == "already logged in")
    {
        _game_session = json_response["session_id"];

        _main_menu_button_click = true;
    }
    else
    {
        tgui::Label::Ptr toolTip = _theme->load("Label");
        toolTip->setTextColor(tgui::Color::Color("red"));
        toolTip->setPosition(tgui::bindWidth(_gui) / 2 + 200, tgui::bindHeight(_gui) / 2 + 115);
        toolTip->setSize(390,60);
        toolTip->setText("This user does not exist or password provided is incorrect");

        _gui.add(toolTip);
        _main_menu_button_click = false;
    }
}


LoginForm::LoginForm(const sf::Event & event, sf::RenderWindow &window, HttpClient *http_client)
{
    _gui.setWindow(window);
    _http_client = http_client;
    _main_menu_button_click = false;
    this->_event = event;
    fInitUIElements();
    _menu_option = _selected_menu::NONE;
}

void LoginForm::fUpdate(sf::RenderWindow &window)
{
    switch (_menu_option)
    {
    case LoginForm::NONE:
    {
        while (window.pollEvent(_event))
        {
        
            if (_event.type == sf::Event::Closed)
                window.close();

            if (_reg_button_click)
            {
                _registration_window = new RegisterForm(_event, window, _http_client);
                _menu_option = REGISTER;
            }

            if (_main_menu_button_click)
            {
                _menu_option = MAIN_MENU;
                _menu_window = new MeinMenuForm(_event, window, _game_session, _http_client);
            }

            _gui.handleEvent(_event);


        }
    }
    break;
    case REGISTER:
    {
        _registration_window->fUpdate(window);
    }
    break;
    case MAIN_MENU:
    {
        _menu_window->fUpdate(window);
    }

    }
}
void LoginForm::fDraw(sf::RenderWindow & window)
{
    switch (_menu_option)
    {
    case LoginForm::NONE:
    {
        _gui.draw();
    }
    break;
    case REGISTER:
    {
        if (_registration_window->display_window)
            _registration_window->fDraw(window);
        else
        {
            delete _registration_window;
            _menu_option = LoginForm::NONE;
            _reg_button_click = false;
        }
    }
    break;
    case MAIN_MENU:
    {
        if (_menu_window->display_window)
            _menu_window->fDraw(window);
        else
        {
            delete _menu_window;
            _menu_option = LoginForm::NONE;
            _main_menu_button_click = false;
        }
    }
    }

}

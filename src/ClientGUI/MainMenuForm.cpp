#include "Includes/MeinMenuForm.hpp"


void MeinMenuForm::fInitUIElements()
{
    auto windowWidth = tgui::bindWidth(_gui);
    auto windowHeight = tgui::bindHeight(_gui);

    _theme = std::make_shared<tgui::Theme>("Interface/Game.txt");
    _main = std::make_shared<tgui::Picture>("Interface/MainMenu.jpg");
    _main->setSize(tgui::bindMax(1280, windowWidth), tgui::bindMax(800, windowHeight));
    _gui.add(_main);

    _menu = std::make_shared<tgui::Picture>("Interface/MenuWindow.png");
    _menu->setPosition(100, 180);
    _menu->setSize(372, 582);
    _gui.add(_menu);

    _logo = std::make_shared<tgui::Picture>("Interface/logo.png");
    _logo->setPosition(100, 100);
    _logo->setSize(372, 122);
    _gui.add(_logo);

    _btn_player = _theme->load("Button");
    _btn_player->setSize(275, 50);
    _btn_player->setPosition(145, 270);
    _btn_player->setText("Player mode");
    _gui.add(_btn_player);

    _btn_dm = _theme->load("Button");
    _btn_dm->setSize(275, 50);
    _btn_dm->setPosition(145, 350);
    _btn_dm->setText("DM mode");
    _gui.add(_btn_dm);

    _btn_exit = _theme->load("Button");
    _btn_exit->setSize(275, 50);
    _btn_exit->setPosition(145, 430);
    _btn_exit->setText("Log Out");
    _gui.add(_btn_exit);

    _btn_exit->connect("pressed", &MeinMenuForm::fExit_clicked, this);
}

void MeinMenuForm::fExit_clicked()
{

    json request;
    request["session_id"] = _game_session;
    std::string response;

    _http_client->fSendRequest(HttpClient::_POST, "/api/userlogout", request.dump());
    _http_client->fGetResponse(response);
    std::cout << response;

    display_window = false;
}

MeinMenuForm::MeinMenuForm(const sf::Event &event, sf::RenderWindow &window, std::string game_session, HttpClient *http_client)
{

    display_window = true;
    _menu_option = NONE;
    _http_client = http_client;
    this->_game_session = game_session;
    _gui.setWindow(window);
    this->_event = event;
    fInitUIElements();

}
void MeinMenuForm::fUpdate(sf::RenderWindow  &window)
{
    switch (_menu_option)
    {
    case MeinMenuForm::NONE:
    {
        while (window.pollEvent(_event))
        {
            if (_event.type == sf::Event::Closed)
                window.close();

            
                if (_btn_dm->mouseOnWidget(_event.mouseButton.x, _event.mouseButton.y))
                {
                    if (_event.type == sf::Event::MouseButtonReleased && _event.mouseButton.button == sf::Mouse::Left)
                    {
                        _dm_window = new DMForm(_event, window, _game_session, _http_client);
                        _menu_option = DM_MODE;
                    }
                }
                if (_btn_player->mouseOnWidget(_event.mouseButton.x, _event.mouseButton.y))
                {
                    if (_event.type == sf::Event::MouseButtonReleased && _event.mouseButton.button == sf::Mouse::Left)
                    {
                        _menu_option = PLAYER_MODE;

                        _player_window = new PlayerForm(_event, window, _game_session, _http_client);
                    }
                }

            _gui.handleEvent(_event);
        }
    }
    break;
    case MeinMenuForm::PLAYER_MODE:
    {
        _player_window->fUpdate(window);
    }
    break;
    case MeinMenuForm::DM_MODE:
    {
        _dm_window->fUpdate(window);
    }
    break;
    case MeinMenuForm::EXIT:
        break;
    }
}

void MeinMenuForm::fDraw(sf::RenderWindow & window)
{
    switch (_menu_option)
    {
    case MeinMenuForm::NONE:
    {
        _gui.draw();
    }
    break;
    case MeinMenuForm::PLAYER_MODE:
    {
        if (_player_window->display_window)
            _player_window->fDraw(window);
        else
        {
            delete _player_window;
            _menu_option = MeinMenuForm::NONE;
        }
    }
    break;
    case MeinMenuForm::DM_MODE:
    {
        if (_dm_window->display_window)
            _dm_window->fDraw(window);
        else
        {
            delete _dm_window;
            _menu_option = MeinMenuForm::NONE;
        }
    }
    break;
    case MeinMenuForm::EXIT:
    {

    }
    break;
    }
}
#include "Includes/PlayerForm.hpp"


void PlayerForm::fClickedCharacter()
{
    _character_button_click = true;
}
void PlayerForm::fClickedBoard()
{
    _board_button_click = true;
}

void PlayerForm::fDisable()
{
    display_window = false;
}

void PlayerForm::fInitUIElements()
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
    _btn_character = _theme->load("Button");
    _btn_character->setSize(275, 50);
    _btn_character->setPosition(145, 270);
    _btn_character->setText("Character menu");
    _gui.add(_btn_character);

    _btn_board = _theme->load("Button");
    _btn_board->setSize(275, 50);
    _btn_board->setPosition(145, 350);
    _btn_board->setText("Board menu");
    _gui.add(_btn_board);

    _btn_back = _theme->load("Button");
    _btn_back->setSize(275, 50);
    _btn_back->setPosition(145, 430);
    _btn_back->setText("Back");
    _gui.add(_btn_back);

    _btn_back->connect("pressed", &PlayerForm::fDisable, this);
    _btn_character->connect("pressed", &PlayerForm::fClickedCharacter, this);
    _btn_board->connect("pressed", &PlayerForm::fClickedBoard, this);
}


PlayerForm::PlayerForm(const sf::Event &event, sf::RenderWindow &window, std::string game_session, HttpClient *http_client)
{
    display_window = true;
    _game_session = game_session;
    _http_client = http_client;
    _gui.setWindow(window);
    _menu_option = NONE;
    fInitUIElements();
    this->_event = event;
}

void PlayerForm::fUpdate(sf::RenderWindow  &window)
{
    switch (_menu_option)
    {
    case PlayerForm::NONE:
    {
        while (window.pollEvent(_event))
        {
            if (_event.type == sf::Event::Closed)
                window.close();

            if (_btn_board->mouseOnWidget(_event.mouseButton.x, _event.mouseButton.y))
            {
                if (_event.type == sf::Event::MouseButtonReleased && _event.mouseButton.button == sf::Mouse::Left)
                {
                    _menu_option = PlayerForm::BOARD_MENU;
                }
            }

            if (_btn_character->mouseOnWidget(_event.mouseButton.x, _event.mouseButton.y))
            {
                if (_event.type == sf::Event::MouseButtonReleased && _event.mouseButton.button == sf::Mouse::Left)
                {
                    _character_window = new CharacterForm(_event, window, _game_session, _http_client);
                    _menu_option = PlayerForm::Character_MENU;
                }
            }
            _gui.handleEvent(_event);
        }
    }
    break;
    case PlayerForm::BOARD_MENU:
    {

    }
    break;
    case PlayerForm::Character_MENU:
    {
        _character_window->fUpdate(window);
    }
    break;
    case PlayerForm::BACK:
        break;
    }
}
void PlayerForm::fDraw(sf::RenderWindow & window)
{
    switch (_menu_option)
    {
    case PlayerForm::NONE:
    {
        _gui.draw();
    }
    break;
    case PlayerForm::BOARD_MENU:
    {

    }
    break;
    case PlayerForm::Character_MENU:
    {
        if (_character_window->display_window)
            _character_window->fDraw(window);
        else
        {
            delete _character_window;
            _menu_option = PlayerForm::NONE;
            _character_button_click = false;
        }
    }
    break;
    }
}

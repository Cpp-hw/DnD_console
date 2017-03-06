#include "Includes/RegisterForm.hpp"


void RegisterForm::fRegistration(tgui::EditBox::Ptr username, tgui::EditBox::Ptr email, tgui::EditBox::Ptr password, tgui::EditBox::Ptr re_password)
{
    information_window->show();
    std::string request = UserActions::fRegistration(username->getText(), email->getText().toAnsiString(), password->getText().toAnsiString(), re_password->getText().toAnsiString()).dump();
    std::cout << request << std::endl;
    std::string response;
    _http_client->fSendRequest(HttpClient::_POST, "/api/userregister", request);
    _http_client->fGetResponse(response);
    std::cout << "response" << response << std::endl;

    nlohmann::json json_response = json::parse(response);
    std::string status = json_response["status"];

    if (status == "success")
    {
        std::cout << "OK" << std::endl;
        //information_window->setCloseButton(_back);
    }
}

void RegisterForm::fBack_Click()
{
    display_window = false;
}

void RegisterForm::fInitUIElements()
{
    _theme = std::make_shared<tgui::Theme>("Interface/Black.txt");
    auto windowWidth = tgui::bindWidth(_gui);
    auto windowHeight = tgui::bindHeight(_gui);

    _picture = std::make_shared<tgui::Picture>("Interface/login.jpg");
    _picture->setSize(windowWidth,windowHeight);
    _gui.add(_picture);

    tgui::Picture::Ptr logo1 = std::make_shared<tgui::Picture>("Interface/log.png");
    logo1->setSize(400, 232);
    logo1->setPosition(10, -30);
    _gui.add(logo1);

    _editBoxUsername = _theme->load("EditBox");
    _editBoxUsername->setSize(300, 40);
    _editBoxUsername->setPosition(windowWidth/2, windowHeight/2-100);
    _editBoxUsername->setDefaultText("Username");
    _gui.add(_editBoxUsername, "Username");

    _editBoxEmail = _theme->load("EditBox");
    _editBoxEmail->setSize(300, 40);
    _editBoxEmail->setPosition(windowWidth/2+350, windowHeight/2-100);
    _editBoxEmail->setDefaultText("E-mail");
    _gui.add(_editBoxEmail, "E-mail");

    _editBoxPassword = _theme->load("EditBox");
    _editBoxPassword->setSize(300, 40);
    _editBoxPassword->setPosition(windowWidth / 2, windowHeight / 2 +5);
    _editBoxPassword->setPasswordCharacter('*');
    _editBoxPassword->setDefaultText("Password");
    _gui.add(_editBoxPassword, "Password");

    _editBoxPasswordRe = _theme->load("EditBox");
    _editBoxPasswordRe->setSize(300, 40);
    _editBoxPasswordRe->setPosition(windowWidth / 2 + 350, windowHeight / 2 +5);
    _editBoxPasswordRe->setPasswordCharacter('*');
    _editBoxPasswordRe->setDefaultText("Repeat Password");
    _gui.add(_editBoxPasswordRe, "PasswordRe");

    _register_button = _theme->load("Button");
    _register_button->setSize(335, 40);
    _register_button->setPosition(windowWidth/2+155, windowHeight/2+100);
    _register_button->setText("Register");
    _gui.add(_register_button);
    
    _back = _theme->load("Button");
    _back->setSize(335, 40);
    _back->setPosition(windowWidth/2+155, windowHeight/2+160);
    _back->setText("Cancel");
    _gui.add(_back);

    test = _theme->load("Button");
    test->setSize(50, 25);
    test->setPosition(270, 140);
    test->setText("Test");
    test->hide();

    _gui.add(test);

    information_window = _theme->load("ChildWindow");
    information_window->setSize(250, 120);
    information_window->setPosition(windowWidth / 2 - 250 / 2, windowHeight / 2 - 120 / 2);
    information_window->setTitle("Registration success");
    _gui.add(information_window);
    information_window->hide();


    _register_button->connect("pressed", &RegisterForm::fRegistration, this, _editBoxUsername, _editBoxEmail, _editBoxPassword, _editBoxPasswordRe);
    _back->connect("pressed", &RegisterForm::fBack_Click, this);
}

RegisterForm::RegisterForm(const sf::Event &event, sf::RenderWindow &window, HttpClient *http_client)
{
    _http_client = http_client;
    display_window = true;
    window.setTitle("Dungeons & Dragons: Registration");
    _gui.setWindow(window);
    this->_event = event;
    fInitUIElements();
}
void RegisterForm::fUpdate(sf::RenderWindow  &window)
{
    while (window.pollEvent(_event))
    {
        if (_event.type == sf::Event::Closed)
            window.close();
        _gui.handleEvent(_event);
    }
}

void RegisterForm::fDraw(sf::RenderWindow & window)
{
    _gui.draw();
}

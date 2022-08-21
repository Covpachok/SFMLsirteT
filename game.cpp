#include "game.hpp"
#include <random>
#include <sstream>

const unsigned int kWindowHeight = 720;
const unsigned int kWindowWidth  = 960;

const int kScoreForLines[5] = {0, 40, 100, 300, 1200};

const int  kMaxLevel         = 20;
const char kRecordFilename[] = "record";
const int  kControlTickTime  = 8;

const int kTetrominoDefaultTick = 27;

std::default_random_engine              kRand(time(nullptr));
std::uniform_int_distribution<unsigned> kIDist(0, Tetromino::Amount - 1);

Game::Game()
    : _window(sf::VideoMode(kWindowWidth, kWindowHeight), "Tetris",
              sf::Style::Close),
      _tetrominoTickToMove(kTetrominoDefaultTick)
{
    const sf::Color kBGColor(0x181818FF);

    std::ifstream recordFile;
    recordFile.open(kRecordFilename, std::ios::binary);
    recordFile.read((char *)&_bestScore, sizeof(_bestScore));

    _bg.setSize({kWindowWidth, kWindowHeight});
    _bg.setFillColor(kBGColor);

    _tetrominoTextures.load(Tetromino::Type::Projection,
                            "../assets/textures/P.png");
    _tetrominoTextures.load(Tetromino::Type::I, "../assets/textures/I.png");
    _tetrominoTextures.load(Tetromino::Type::J, "../assets/textures/J.png");
    _tetrominoTextures.load(Tetromino::Type::L, "../assets/textures/L.png");
    _tetrominoTextures.load(Tetromino::Type::O, "../assets/textures/O.png");
    _tetrominoTextures.load(Tetromino::Type::Z, "../assets/textures/Z.png");
    _tetrominoTextures.load(Tetromino::Type::T, "../assets/textures/T.png");
    _tetrominoTextures.load(Tetromino::Type::S, "../assets/textures/S.png");

    _backgroundTextures.load(FieldBG, "../assets/textures/bg.png");

    _fonts.load(0, "../assets/font/Font.ttf");

    /*
    _scoreText.setFont(_fonts.get(0));
    _scoreText.setString("Score: 0");
    _scoreText.setPosition(kWindowWidth / 4.f * 2.95f, kWindowHeight / 7.f);

    _levelText.setFont(_fonts.get(0));
    _levelText.setString("Level: 0");
    _levelText.setPosition(kWindowWidth / 4.f * 2.95f, kWindowHeight / 5.f);
     */

    sf::Text tempText;

    auto bounds = _backgroundTextures.get(FieldBG).getSize();

    tempText.setScale(0.5f, 0.5f);

    tempText.setFillColor(sf::Color(0xFFFFFFE0));
    tempText.setFont(_fonts.get(0));
    tempText.setString("SCORE:");

    auto textBounds = tempText.getLocalBounds();

    auto rightX = (kWindowWidth - bounds.x) / 2.f + bounds.x + 30.f;
    auto leftX  = (kWindowWidth - bounds.x) / 2.f - 30.f - 100.f;

    tempText.setPosition(rightX, kWindowHeight / 16.f * 3.f);
    _text.insert({Score, tempText});

    tempText.setString("LEVEL:");
    tempText.setPosition(rightX, kWindowHeight / 16.f * 8.f);
    _text.insert({Level, tempText});

    tempText.setString("BEST :");
    tempText.setPosition(rightX, kWindowHeight / 16.f * 4.f);
    _text.insert({Record, tempText});

    tempText.setString("NEXT");
    tempText.setPosition(rightX + 15.f, kWindowHeight / 16.f * 5.f);
    _text.insert({Next, tempText});

    tempText.setString("HOLD");
    tempText.setPosition(leftX + 15.f, kWindowHeight / 16.f * 5.f);
    _text.insert({Hold, tempText});

    tempText.setFillColor(sf::Color(0xFFFFFF10));
    tempText.setString("MOVE: LEFT & RIGHT\nROTATE: UP\nFAST FALL: DOWN\nHOLD: "
                       "SPACE\nRESTART: R\nRESET BEST: TILDE\nPAUSE: ENTER");
    tempText.setPosition(10.f, 5.f);
    _text.insert({Controls, tempText});

    tempText.setScale(0.7f, 0.7f);
    tempText.setFillColor(sf::Color(0xFFFFFF00));
    tempText.setString("     GAME OVER\nPRESS R FOR RESTART");
    auto tempBounds = tempText.getGlobalBounds();
    tempText.setPosition((kWindowWidth - tempBounds.width) / 2.f, 25.f);
    _text.insert({Restart, tempText});

    tempText.setScale(0.7f, 0.7f);
    tempText.setFillColor(sf::Color(0xFFFFFF00));
    tempText.setString("     GAME PAUSED\nPRESS ENTER TO UNPAUSE");
    tempBounds = tempText.getGlobalBounds();
    tempText.setPosition((kWindowWidth - tempBounds.width) / 2.f, 25.f);
    _text.insert({Pause, tempText});

    _currTetromino = new Tetromino(
        static_cast<Tetromino::Type>(kIDist(kRand)), _tetrominoTextures,
        {(kWindowWidth - bounds.x) / 2.f, (kWindowHeight - bounds.y) / 2.f});

    _nextTetromino = new Tetromino(static_cast<Tetromino::Type>(kIDist(kRand)),
                                   _tetrominoTextures,
                                   {rightX, kWindowHeight / 16.f * 6.f}, false);

    _projection = new Tetromino(
        _currTetromino->getType(), _tetrominoTextures,
        {(kWindowWidth - bounds.x) / 2.f, (kWindowHeight - bounds.y) / 2.f},
        true, true);

    _holdTetromino = new Tetromino(static_cast<Tetromino::Type>(kIDist(kRand)),
                                   _tetrominoTextures,
                                   {leftX, kWindowHeight / 16.f * 6.f}, false);

    _field = new Field(
        _tetrominoTextures, _backgroundTextures.get(FieldBG),
        {(kWindowWidth - bounds.x) / 2.f, (kWindowHeight - bounds.y) / 2.f});

    render();
}

Game::~Game()
{
    saveBestScore();
}

void Game::play()
{
    const sf::Time kTimePerFrame = sf::seconds(1.f / 60.f);
    sf::Clock      clock;
    sf::Time       time_since_last_upd = sf::Time::Zero;

    while (_window.isOpen()) {
        time_since_last_upd += clock.restart();
        while (time_since_last_upd > kTimePerFrame) {
            time_since_last_upd -= kTimePerFrame;
            processEvents();
            if (!_pause) {
                update();
                ++_tick;
            }
        }
        render();
    }
}

void Game::update()
{
    if (_hold) {
        hold();
        _hold = false;
    }

    if (_controlTick >= kControlTickTime) {
        if (_tetrominoMoveRight) {
            if (_field->isActionPossible(*_currTetromino,
                                         Tetromino::MoveRight)) {
                _currTetromino->moveRight();
                //_tetrominoMoveRight = false;
            }
        } else if (_tetrominoMoveLeft) {
            if (_field->isActionPossible(*_currTetromino,
                                         Tetromino::MoveLeft)) {
                _currTetromino->moveLeft();
                //_tetrominoMoveLeft = false;
            }
        } else if (_tetrominoRotate) {
            if (!_field->isActionPossible(*_currTetromino, Tetromino::Rotate)) {
                if (fitTetromino()) {
                    _currTetromino->rotate();
                }
            } else {
                _currTetromino->rotate();
            }
            //_tetrominoRotate = false;
        }
        _controlTick = 0;
    }

    if (_tick >= (_fastFall ? 2 : _tetrominoTickToMove)) {
        if (_field->isOnBlock(*_currTetromino)) {
            _field->saveTetromino(*_currTetromino);
            auto linesAmount = _field->checkLines();
            _linesCleared += linesAmount;
            _score += kScoreForLines[linesAmount] * (_level + 1);
            if (_linesCleared >= (_level + 1) * 10) {
                if (_tetrominoTickToMove > 10) {
                    if (_level < kMaxLevel)
                        ++_level;
                    --_tetrominoTickToMove;
                }
            }
            _fastFall = false;
            newTetromino();
        } else {
            _currTetromino->moveDown();
        }
        _tick = 0;
    }

    updateProjection();

    static char temp[64] = "";
    sprintf(temp, "SCORE: %08lld", _score);
    _text[Score].setString(temp);
    sprintf(temp, "LEVEL: %02lld", _level);
    _text[Level].setString(temp);
    sprintf(temp, "BEST : %08lld", _bestScore);
    _text[Record].setString(temp);

    if (!_field->isTopEmpty()) {
        saveBestScore();
        _pause = true;
        _text[Restart].setFillColor(sf::Color(0xFFFFFFFF));
    }

    ++_controlTick;
}

void Game::handleInput(sf::Keyboard::Key key, bool isPressed)
{
    switch (key) {
    case sf::Keyboard::R:
        if (isPressed)
            restart();
        break;
    case sf::Keyboard::Tilde:
        if (isPressed)
            resetBestScore();
        break;
    case sf::Keyboard::Up:
        _tetrominoRotate = isPressed;
        break;
    case sf::Keyboard::Left:
        _tetrominoMoveLeft = isPressed;
        break;
    case sf::Keyboard::Right:
        _tetrominoMoveRight = isPressed;
        break;
    case sf::Keyboard::Down:
        _fastFall = isPressed;
        break;
    case sf::Keyboard::Space:
        _hold = isPressed;
        break;
    case sf::Keyboard::Enter:
        if (isPressed)
            pauseGame();
        break;
    default:
        break;
    }
}

void Game::processEvents()
{
    sf::Event event{};
    while (_window.pollEvent(event)) {
        switch (event.type) {
        case sf::Event::Closed:
            _window.close();
            break;
        case sf::Event::KeyPressed:
            _controlTick = kControlTickTime;
            handleInput(event.key.code, true);
            break;
        case sf::Event::KeyReleased:
            handleInput(event.key.code, false);
            break;
        default:
            break;
        }
    }
}

void Game::render()
{
    _window.clear();
    _window.draw(_bg);
    _window.draw(*_field);
    _window.draw(*_projection);
    _window.draw(*_currTetromino);
    _window.draw(*_nextTetromino);
    _window.draw(*_holdTetromino);
    for (const auto &t : _text)
        _window.draw(t.second);
    _window.display();
}

void Game::newTetromino()
{
    _currTetromino->reset(_nextTetromino->getType());
    _projection->reset(_currTetromino->getType());
    auto type = static_cast<Tetromino::Type>(kIDist(kRand));
    if (type == _nextTetromino->getType())
        type = static_cast<Tetromino::Type>(kIDist(kRand));
    _nextTetromino->reset(type);
}

bool Game::fitTetromino()
{
    bool left = false, right = false;
    if (_field->isActionPossible(*_currTetromino, Tetromino::MoveLeft)) {
        _currTetromino->moveLeft();
        left = true;
    } else if (_field->isActionPossible(*_currTetromino,
                                        Tetromino::MoveRight)) {
        _currTetromino->moveRight();
        right = true;
    }
    if (_field->isActionPossible(*_currTetromino, Tetromino::Rotate)) {
        return true;
    } else {
        if (left) {
            _currTetromino->moveRight();
        } else if (right) {
            _currTetromino->moveLeft();
        }
        return false;
    }
}

void Game::updateProjection()
{
    _projection->setPos(*_currTetromino);
    while (!_field->isOnBlock(*_projection))
        _projection->moveDown();
}

void Game::restart()
{
    saveBestScore();
    _holdTetromino->reset(static_cast<Tetromino::Type>(kIDist(kRand)));
    _currTetromino->reset(static_cast<Tetromino::Type>(kIDist(kRand)));
    _projection->reset(_currTetromino->getType());
    _nextTetromino->reset(static_cast<Tetromino::Type>(kIDist(kRand)));
    _field->reset();
    _score               = 0;
    _tick                = 0;
    _level               = 0;
    _linesCleared        = 0;
    _hold                = false;
    _fastFall            = false;
    _tetrominoMoveLeft   = false;
    _tetrominoMoveRight  = false;
    _tetrominoRotate     = false;
    _tetrominoTickToMove = kTetrominoDefaultTick;
    _text[Restart].setFillColor(sf::Color(0xFFFFFF00));
    if (_pause)
        pauseGame();
}

void Game::hold()
{
    auto prevType = _currTetromino->getType();
    _currTetromino->reset(_holdTetromino->getType());
    _projection->reset(_currTetromino->getType());
    _holdTetromino->reset(prevType);
}

void Game::saveBestScore()
{
    if (_score > _bestScore) {
        _bestScore = _score;

        std::ofstream oRecordFile;
        oRecordFile.open(kRecordFilename, std::ios::binary);
        oRecordFile.write(reinterpret_cast<char *>(&_bestScore),
                          sizeof(_bestScore));
    }
}

void Game::resetBestScore()
{
    _bestScore = 0;

    std::ofstream oRecordFile;
    oRecordFile.open(kRecordFilename, std::ios::binary);
    oRecordFile.write(reinterpret_cast<char *>(&_bestScore),
                      sizeof(_bestScore));
}

void Game::pauseGame()
{
    if (_pause) {
        _text[Pause].setFillColor(sf::Color(0xFFFFFF00));
        _pause = false;
    } else {
        _text[Pause].setFillColor(sf::Color(0xFFFFFFFF));
        _pause = true;
    }
}

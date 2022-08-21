#ifndef __GAME_HPP__
#define __GAME_HPP__

#include "field.hpp"
#include "resource_holder.hpp"
#include "tetromino.hpp"
#include <map>
#include <fstream>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Window/Keyboard.hpp>

class Game {
    enum BackgroundID {
        FieldBG
    };
    enum TextID {
        Score, Level, Record, Next, Hold, Controls, Restart, Pause
    };

    ResourceHolder<sf::Texture, Tetromino::Type> _tetrominoTextures;
    ResourceHolder<sf::Texture, BackgroundID> _backgroundTextures;
    ResourceHolder<sf::Font, int> _fonts;

    sf::RenderWindow _window;

    std::map<TextID, sf::Text> _text;

    sf::RectangleShape _bg;

    Tetromino *_currTetromino, *_nextTetromino, *_projection, *_holdTetromino;
    Field *_field;

    int _tetrominoTickToMove;

    bool _fastFall = false;
    bool _tetrominoMoveLeft = false;
    bool _tetrominoMoveRight = false;
    bool _tetrominoRotate = false;
    bool _hold = false;
    bool _pause = false;

    long long _tick = 0, _score = 0, _level = 0, _linesCleared = 0, _controlTick = 0;

    long long _bestScore = 0;

public:
    Game();

    ~Game();

    void play();

private:
    void update();

    void handleInput(sf::Keyboard::Key key, bool isPressed);

    void processEvents();

    void render();

    void newTetromino();

    bool fitTetromino();

    void updateProjection();

    void hold();

    void restart();

    void saveBestScore();

    void resetBestScore();

    void pauseGame();
};

#endif

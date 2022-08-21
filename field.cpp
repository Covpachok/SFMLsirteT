#include "field.hpp"
#include "tetromino.hpp"
#include <SFML/Graphics/Texture.hpp>

Field::Field(const ResourceHolder<sf::Texture, Tetromino::Type> &tetrominoTextures, const sf::Texture &bgTexture,
             sf::Vector2f relativePos)
{
    setOrigin(relativePos);
    _sBackground.setTexture(bgTexture);
    _sBackground.setPosition(relativePos);
    for (int i = 0; i < Tetromino::Amount; ++i)
        _sTetromino[i].setTexture(tetrominoTextures.get(static_cast<Tetromino::Type>(i)));

    for (int y = 0; y < kFieldHeight; ++y) {
        for (int x = 0; x < kFieldWidth; ++x) {
            _field[y][x] = Tetromino::None;
        }
    }
}

void Field::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    static const float blockSize = _sTetromino->getGlobalBounds().width;

    auto       relativePos = getOrigin();
    sf::Sprite block;

    target.draw(_sBackground);
    for (int y = 0; y < kFieldHeight; ++y) {
        for (int x = 0; x < kFieldWidth; ++x) {
            if (_field[y][x] != Tetromino::None) {
                block = _sTetromino[_field[y][x]];
                block.setPosition(x * blockSize + relativePos.x, y * blockSize + relativePos.y);
                target.draw(block);
            }
        }
    }
}

void Field::saveTetromino(const Tetromino &tetromino)
{
    for (int i = 0; i < Tetromino::kBlocksAmount; ++i)
        _field[tetromino.getBlockY(i)][tetromino.getBlockX(i)] = tetromino.getType();
}

bool Field::isOnBlock(const Tetromino &tetromino) const
{
    for (int i = 0; i < Tetromino::kBlocksAmount; ++i) {
        auto pos = tetromino.getBlockPos(i);
        if (pos.y == kFieldHeight - 1 || !(pos.y <= 0 || _field[pos.y + 1][pos.x] == Tetromino::None))
            return true;
    }
    return false;
}

int Field::checkLines()
{
    auto fullLines = findFullLines();
    for (auto line : fullLines)
        deleteLine(line);
    return fullLines.size();
}

void Field::deleteLine(int n)
{
    for (int x = 0; x < kFieldWidth; ++x)
        _field[n][x] = Tetromino::None;

    // field shift down
    for (int y = n; y > 0; --y) {
        for (int x = 0; x < kFieldWidth; ++x) {
            _field[y][x] = _field[y - 1][x];
        }
    }
}

std::vector<int> Field::findFullLines() const
{
    std::vector<int> result;
    for (int y = 0; y < kFieldHeight; ++y) {
        int count = 0;
        for (int x = 0; x < kFieldWidth; ++x) {
            if (_field[y][x] != Tetromino::None)
                ++count;
        }
        if (count == kFieldWidth)
            result.push_back(y);
    }
    return result;
}

bool Field::isActionPossible(Tetromino tetromino, Tetromino::Action action) const
{
    switch (action) {
    case Tetromino::Rotate:
        tetromino.rotate();
        break;
    case Tetromino::MoveLeft:
        tetromino.moveLeft();
        break;
    case Tetromino::MoveRight:
        tetromino.moveRight();
        break;
    default:
        return true;
    }

    for (int i = 0; i < Tetromino::kBlocksAmount; ++i) {
        auto block = tetromino.getBlockPos(i);
        if (block.x < 0 || block.x >= kFieldWidth || !(block.y <= 0 || _field[block.y][block.x] == Tetromino::None))
            return false;
    }

    return true;
}

bool Field::isTopEmpty() const
{
    for (int i = kFieldWidth / 2 - 2; i < kFieldWidth / 2 + 2; ++i)
        if (_field[0][i] != Tetromino::None || _field[2][i] != Tetromino::None || _field[1][i] != Tetromino::None)
            return false;
    return true;
}

void Field::reset() {
    for(int y = 0; y < kFieldHeight; ++y)
        for(int x = 0; x < kFieldWidth; ++x)
            _field[y][x] = Tetromino::None;
}

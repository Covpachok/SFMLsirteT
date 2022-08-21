#include "tetromino.hpp"
#include "field.hpp"

Tetromino::Tetromino(Type type, ResourceHolder<sf::Texture, Type> &textures, sf::Vector2f relativePos, bool onField,
                     bool isProjection)
    : _type(type), _rotation(0), _onField(onField), _isProjection(isProjection)
{
#ifndef NDEBUG
    assert(_type != Type::None);
#endif
    setOrigin(relativePos);
    updateShape();
    for (int i = 0; i < Type::Amount; ++i)
        _sprites[i].setTexture(textures.get(static_cast<Type>(i)));
    _sprites[Type::Amount].setTexture(textures.get(Type::Projection));
}

void Tetromino::reset(Tetromino::Type type)
{
    _type     = type;
    _rotation = 0;
    updateShape();
}

void Tetromino::rotate()
{
    for (int i = 0; i < kBlocksAmount; ++i) {
        _blocks[i].x -= kShapes[_type][_rotation][i][0];
        _blocks[i].y -= kShapes[_type][_rotation][i][1];
    }

    if (_rotation >= 3)
        _rotation = 0;
    else
        ++_rotation;

    for (int i = 0; i < kBlocksAmount; ++i) {
        _blocks[i].x += kShapes[_type][_rotation][i][0];
        _blocks[i].y += kShapes[_type][_rotation][i][1];
    }
}

// maybe should pass here game field
void Tetromino::moveDown()
{
    for (auto & _block : _blocks)
        ++_block.y;
}

void Tetromino::moveLeft()
{
    for (auto & _block : _blocks)
        --_block.x;
}

void Tetromino::moveRight()
{
    for (auto & _block : _blocks)
        ++_block.x;
}

void Tetromino::centralize(int width)
{
    for (auto & _block : _blocks)
        _block.x += (width - kBlocksAmount) / 2;
}

void Tetromino::setPos(const Tetromino &tetromino)
{
    for (int i = 0; i < kBlocksAmount; ++i) {
        _blocks[i].x = tetromino._blocks[i].x;
        _blocks[i].y = tetromino._blocks[i].y;
    }
}

void Tetromino::updateShape()
{
    if (_type != None) {
        for (int i = 0; i < kBlocksAmount; ++i) {
            _blocks[i].x = kShapes[_type][_rotation][i][0];
            _blocks[i].y = kShapes[_type][_rotation][i][1];
        }
    }
    if (_onField) {
        centralize(Field::kFieldWidth);
    }
}

void Tetromino::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    static const float blockSize = _sprites->getGlobalBounds().width;

    auto relativePos = getOrigin();
    auto block       = _sprites[_type];
    if (_isProjection)
        block = _sprites[Type::Amount];

    for (auto _block : _blocks) {
        if (_block.y >= 0) {
            block.setPosition(_block.x * blockSize + relativePos.x, _block.y * blockSize + relativePos.y);
            target.draw(block);
        }
    }
}

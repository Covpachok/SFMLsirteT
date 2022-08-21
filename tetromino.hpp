#ifndef __TETROMINO_HPP__
#define __TETROMINO_HPP__

#include "resource_holder.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Transform.hpp>
#include <SFML/System/Vector2.hpp>

class Tetromino : public sf::Drawable, public sf::Transformable {
public:
    enum Type { Projection = -2, None = -1, O = 0, S, Z, I, L, J, T, Amount };
    enum Action { Rotate, MoveLeft, MoveRight, MoveDown };

    static constexpr int kBlocksAmount    = 4;
    static constexpr int kRotationsAmount = 4;

    // describes positions of all blocks per each tetromino type with all possible rotations.
    // nintendo rotation system used.
    static constexpr int kShapes[Type::Amount][kRotationsAmount][kBlocksAmount][2] = {
  // O
        {{{1, 0}, {2, 0}, {1, 1}, {2, 1}},
         {{1, 0}, {2, 0}, {1, 1}, {2, 1}},
         {{1, 0}, {2, 0}, {1, 1}, {2, 1}},
         {{1, 0}, {2, 0}, {1, 1}, {2, 1}}  },

 // S
        {{{0, 1}, {1, 1}, {1, 0}, {2, 0}},
         {{1, -1}, {1, 0}, {2, 0}, {2, 1}},
         {{0, 1}, {1, 1}, {1, 0}, {2, 0}},
         {{1, -1}, {1, 0}, {2, 0}, {2, 1}} },

 // Z
        {{{0, 0}, {1, 0}, {1, 1}, {2, 1}},
         {{1, 1}, {1, 0}, {2, 0}, {2, -1}},
         {{0, 0}, {1, 0}, {1, 1}, {2, 1}},
         {{1, 1}, {1, 0}, {2, 0}, {2, -1}} },

 // I
        {{{0, 0}, {1, 0}, {2, 0}, {3, 0}},
         {{2, -2}, {2, -1}, {2, 0}, {2, 1}},
         {{0, 0}, {1, 0}, {2, 0}, {3, 0}},
         {{2, -2}, {2, -1}, {2, 0}, {2, 1}}},

 // L
        {{{0, 1}, {0, 0}, {1, 0}, {2, 0}},
         {{0, -1}, {1, -1}, {1, 0}, {1, 1}},
         {{0, 0}, {1, 0}, {2, 0}, {2, -1}},
         {{1, -1}, {1, 0}, {1, 1}, {2, 1}} },

 // J
        {{{0, 0}, {1, 0}, {2, 0}, {2, 1}},
         {{0, 1}, {1, 1}, {1, 0}, {1, -1}},
         {{0, -1}, {0, 0}, {1, 0}, {2, 0}},
         {{1, 1}, {1, 0}, {1, -1}, {2, -1}}},

 // T
        {{{0, 0}, {1, 0}, {1, 1}, {2, 0}},
         {{0, 0}, {1, 0}, {1, 1}, {1, -1}},
         {{0, 0}, {1, 0}, {1, -1}, {2, 0}},
         {{1, -1}, {1, 0}, {1, 1}, {2, 0}} },
    }; // kShapes

private:
    Type         _type;
    sf::Vector2i _blocks[kBlocksAmount];
    int          _rotation;

    sf::Sprite _sprites[Type::Amount + 1];

    bool _onField;
    bool _isProjection;

public:
    Tetromino(Type type, ResourceHolder<sf::Texture, Type> &textures, sf::Vector2f relativePos, bool onField = true, bool isProjection = false);

    void reset(Type type);
    void rotate();
    void moveDown();
    void moveLeft();
    void moveRight();
    void setPos(const Tetromino &tetromino);
    void centralize(int width);

    int           getBlockX(int n) const { return _blocks[n].x; }
    int           getBlockY(int n) const { return _blocks[n].y; }
    sf::Vector2i  getBlockPos(int n) const { return _blocks[n]; }
    Type          getType() const { return _type; }

    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

private:
    void updateShape();
};

#endif // __TETROMINO_HPP__
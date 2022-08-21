#ifndef __FIELD_HPP__
#define __FIELD_HPP__

#include "resource_holder.hpp"
#include "tetromino.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Transform.hpp>

class Field : public sf::Drawable, public sf::Transformable {
public:
    static constexpr int kFieldHeight = 20;
    static constexpr int kFieldWidth  = 10;

private:
    typedef Tetromino::Type Cell;

    Cell       _field[kFieldHeight][kFieldWidth];
    sf::Sprite _sTetromino[Tetromino::Amount];
    sf::Sprite _sBackground;

public:
    Field(const ResourceHolder<sf::Texture, Tetromino::Type> &tetrominoTextures, const sf::Texture &bgTexture,
          sf::Vector2f relativePos);

    void saveTetromino(const Tetromino &tetromino);
    int  checkLines();

    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
    
    bool         isOnBlock(const Tetromino &tetromino) const;
    bool         isActionPossible(Tetromino tetromino, Tetromino::Action action) const;
    bool         isTopEmpty() const;
    
    void reset();

private:
    void deleteLine(int n);

    std::vector<int> findFullLines() const;

private:
};

#endif
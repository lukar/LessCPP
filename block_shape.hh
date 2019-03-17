#ifndef BLOCK_SHAPE_H
#define BLOCK_SHAPE_H

#include "wall.hh"
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Export.hpp>
#include <SFML/Graphics/Shape.hpp>

namespace sf
{
class BlockShape : public Drawable
{
private:
    void draw(RenderTarget& target, RenderStates states) const;
    RectangleShape surface;
    RectangleShape vertical_line;
    RectangleShape horizontal_line;
    WallShape wall;
    const float m_size;
    const float m_line_width;
    const float m_inner_size;
    const float m_border;

public:
    explicit BlockShape(const float size, const float border);
    void setPosition(float x, float y);
};
} // namespace sf

#endif /* BLOCK_SHAPE_H */

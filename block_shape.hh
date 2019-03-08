#ifndef BLOCK_SHAPE_H
#define BLOCK_SHAPE_H

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Export.hpp>
#include <SFML/Graphics/Shape.hpp>

namespace sf
{
class SFML_GRAPHICS_API BlockShape : public Drawable
{
private:
    virtual void draw(RenderTarget& target, RenderStates states) const;
    RectangleShape surface;
    RectangleShape vertical_line;
    RectangleShape horizontal_line;
    const float m_size;
    const float m_line_width;
    const float m_inner_size;
    const float m_border;

public:
    explicit BlockShape(const float, const float);
    void setPosition(float, float);
};
} // namespace sf

#endif /* BLOCK_SHAPE_H */

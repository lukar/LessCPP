#ifndef WALL_H
#define WALL_H

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Export.hpp>
#include <SFML/Graphics/Shape.hpp>
#include <vector>

namespace wall {
typedef std::vector<std::vector<int>> WallConfig;

// static variabla garantira, da bo obstajala samo ena kopija na .cc fajl
// (Brez tega ne dela!)
static const WallConfig bFullSide = {{1, -1, -1}, {1, -1, 1}};
static const WallConfig bHalfSide = {{1, -1, 1}};
static const WallConfig bZigZag = {{1, -1, -1}, {-1, 0, -1}, {1, 0, 1}};
static const WallConfig bT_Block = {{1, -1, -1}, {1, -1, 1}, {-1, 0, -1}};
static const WallConfig bTopLeft = {{1, -1, -1}, {-1, 0, -1}};
static const WallConfig bTopRight = {{1, 0, -1}, {-1, 0, 1}};
static const WallConfig bBottomRight = {{1, 0, 1}, {-1, 1, 1}};

static std::vector<WallConfig> wall_configs{
    bFullSide, bHalfSide, bZigZag, bT_Block, bTopLeft, bTopRight, bBottomRight};
} // namespace wall

namespace sf {
class WallShape : public Drawable {
public:
  explicit WallShape(float width, float length, int rotation, int config);
  void setPosition(float x, float y);
  virtual std::size_t getPointCount() const = 0;
  virtual Vector2f getPoint(std::size_t index) const = 0;

private:
  void draw(RenderTarget &target, RenderStates states) const;
  const int m_rotation;
  const float m_width;
  const float m_length;
  wall::WallConfig m_wall;
  std::vector<RectangleShape> m_wallShape;
};
}; // namespace sf

#endif /* WALL_H */

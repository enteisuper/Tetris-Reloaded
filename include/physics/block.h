//
// Created by Henrik Tseng on 4/19/20.
//

#ifndef FINALPROJECT_BLOCK_H
#define FINALPROJECT_BLOCK_H

#include <Box2D/Collision/Shapes/b2PolygonShape.h>
#include <Box2D/Common/b2Draw.h>
#include <Box2D/Dynamics/b2Body.h>
#include <Box2D/Dynamics/b2Fixture.h>
#include <cinder/Color.h>

#include <vector>
namespace tetris {
// defined as a class type object
class World;

/**
 * Holds the physics engine fixtures and the color of a single block in game
 */
class Block {
 public:
  // represent a block moving left or right
  enum Move {
    kMoveLeft,
    kMoveRight,
    kMoveDown,
    kRotate
  };

  /**
   * Holds a b2Transform of the block's transformation and
   * also the amount of times the block has been rotated
   */
  struct Transform : public b2Transform {
    size_t times_rotated_;

    Transform(b2Transform transform, size_t times_rotated) :
        b2Transform(transform), times_rotated_(times_rotated) {}

    explicit Transform(Block block) :
        b2Transform(block.GetBody()->GetTransform()),
        times_rotated_(block.GetTimesRotated()) {}
  };

  /**
   * Holds the fixture for the physics engine of a single tile
   * on the game board and the color of the tile.
   */
  struct Tile {
    b2Fixture* fixture_;
    cinder::Color color_;

    // constructor for regular tile
    Tile(b2Fixture* set_fixture, const cinder::Color& set_color)
        : fixture_(set_fixture), color_(set_color) {};

    // constructor for empty tile
    Tile() : fixture_(nullptr), color_(cinder::Color::black()) {};
  };

 private:
  std::vector<b2Vec2> tile_list_;
  cinder::Color color_;
  b2Body* body_;
  size_t template_id_;
  size_t times_rotated_;

 public:
  /**
   * Get the bounds of a body
   * @param world the world
   * @return bounding box
   */
  b2AABB GetBlockBox(World *world);

  /**
   * sets the tile at x and y
   * @param shape shape of object
   * @param col x position
   * @param row y position
   */
  static void SetTileShapeAtColRow(b2PolygonShape* shape,
      double col, double row);

  Block() : body_(nullptr), template_id_(-1), times_rotated_(0) {};

  Block(std::vector<b2Vec2>& tile_list,
      const cinder::Color& color, size_t template_id)
      : tile_list_(tile_list),
      color_(color), body_(nullptr), template_id_(template_id),
      times_rotated_(0) {}

  Block(const Block& block, b2Body* body)
      : tile_list_(block.tile_list_),
      color_(block.color_), body_(body), template_id_(block.template_id_),
      times_rotated_(0) {}

  const std::vector<b2Vec2>& GetTileList() const {
    return tile_list_;
  }

  /**
   * Get a list of tiles that form the bounding box
   * @return list of tiles
   */
  const std::vector<b2AABB> GetBoundingBoxList() const;

  cinder::Color GetColor() const {
    return color_;
  }

  b2Body* GetBody() const {
    return body_;
  }

  const size_t& GetTemplateId() const {
    return template_id_;
  }

  size_t GetTimesRotated() {
    return times_rotated_;
  }

  void SetTimesRotated(size_t times_rotated) {
    times_rotated_ = times_rotated;
  }

};

} // namespace tetris
#endif  // FINALPROJECT_BLOCK_H

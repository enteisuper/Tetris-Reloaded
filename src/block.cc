//
// Created by Henrik Tseng on 4/25/20.
//

#include "physics/block.h"

#include <Box2D/Dynamics/b2Fixture.h>
#include <physics/world.h>

namespace tetris {

b2AABB Block::GetBlockBox(World* world) {
  b2Fixture* fixture_list = body_->GetFixtureList();
  b2AABB body_aabb;
  // manually set constructor
  body_aabb.lowerBound.x = world->GetTotalNumCol();
  body_aabb.lowerBound.y = world->GetTotalNumRow();
  body_aabb.upperBound.x = 0.0;
  body_aabb.upperBound.y = 0.0;
  for (b2Fixture* fixture = fixture_list; fixture != nullptr;
      fixture = fixture->GetNext()) {
    body_aabb.Combine(fixture->GetAABB(0));
  }

  return body_aabb;
}

const std::vector<b2AABB> Block::GetBoundingBoxList() const {
  std::vector<b2AABB> bound_box_list;
  b2Fixture* fixture_list = body_->GetFixtureList();
  for (b2Fixture* fixture = fixture_list; fixture != nullptr;
       fixture = fixture->GetNext()) {
    bound_box_list.push_back(fixture->GetAABB(0));
  }

  return bound_box_list;
}

void Block::SetTileShapeAtColRow(b2PolygonShape* shape,
    double col, double row) {
  // smaller blocks for easier collision
  double scaled_size = 0.8;
  shape->SetAsBox(scaled_size / 2, scaled_size / 2,
      b2Vec2(col + 0.5, row + 0.5),
      0.0f);
}

} // namespace tetris

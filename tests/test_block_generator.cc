//
// Created by Henrik Tseng on 4/20/20.
//

#include <Box2D/Dynamics/b2World.h>
#include <physics/world.h>

#include <catch2/catch.hpp>

#include "physics/block_generator.h"

namespace tetris {

TEST_CASE("Test block generator constructor", "[block-generator]") {
  BlockGenerator block_generator(false);
  const std::vector<Block>& shape_list = block_generator.GetBlockTemplateList();
  REQUIRE(shape_list.size() == 7);
}

TEST_CASE("Create Random Shape", "[block-generator]") {
  BlockGenerator block_generator(false);
  World world;
  Block* block = block_generator.CreateRandomBlock(&world);
  // makes sure the block is created in the correct world
  REQUIRE(block->GetBody()->GetWorld() == world.GetB2World());
}

TEST_CASE("Create Set Shape", "[block-generator]") {
  BlockGenerator block_generator(false);
  World world;
  // create 4 by 1 block
  Block* block = block_generator.CreateBlockByTemplate(&world, 0);
  // makes sure the block is created in the correct world
  REQUIRE(block->GetBody()->GetWorld() == world.GetB2World());
}

TEST_CASE("Create Bomb Block", "[block-generator][bomb]") {
  BlockGenerator block_generator(true);
  World world;
  world.SetIsBombMode(true);
  int bomb_id = World::kBombId;
  Block* block = block_generator.CreateBlockByTemplate(&world, bomb_id);
  // Check id
  REQUIRE(block->GetTemplateId() == bomb_id);
}

TEST_CASE("Check block functions", "[block-generator][create-block][block]") {
  BlockGenerator block_generator(false);
  World world;
  world.SetCurrentGameState(World::kClassic);
  Block* block = block_generator.CreateBlockByTemplate(&world, 0);
  SECTION("Check Id") {
    REQUIRE(block->GetTemplateId() == 0);
  }

  SECTION("Check Color") {
    cinder::Color color = block->GetColor();
    REQUIRE(color.r < 0.1);
    REQUIRE(color.g > 0.9);
    REQUIRE(color.b > 0.9);
  }

  SECTION("Times Rotated") {
    // default should be 0
    REQUIRE(block->GetTimesRotated() == 0);
  }

  SECTION("Get bounding box") {
    b2AABB bound_box = block->GetBlockBox(&world);
    REQUIRE(bound_box.lowerBound.x > 4.9);
    REQUIRE(bound_box.lowerBound.y > 23);
    REQUIRE(bound_box.upperBound.x < 9);
    REQUIRE(bound_box.upperBound.y < 26);
  }

  SECTION("Get Bounding Box List") {
    std::vector<b2AABB> bound_box_list = block->GetBoundingBoxList();
    REQUIRE(bound_box_list.size() == 4);
  }
}

} // namespace tetris

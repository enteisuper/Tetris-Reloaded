// Copyright (c) 2020 CS126SP20. All rights reserved.

#define CATCH_CONFIG_MAIN

#include <cinder/Rand.h>
#include <catch2/catch.hpp>

#include "physics/world.h"

namespace tetris {

TEST_CASE("Test world Constructor", "[world-constructor][world]") {
  World world;
  // checking if b2world is created properly
  b2World* b2world = world.GetB2World();
  // empty world until mode is declared
  REQUIRE(b2world->GetBodyCount() == 0);
}

TEST_CASE("Test Single Step", "[world][classic]") {
  World world;
  world.SpawnNewRandomBlock();

  SECTION("Classic mode step") {
    world.SetCurrentGameState(World::kClassic);
    // checking if step is executed properly
    world.Step();
    b2World* b2world = world.GetB2World();
    // Body should be the moved tetris block
    b2Body* body = b2world->GetBodyList();
    b2Vec2 position = body->GetPosition();
    REQUIRE(position.x > 10);
    REQUIRE(position.x < 17);
    REQUIRE(position.y > 10);
    REQUIRE(position.y < 15);
  }

  SECTION("Reloaded mode step") {
    world.SetCurrentGameState(World::kReloaded);
    // checking if step is executed properly
    world.Step();
    b2World* b2world = world.GetB2World();
    // Body should be the moved tetris block
    b2Body* body = b2world->GetBodyList();
    b2Vec2 position = body->GetPosition();
    // y is greater due to reloaded mode's larger board size
    REQUIRE(position.x > 8);
    REQUIRE(position.x < 12);
    REQUIRE(position.y > 45);
    REQUIRE(position.y < 55);
  }
}

TEST_CASE("Test Spawning New Block", "[world][world-spawn]") {
  World world;
  world.SetCurrentGameState(World::kClassic);
  // checking if b2world is created properly
  b2World* b2world = world.GetB2World();
  world.SpawnNewRandomBlock();
  // only ground, 2 walls, and 1 tetris block should be created
  int body_num = b2world->GetBodyCount();
  REQUIRE(body_num == 4);
}

TEST_CASE("Test Score", "[world-constructor][world][world-score]") {
  World world;
  // score should be 0 at start
  REQUIRE(world.GetScore() == 0);
}

TEST_CASE("Test Floor Tile Array",
    "[world-constructor][world][world-tile-array][classic][reloaded]") {
  World world;

  SECTION("Floor size in classic mode") {
    world.SetCurrentGameState(World::kClassic);
    // Floor should be default 24 size at start
    REQUIRE(world.GetFloorTileArray().size() == 24);
  }

  SECTION("Floor size in Reloaded mode") {
    world.SetCurrentGameState(World::kReloaded);
    // Floor should be default 24 size at start
    REQUIRE(world.GetFloorTileArray().size() == 48);
  }
}

TEST_CASE("Moving commands", "[world-constructor][world][move]") {
  World world;
  world.SetCurrentGameState(World::kClassic);
  world.SpawnNewRandomBlock();

  SECTION("Moving left") {
    world.Move(Block::kMoveLeft);
    REQUIRE(world.GetCurrentMoveStatus() == World::kValidateLegalMove);
  }

  SECTION("Moving Right") {
    world.Move(Block::kMoveRight);
    REQUIRE(world.GetCurrentMoveStatus() == World::kValidateLegalMove);
  }

  SECTION("Moving Down") {
    world.Move(Block::kMoveDown);
    REQUIRE(world.GetCurrentMoveStatus() == World::kValidateLegalMove);
  }

  SECTION("Rotate Block") {
    world.Move(Block::kRotate);
    REQUIRE(world.GetCurrentMoveStatus() == World::kValidateLegalMove);
  }
}

TEST_CASE("World setters and getters",
    "[world-constructor][world][getter][setter][block]") {
  World world;

  SECTION("Bomb mode") {
    world.SetIsBombMode(true);
    REQUIRE(world.GetIsBombMode());
  }

  SECTION("Disconnected mode") {
    world.SetIsTileDisconnectedMode(true);
    REQUIRE(world.GetIsTileDisconnectedMode());
  }
}

} // namespace tetris

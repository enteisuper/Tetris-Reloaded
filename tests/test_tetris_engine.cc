//
// Created by Henrik Tseng on 5/2/20.
//

#include <catch2/catch.hpp>
#include "tetris_engine.h"

namespace tetris {
TEST_CASE("Test engine constructor",
    "[tetris-engine-constructor][tetris-engine]") {
  TetrisEngine engine;
  REQUIRE(engine.GetWorld().GetCurrentGameState() ==
      engine.GetWorld().kChooseMode);
}

TEST_CASE("Test engine setter/getter",
    "[tetris-engine-constructor][tetris-engine][tetris-engine-getter]"
    "[tetris-engine-setter]") {
  TetrisEngine engine;

  SECTION("Get world score should be 0") {
    REQUIRE(engine.GetWorld().GetScore() == 0);
  }

  SECTION("Get game state") {
    REQUIRE(engine.GetCurrentGameState() == World::kChooseMode);
  }
  SECTION("Set and get game state") {
    engine.SetCurrentGameState(World::kReloaded);
    REQUIRE(engine.GetCurrentGameState() == World::kReloaded);
  }
}

TEST_CASE("Engine step", "[tetris-engine-constructor][tetris-engine][step]") {
  TetrisEngine engine;
  engine.GetWorld().SetCurrentGameState(World::kClassic);
  engine.GetWorld().Step();
  REQUIRE(engine.GetCurrentGameState() == World::kClassic);
}

TEST_CASE("Engine move", "[tetris-engine-constructor][tetris-engine][move]") {
  TetrisEngine engine;
  engine.GetWorld().SetCurrentGameState(World::kClassic);
  engine.GetWorld().SpawnNewRandomBlock();
  SECTION("Move left") {
    engine.Move(Block::kMoveLeft);
    REQUIRE(engine.GetCurrentGameState() == World::kClassic);
  }

  SECTION("Move right") {
    engine.Move(Block::kMoveRight);
  }

}

} // namespace tetris

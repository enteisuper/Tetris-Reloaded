// Copyright (c) 2020 [Henrik Tseng]. All rights reserved.
#ifndef TETRIS_H_
#define TETRIS_H_

#include <physics/world.h>

namespace tetris {

class TetrisEngine {

 private:
  World world_;

 public:

  TetrisEngine();

  /**
   * Moves/rotates the block in the direction given
   * @param move direction/rotation
   */
  void Move(Block::Move move) {
    world_.Move(move);
  }

  World& GetWorld() {
    return world_;
  }

  const World& GetWorld() const {
    return world_;
  }

  const World::GameState& GetCurrentGameState() const {
    return world_.GetCurrentGameState();
  };

  void SetCurrentGameState(World::GameState game_mode) {
    world_.SetCurrentGameState(game_mode);
  };

};

}  // namespace physics


#endif // TETRIS_H_

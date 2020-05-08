//
// Created by Henrik Tseng on 4/19/20.
//

#ifndef FINALPROJECT_WORLD_H
#define FINALPROJECT_WORLD_H

#include <Box2D/Dynamics/b2World.h>
#include <cinder/audio/Voice.h>

#include <vector>

#include "block_generator.h"

namespace tetris {

/**
 * Deals with the physics engine and drawing the current blocks in game
 */
class World {
 private:
  constexpr static const double kDefaultBlockVerticalSpeed = -6.0;
 public:
  constexpr static const double kDefaultWorldNumCol = 10.0;
  constexpr static const double kDefaultWorldNumRow = 24.0;
  constexpr static const int kBombId = 7;
  static const int kGroundFloorInitialHeight = 10;
  static const int kRotatesForFullCircle = 4;

  enum MoveStatus {
    kValidateLegalMove,
    kDetectIllegal,
    kHitGround,
    kMoveOk
  };

  enum GameState {
    kChooseMode,
    kClassic,
    kReloaded,
    kEndScreen
  };

 private:
  const float kTimeStep = 1.0f/60.0f;
  const int32 kVelocityIterations = 6;
  const int32 kPositionIterations = 2;
  const int32 kDisconnectVelocityIter = 2;
  const int32 kDisconnectPositionIter = 6;

  // Audio files
  cinder::audio::VoiceSamplePlayerNodeRef row_complete_sound_;
  cinder::audio::VoiceSamplePlayerNodeRef block_collide_sound_;
  cinder::audio::VoiceSamplePlayerNodeRef bomb_explode_sound_;
  b2World* b2_world_;
  Block* moving_block_;
  BlockGenerator* block_generator_;
  b2Body* ground_floor_body_;
  // if not filled in, color_ is black
  std::vector<std::vector<Block::Tile>> floor_tile_array_;
  // variables used for determining legal moves
  MoveStatus move_status_;
  Block::Transform previous_legal_transform_;
  size_t current_score_;
  GameState current_game_state_;
  size_t block_to_tile_width_ratio_;
  size_t total_num_col_;
  size_t total_num_row_;
  double expected_block_speed_;
  // used for disconnected mode to detect block stuck on floor
  size_t num_illegal_move_;
  // physical engine rarely checks for iteration if true,
  // causing blocks to not attach properly,
  // falling as a free body for interesting interactions.
  bool is_tile_disconnected_mode_;
  // bomb now added to created blocks
  bool is_bomb_mode_;

  /**
   * Rebuilds the ground floor with floor tile array and floor
   */
  void BuildGroundFloor();

  /**
   * Reverts the illegal move to previous legal position
   */
   void RevertIllegalMove();

   /**
    * Checks if a row is completed and removes the row if so
    */
   void CheckCompleteRow();

   /**
    * Builds the walls in the game engine
    */
   void InitializeWalls();

   /**
    * Deletes surrounding tiles while in bomb mod
    * @param row the row of tile
    * @param col the col of tile
    */
   void BlowUpSurroundingTiles(size_t row, size_t col);

   /**
    * Checks and adds blocks dropping on the floor
    */
   void HandleBlockDroppingOnFloor();

 public:
  World();

  /**
   * Executes a single step.
   */
  void Step();

  /**
   * Spawns a new block at the top of the screen
   */
  void SpawnNewRandomBlock();

  /**
   * Moves/rotates the block based on the input
   * @param direction direction to move/rotate
   */
  void Move(Block::Move direction);

  /**
   * Deals with illegal movements
   * @param other_body other illegal body
   */
  void IllegalMoveCallBack(b2Body* other_body);

  const Block* GetMovingBlock() const {
    return moving_block_;
  }

  b2World* GetB2World() const {
    return b2_world_;
  }

  const std::vector<std::vector<Block::Tile>>& GetFloorTileArray() const {
    return floor_tile_array_;
  }

  std::vector<std::vector<Block::Tile>>& GetFloorTileArray() {
    return floor_tile_array_;
  }

  size_t GetScore() {
    return current_score_;
  }

  const GameState& GetCurrentGameState() const {
    return current_game_state_;
  };

  /**
   * Sets the current game state and initialize tile structure accordingly
   * @param game_state game state
   */
  void SetCurrentGameState(GameState game_state);

  double GetBlockToTileWidthRatio() const {
    return block_to_tile_width_ratio_;
  }

  size_t GetTotalNumCol() const {
    return total_num_col_;
  }

  size_t GetTotalNumRow() const {
    return total_num_row_;
  }

  void SetIsTileDisconnectedMode(bool tile_disconnected) {
    is_tile_disconnected_mode_ = tile_disconnected;
  }

  bool GetIsTileDisconnectedMode() const {
    return is_tile_disconnected_mode_;
  }

  void SetIsBombMode(bool is_bomb) {
    is_bomb_mode_ = is_bomb;
  }

  bool GetIsBombMode() const {
    return is_bomb_mode_;
  }

  double GetExpectedBlockSpeed() const {
    return expected_block_speed_;
  }

  /**
   * Check the move status for testing
   */

  const MoveStatus& GetCurrentMoveStatus() const {
    return move_status_;
  }
};

} // namespace tetris

#endif  // FINALPROJECT_WORLD_H

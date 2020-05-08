//
// Created by Henrik Tseng on 4/19/20.
//

#include "physics/world.h"

#include <Box2D/Collision/Shapes/b2PolygonShape.h>
#include <Box2D/Common/b2Math.h>
#include <Box2D/Dynamics/b2Body.h>
#include <Box2D/Dynamics/b2Fixture.h>
#include <Box2D/Dynamics/b2World.h>
#include <cinder/app/AppBase.h>
#include <cinder/audio/Voice.h>
#include <math.h>
#include <physics/block_contact_listener.h>

#include "../apps/tetris_game.h"

namespace tetris {

// Audio file paths
constexpr const static char kCompleteRowSound[] = "Row_Complete_Sound.mp3";
constexpr const static char kBlockCollisionSound[] = "Block_Collision.mp3";
constexpr const static char kBombExplodeSound[] = "Explosion_Sound.mp3";

World::World() : moving_block_(nullptr), block_generator_(nullptr),
    ground_floor_body_(nullptr),
    move_status_(kMoveOk), previous_legal_transform_(b2Transform(), 0),
    current_score_(0), current_game_state_(kChooseMode),
    block_to_tile_width_ratio_(1), total_num_row_(kDefaultWorldNumRow),
    total_num_col_(kDefaultWorldNumCol),
    expected_block_speed_(kDefaultBlockVerticalSpeed), num_illegal_move_(0),
    is_bomb_mode_(false), is_tile_disconnected_mode_(false) {
  // Creating audio files
  cinder::audio::SourceFileRef complete_row_file = cinder::audio::load(
      cinder::app::loadAsset(kCompleteRowSound));
  row_complete_sound_ = cinder::audio::Voice::create(complete_row_file);
  cinder::audio::SourceFileRef block_collide_file = cinder::audio::load(
      cinder::app::loadAsset(kBlockCollisionSound));
  block_collide_sound_ = cinder::audio::Voice::create(block_collide_file);
  cinder::audio::SourceFileRef bomb_explode_file = cinder::audio::load(
      cinder::app::loadAsset(kBombExplodeSound));
  bomb_explode_sound_ = cinder::audio::Voice::create(bomb_explode_file);
  bomb_explode_sound_->setVolume(2);

  // no gravity needed
  b2Vec2 gravity(0.0f, 0.0f);
  b2_world_ = new b2World(gravity);
  // contact listeners for handling illegal move inputs
  // and revert to previous legal position
  BlockContactListener block_contact_listener;
  b2_world_->SetContactListener(&block_contact_listener);
}

void World::BuildGroundFloor() {
  // rebuilds ground floor if it already exists
  if (ground_floor_body_ != nullptr) {
    b2_world_->DestroyBody(ground_floor_body_);
    ground_floor_body_ = nullptr;
  }

  // creating a ground floor for the game
  b2BodyDef ground_floor_body_def;
  ground_floor_body_def.position.Set(
      0.0f, -kGroundFloorInitialHeight);
  b2PolygonShape* box_ground = new b2PolygonShape;
  box_ground->SetAsBox(50.0f, kGroundFloorInitialHeight);
  b2FixtureDef fixture_def;
  fixture_def.shape = box_ground;
  fixture_def.density = 1.0f;
  fixture_def.friction = 0.0f;
  // elasticity = 0 so block will hit and change speed quickly
  fixture_def.restitution = 0.0f;
  ground_floor_body_ = b2_world_->CreateBody(&ground_floor_body_def);
  ground_floor_body_->CreateFixture(&fixture_def);
  // block will stop moving
  ground_floor_body_->SetType(b2_staticBody);

  // physics engine logic code
  for (size_t row = 0; row < floor_tile_array_.size(); row++) {
    for (size_t col = 0; col < floor_tile_array_[row].size(); col++) {
      Block::Tile tile = floor_tile_array_[row][col];
      // don't color null fixtures with black
      if (tile.fixture_ == nullptr) {
        continue;
      }

      // End the game if the block hits the ceiling
      if (row >= floor_tile_array_.size() - 2) {
        current_game_state_ = kEndScreen;
        return;
      }

      // color in the new block that collided onto the screen
      b2PolygonShape* copy_shape = new b2PolygonShape;
      Block::SetTileShapeAtColRow(copy_shape, col,
          row + kGroundFloorInitialHeight);
      b2FixtureDef* fixture_def = new b2FixtureDef;
      fixture_def->shape = copy_shape;
      fixture_def->density = 1.0f;
      fixture_def->friction = 0.0f;
      // elasticity = 0 so block will hit and change speed quickly
      fixture_def->restitution = 0.0f;
      ground_floor_body_->CreateFixture(fixture_def);
    }
  }

}

void World::Step() {
  // if game is not in progress, don't do anything
  if (current_game_state_ == kChooseMode
      || current_game_state_ == kEndScreen) {
    return;
  }

  if (is_tile_disconnected_mode_) {
    // disconnected mode iterations are different for loose collision
    b2_world_->Step(
        kTimeStep, kDisconnectVelocityIter, kDisconnectPositionIter);
  } else {
    // regular mode with regular iteration checks
    b2_world_->Step(kTimeStep, kVelocityIterations, kPositionIterations);

  }

  // Spawn a block if there is none yet
  if (moving_block_ == nullptr) {
    SpawnNewRandomBlock();
  }

  // checking if last move was legal or not
  if (move_status_ == kDetectIllegal) {
    RevertIllegalMove();
  }

  HandleBlockDroppingOnFloor();
  // reset move status
  move_status_ = kMoveOk;
}

void World::SpawnNewRandomBlock() {
  if (block_generator_ == nullptr) {
    block_generator_ = new BlockGenerator(is_bomb_mode_);
  }

  // create first block
  moving_block_ = block_generator_->CreateRandomBlock(this);
  // reset move status
  move_status_ = kMoveOk;
  num_illegal_move_ = 0;
}

void World::Move(Block::Move direction) {
  if (direction == Block::kMoveLeft) {
    b2AABB bounding_box = moving_block_->GetBlockBox(this);

    // if block is already on left wall, do nothing
    if ((bounding_box.lowerBound.x - 1.0) >= 0.0) {
      Block::Transform old_trans(*moving_block_);
      moving_block_->GetBody()->SetTransform(
          b2Vec2(old_trans.p.x - 1.0, old_trans.p.y),
          old_trans.q.GetAngle());
      move_status_ = kValidateLegalMove;
      previous_legal_transform_ = old_trans;
    }

    return;
  }

  if (direction == Block::kMoveRight) {
    b2AABB bounding_box = moving_block_->GetBlockBox(this);

    // if block is already on right wall, do nothing
    if ((bounding_box.upperBound.x + 1.0) <= GetTotalNumCol()) {
      Block::Transform old_trans(*moving_block_);
      moving_block_->GetBody()->SetTransform(
          b2Vec2(old_trans.p.x + 1.0, old_trans.p.y),
          old_trans.q.GetAngle());
      move_status_ = kValidateLegalMove;
      previous_legal_transform_ = old_trans;
    }

    return;
  }

  if (direction == Block::kMoveDown) {
    moving_block_->GetBody()->SetLinearVelocity(b2Vec2(0,
        3.0 * GetExpectedBlockSpeed()));

    // ignore checking if block collision was correct if in disconnect mode
    if (is_tile_disconnected_mode_) {
      return;
    }

    // Checking block collides and stops properly
    move_status_ = kValidateLegalMove;
    return;
  }

  if (direction == Block::kRotate) {
    size_t times_rotated = moving_block_->GetTimesRotated();
    Block::Transform current_trans(*moving_block_);
    previous_legal_transform_ = current_trans;

    // Manually change center point of rotation (to 2,2) due to lack of
    // setter for changing the default (0, 0) point of rotation.
    // Tried using set mass data, but point of rotation still does not change
    if ((times_rotated % kRotatesForFullCircle) == 0) {
      current_trans.p.Set(current_trans.p.x, current_trans.p.y + 4.0f);
    } else if ((times_rotated % kRotatesForFullCircle == 1)) {
      current_trans.p.Set(current_trans.p.x + 4.0f, current_trans.p.y);
    } else if ((times_rotated % kRotatesForFullCircle == 2)) {
      current_trans.p.Set(current_trans.p.x, current_trans.p.y - 4.0f);
    } else {
      // rotated 3 times
      current_trans.p.Set(current_trans.p.x - 4.0f, current_trans.p.y);
    }

    moving_block_->GetBody()->SetTransform(
        current_trans.p, current_trans.q.GetAngle() - (M_PI / 2));
    move_status_ = kValidateLegalMove;
    moving_block_->SetTimesRotated(times_rotated + 1);
  }
}

void World::IllegalMoveCallBack(b2Body* other_body) {
  if (move_status_ == kDetectIllegal || move_status_ == kHitGround){
    return;
  }

  // only set previous transform when move is ok
  if (move_status_ == kMoveOk) {
    Block::Transform old_trans(*moving_block_);
    previous_legal_transform_ = old_trans;
  }

  move_status_ = kDetectIllegal;
  // block has hit the ground
  if (other_body == ground_floor_body_) {
    move_status_ = kHitGround;
  }
}

void World::RevertIllegalMove() {
  // destroy and create new moving block
  b2_world_->DestroyBody(moving_block_->GetBody());
  moving_block_ = block_generator_->CreateBlockByTemplate(
      this, moving_block_->GetTemplateId());
  moving_block_->GetBody()->SetTransform(previous_legal_transform_.p,
      previous_legal_transform_.q.GetAngle());
  moving_block_->SetTimesRotated(previous_legal_transform_.times_rotated_);
}

void World::CheckCompleteRow() {
  // Checking if a row was completed and remove if so
  for (size_t row = 0; row < floor_tile_array_.size(); row++) {
    size_t filled_tiles = 0;

    for (size_t col = 0; col < floor_tile_array_[row].size(); col++) {
      if (floor_tile_array_[row][col].color_ != cinder::Color::black()) {
        filled_tiles++;
      }
    }

    if (filled_tiles == floor_tile_array_[row].size()) {
      current_score_++;

      // reset row to empty and move to top
      for (size_t col = 0; col < floor_tile_array_[row].size(); col++) {
        floor_tile_array_[row][col].color_ = cinder::Color::black();
        floor_tile_array_[row][col].fixture_ = nullptr;
      }

      std::vector<Block::Tile> empty_row = floor_tile_array_[row];
      size_t moving_row = row;

      while (moving_row < (floor_tile_array_.size() - 1)) {
        floor_tile_array_[moving_row] = floor_tile_array_[moving_row + 1];
        moving_row++;
      }

      floor_tile_array_[floor_tile_array_.size() - 1] = empty_row;
      // recheck from new row
      row--;

      // Play sound for completing a row
      row_complete_sound_->start();
    }
  }
}

void World::SetCurrentGameState(GameState game_state) {
  current_game_state_ = game_state;
  Block::Tile tile(nullptr, cinder::Color::black());
  // default size for classic mode
  block_to_tile_width_ratio_ = 1;
  expected_block_speed_ = kDefaultBlockVerticalSpeed;

  if (current_game_state_ == kReloaded) {
    block_to_tile_width_ratio_ = 2;
    expected_block_speed_ = kDefaultBlockVerticalSpeed * 2.0;
  }

  total_num_row_ = kDefaultWorldNumRow * block_to_tile_width_ratio_;
  total_num_col_ = kDefaultWorldNumCol * block_to_tile_width_ratio_;

  // create 2d array containing blocks already fallen for reloaded mode
  for (size_t row = 0; row < kDefaultWorldNumRow * block_to_tile_width_ratio_;
      row++) {
    std::vector<Block::Tile> row_colors;
    for (size_t col = 0;
        col < kDefaultWorldNumCol * block_to_tile_width_ratio_; col++) {
      row_colors.push_back(tile);
    }
    floor_tile_array_.push_back(row_colors);
  }

  BuildGroundFloor();
  InitializeWalls();
}

void World::InitializeWalls() {
  // creating wall on left side of the game
  b2BodyDef left_wall_body_def;
  left_wall_body_def.position.Set(-5.0f, GetTotalNumRow() / 2.0);
  b2Body* left_wall_body = b2_world_->CreateBody(&left_wall_body_def);
  b2PolygonShape box_left_wall;
  box_left_wall.SetAsBox(5.0f, GetTotalNumRow() / 2.0);
  left_wall_body->CreateFixture(&box_left_wall, 0.0f);

  // creating wall on right side of the game
  b2BodyDef right_wall_body_def;
  right_wall_body_def.position.Set(
      GetTotalNumCol() + 5.0f, GetTotalNumRow() / 2.0);
  b2Body* right_wall_body = b2_world_->CreateBody(&right_wall_body_def);
  b2PolygonShape box_right_wall;
  box_right_wall.SetAsBox(5.0f, GetTotalNumRow() / 2.0);
  right_wall_body->CreateFixture(&box_right_wall, 0.0f);
}

void World::BlowUpSurroundingTiles(size_t row, size_t col) {
  Block::Tile exploded_tile;
  exploded_tile.color_ = cinder::Color::white();

  // blow up surrounding 3 by 3 blocks,
  // changing them into white color temporarily
  // using int due to row/col possibly being negative when subtracted
  for (int current_row = ((int) row - 1); current_row <= (row + 1);
      current_row++) {
    for (int current_col = ((int) col - 1); current_col <= (col + 1);
        current_col++) {
      if (current_row < 0 || current_row >= floor_tile_array_.size()
          || current_col < 0 || current_col > floor_tile_array_[0].size()) {
        continue;
      }

      floor_tile_array_[current_row][current_col] = exploded_tile;
    }
  }
}

void World::HandleBlockDroppingOnFloor() {
  // ingore a block hitting the ground too fast if in disconnect mode
  // otherwise fix the block from colliding too quickly by the physics engine
  bool is_block_finished = false;
  if (!is_tile_disconnected_mode_ && move_status_ == kHitGround) {
    if (moving_block_->GetBody()->GetLinearVelocity().y > 0) {
        // GetExpectedBlockSpeed() ) {
      is_block_finished = true;
    }

    RevertIllegalMove();
  }

  // reset the block if it has been hitting the floor too many times
  // occurs in disconnect mode when a block gets stuck while falling apart
  if (move_status_ == kHitGround || move_status_ == kDetectIllegal) {
    num_illegal_move_++;

    if (num_illegal_move_ > 20) {
      is_block_finished = true;
    }
  }

  // If blocks collide, both should stop moving and
  // Checking if no block is moving, then spawn new block
  if (moving_block_ != nullptr) {
    if ((moving_block_->GetBody()->GetLinearVelocity().y >
           (GetExpectedBlockSpeed() + 1)) || is_block_finished) {
      b2Fixture* fixture_list = moving_block_->GetBody()->GetFixtureList();

      for (b2Fixture* fixture = fixture_list; fixture != nullptr;
           fixture = fixture->GetNext()) {
        // Game logic code
        b2PolygonShape* shape =
            static_cast<b2PolygonShape*>(fixture->GetShape());
        b2AABB bound_box = fixture->GetAABB(0);
        b2Vec2 bottom_left = bound_box.lowerBound;
        size_t col = roundf(bottom_left.x);
        size_t row = roundf(bottom_left.y);

        // if a block reached the top of the screen, end the game
        if (row >= floor_tile_array_.size()
            || col >= floor_tile_array_[0].size()) {
          current_game_state_ = kEndScreen;
          return;
        }

        // check if shape was bomb, and should blow up surrounding
        // shapes in a 3 by 3 area
        if (moving_block_->GetTemplateId() == kBombId) {
          BlowUpSurroundingTiles(row, col);
          // bomb explosion sound
          bomb_explode_sound_->start();

          // add the tile in to the ground
        } else {
          Block::SetTileShapeAtColRow(shape, col, row);
          Block::Tile tile(fixture, moving_block_->GetColor());
          floor_tile_array_[row][col] = tile;
          // regular collide sound
          block_collide_sound_->start();
        }
      }

      b2_world_->DestroyBody(moving_block_->GetBody());
      moving_block_ = nullptr;

      // Check for a complete row, then build the floor, and spawn new block
      CheckCompleteRow();
      BuildGroundFloor();
      SpawnNewRandomBlock();
    }
  }
}

} // namespace tetris

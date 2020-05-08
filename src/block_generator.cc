//
// Created by Henrik Tseng on 4/19/20.
//

#include "physics/block_generator.h"

#include <Box2D/Dynamics/b2Fixture.h>
#include <cstdlib>
#include <random>

#include "physics/world.h"

namespace tetris {
BlockGenerator::BlockGenerator(bool is_bomb_mode) {
  // https://puzzling.stackexchange.com/questions/5100/mosaic-with-tetris-blocks
  // Shapes for classic mode defined in link above.
  // shape 0: 4 in a row
  {
    std::vector<b2Vec2> tile_list;
    tile_list.emplace_back(0,1);
    tile_list.emplace_back(1,1);
    tile_list.emplace_back(2,1);
    tile_list.emplace_back(3,1);
    classic_block_template_list_.emplace_back(tile_list,
        cinder::Color(0, 1, 1), 0);
  }

  // shape 1: Backwards L
  {
    std::vector<b2Vec2> tile_list;
    tile_list.emplace_back(1,1);
    tile_list.emplace_back(2,1);
    tile_list.emplace_back(3,1);
    tile_list.emplace_back(1,2);
    classic_block_template_list_.emplace_back(tile_list,
        cinder::Color(0, 0, 1), 1);
  }

  // shape 2: L shape
  {
    std::vector<b2Vec2> tile_list;
    tile_list.emplace_back(0,1);
    tile_list.emplace_back(1,1);
    tile_list.emplace_back(2,1);
    tile_list.emplace_back(2,2);
    classic_block_template_list_.emplace_back(tile_list,
        cinder::Color(1, .7, 1), 2);
  }

  // shape 3: 2 by 2 square
  {
    std::vector<b2Vec2> tile_list;
    tile_list.emplace_back(1,1);
    tile_list.emplace_back(2,1);
    tile_list.emplace_back(2,2);
    tile_list.emplace_back(1,2);
    classic_block_template_list_.emplace_back(tile_list,
        cinder::Color(1, 1, 0), 3);
  }

  // shape 4: Backwards Z
  {
    std::vector<b2Vec2> tile_list;
    tile_list.emplace_back(1,1);
    tile_list.emplace_back(2,1);
    tile_list.emplace_back(2,2);
    tile_list.emplace_back(3,2);
    classic_block_template_list_.emplace_back(tile_list,
        cinder::Color(0, 1, 0), 4);
  }

  // shape 5: T shape
  {
    std::vector<b2Vec2> tile_list;
    tile_list.emplace_back(1,1);
    tile_list.emplace_back(2,1);
    tile_list.emplace_back(3,1);
    tile_list.emplace_back(2,2);
    classic_block_template_list_.emplace_back(tile_list,
        cinder::Color(0.5, 0.0, 0.5), 5);
  }

  // shape 6: Z shape
  {
    std::vector<b2Vec2> tile_list;
    tile_list.emplace_back(1,1);
    tile_list.emplace_back(2,1);
    tile_list.emplace_back(2,0);
    tile_list.emplace_back(3,0);
    classic_block_template_list_.emplace_back(tile_list,
        cinder::Color(1, 0, 0), 6);
  }

  // Shapes for reloaded mode
  // Shape 0: one single half block
  {
    std::vector<b2Vec2> tile_list;
    tile_list.emplace_back(1.0,2.0);
    tile_list.emplace_back(2.0,2.0);
    reloaded_block_template_list_.emplace_back(tile_list,
        cinder::Color(1, 0, 0), 0);
  }

  // Shape 1:
  {
    std::vector<b2Vec2> tile_list;
    tile_list.emplace_back(0.0,2.0);
    tile_list.emplace_back(1.0,2.0);
    tile_list.emplace_back(2.0, 2.0);
    tile_list.emplace_back(2.0, 1.0);
    tile_list.emplace_back(3.0, 2.0);
    tile_list.emplace_back(3.0, 1.0);
    reloaded_block_template_list_.emplace_back(tile_list,
        cinder::Color(0, 1, 0), 1);
  }

  // Shape 2:
  {
    std::vector<b2Vec2> tile_list;
    tile_list.emplace_back(0.0,0.0);
    tile_list.emplace_back(1.0,0.0);
    tile_list.emplace_back(2.0, 0.0);
    tile_list.emplace_back(3.0, 0.0);
    tile_list.emplace_back(2.0, 1.0);
    tile_list.emplace_back(3.0, 1.0);
    tile_list.emplace_back(3.0, 2.0);
    tile_list.emplace_back(3.0, 3.0);
    reloaded_block_template_list_.emplace_back(tile_list,
        cinder::Color(0, 0, 1), 2);
  }

  // Shape 3:
  {
    std::vector<b2Vec2> tile_list;
    tile_list.emplace_back(0.0,0.0);
    tile_list.emplace_back(1.0,0.0);
    tile_list.emplace_back(2.0,0.0);
    tile_list.emplace_back(2.0,1.0);
    tile_list.emplace_back(2.0,2.0);
    tile_list.emplace_back(3.0,0.0);
    tile_list.emplace_back(3.0,1.0);
    tile_list.emplace_back(3.0,2.0);
    reloaded_block_template_list_.emplace_back(tile_list,
        cinder::Color(1, 0, 1), 3);
  }

  // Shape 7: Bomb. Used only in bomb mode
  {
    std::vector<b2Vec2> tile_list;
    tile_list.emplace_back(2.0,2.0);
    // Default grey color, will be changed randomly while in play
    bomb_ = Block(tile_list,
        cinder::Color(0.8,0.8,0.8), World::kBombId);
    // add bomb once only if in bomb mode
    if (is_bomb_mode) {
      classic_block_template_list_.push_back(bomb_);
    }
  }
}

Block* BlockGenerator::CreateRandomBlock(World* world) {
  // Make non-deterministic by
  // choose a random seed to spawn blocks
  std::random_device rd;
  size_t random_id = 0;

  // classic mode
  if (world->GetCurrentGameState() == World::kClassic) {
    std::uniform_int_distribution<int> dist(0, classic_block_template_list_.size() - 1);
    random_id = dist(rd);

  } else if (world->GetCurrentGameState() == World::kReloaded) {
    std::uniform_int_distribution<int> dist(0, reloaded_block_template_list_.size() - 1);
    random_id = dist(rd);
  }

  return CreateBlockByTemplate(world, random_id);
}

Block* BlockGenerator::CreateBlockByTemplate(World *world, size_t index_id) {
  // creating a dynamic body
  b2BodyDef body_def;
  body_def.type = b2_dynamicBody;
  body_def.position.Set(world->GetTotalNumCol() / 2.0, world->GetTotalNumRow());
  body_def.linearVelocity.Set(0.0f, world->GetExpectedBlockSpeed());
  b2Body* dynamic_body = world->GetB2World()->CreateBody(&body_def);
  dynamic_body->SetUserData(world);

  Block block;
  if (world->GetCurrentGameState() == World::kReloaded) {
    block = reloaded_block_template_list_[index_id];

    // only other option is classic mode
  } else {
    block = classic_block_template_list_[index_id];
  }

  for (b2Vec2 pos : block.GetTileList()) {
    b2PolygonShape shape;
    Block::SetTileShapeAtColRow(&shape, pos.x, pos.y);
    b2FixtureDef fixture_def;
    fixture_def.shape = &shape;
    fixture_def.density = 0.1f;
    fixture_def.friction = 0.0f;
    fixture_def.restitution = 0.0f;
    dynamic_body->CreateFixture(&fixture_def);
  }

  return new Block(block, dynamic_body);
}

} // namespace tetris
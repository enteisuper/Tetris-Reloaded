//
// Created by Henrik Tseng on 4/19/20.
//

#ifndef FINALPROJECT_BLOCK_GENERATOR_H
#define FINALPROJECT_BLOCK_GENERATOR_H

#include <Box2D/Collision/Shapes/b2PolygonShape.h>
#include <Box2D/Common/b2Math.h>
#include <Box2D/Dynamics/b2Body.h>

#include <vector>

#include "block.h"

namespace tetris {

class World;

/**
 * Holds lists that generate specific types of blocks for each mode
 */
class BlockGenerator {
 private:
  std::vector<Block> classic_block_template_list_;
  std::vector<Block> reloaded_block_template_list_;
  Block bomb_;

 public:
  BlockGenerator(bool is_bomb_mode);


  /**
   * Chooses and creates a random tetris block
   * @param world the world
   * @return a tetris block
   */
  Block* CreateRandomBlock(World* world);

  /**
   * Chooses and returns selected id tetris block
   * @param world the world
   * @param index_id template id for the block
   * @return tetris block
   */
  Block* CreateBlockByTemplate(World* world, size_t index_id);

  const std::vector<Block>& GetBlockTemplateList() const {
    return classic_block_template_list_;
  }
};

} // namespace tetris

#endif  // FINALPROJECT_BLOCK_GENERATOR_H

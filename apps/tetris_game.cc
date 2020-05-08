// Copyright (c) 2020 [Your Name]. All rights reserved.

#include "tetris_game.h"

#include <Box2D/Dynamics/b2Body.h>
#include <cinder/app/App.h>
#include <cinder/audio/Voice.h>
#include <cinder/gl/gl.h>

#include <strstream>

using cinder::app::KeyEvent;

namespace tetris {

// Audio file paths
constexpr const static char kBackgroundMusicName[] = "Tetris_Main_Theme.mp3";
constexpr const static char kEndingMusicName[] = "Tetris_Ending_Theme.mp3";
const char kNormalFont[] = "Arial";
const double kTextBoxWidth = 2.0;

TetrisGame::TetrisGame() : engine_() {}

void TetrisGame::DrawPolygonBlock(const Block* block) {
  // Color of the bomb changes randomly,
  // while other blocks remain the same color
  if (block->GetTemplateId() == World::kBombId) {
    float random_double = (float) random() / RAND_MAX;
    cinder::gl::color(random_double, random_double, random_double);
  } else {
    cinder::gl::color(block->GetColor());
  }

  for (const b2AABB& tile : block->GetBoundingBoxList()) {
    double tile_size = GetTileDisplaySize();
    cinder::Rectf rectangle(
        tile_size * (tile.lowerBound.x),
        tile_size *
            (engine_.GetWorld().GetTotalNumRow() - (tile.upperBound.y)),
        tile_size * (tile.upperBound.x),
        tile_size *
            (engine_.GetWorld().GetTotalNumRow() - (tile.lowerBound.y)));
    cinder::gl::drawSolidRect(rectangle);
  }
}

void TetrisGame::update() {
  engine_.GetWorld().Step();
}

void TetrisGame::draw() {
  cinder::gl::clear();

  // draw beginning options and modes
  if (engine_.GetCurrentGameState() == World::kChooseMode) {
    DrawStartScreen();
    return;
  }

  // draw ending options and scores
  if (engine_.GetCurrentGameState() == World::kEndScreen) {
    DrawEndingScreen();
    return;
  }

  // only draw blocks once a game is in progress and a block has spawned
  const Block* block = engine_.GetWorld().GetMovingBlock();
  if (block != nullptr) {
    b2Vec2 position = block->GetBody()->GetPosition();
    float angle = block->GetBody()->GetAngle();
    printf("%4.2f %4.2f %4.2f\n", position.x, position.y, angle);
    DrawPolygonBlock(block);
  }

  DrawFloor();
  DrawCurrentScore();
}

void TetrisGame::setup() {
  // setting up background music
  cinder::audio::SourceFileRef background_file = cinder::audio::load(
      cinder::app::loadAsset(kBackgroundMusicName));
  background_music_ = cinder::audio::Voice::create(background_file);
  background_music_->start();
  background_music_->setVolume(0.5);

  cinder::audio::SourceFileRef ending_file = cinder::audio::load(
      cinder::app::loadAsset(kEndingMusicName));
  ending_music_ = cinder::audio::Voice::create(ending_file);
  ending_music_->setVolume(0.5);
}

void TetrisGame::keyDown(KeyEvent event) {
  switch (event.getCode()) {
    // Choose game mode classic
    case KeyEvent::KEY_1: {
      if (engine_.GetCurrentGameState() == World::kChooseMode) {
        engine_.SetCurrentGameState(World::kClassic);
      }
      break;
    }

    // Choose game mode reloaded
    case KeyEvent::KEY_2: {
      if (engine_.GetCurrentGameState() == World::kChooseMode) {
        engine_.SetCurrentGameState(World::kReloaded);
      }
      break;
    }

    // Choose blitz mode
    case KeyEvent::KEY_3: {
      if (engine_.GetCurrentGameState() == World::kChooseMode) {
        engine_.SetCurrentGameState(World::kReloaded);
        engine_.GetWorld().SetIsTileDisconnectedMode(true);
      }
      break;
    }

    // Choose bomb mode
    case KeyEvent::KEY_4: {
      if (engine_.GetCurrentGameState() == World::kChooseMode) {
        engine_.SetCurrentGameState(World::kClassic);
        engine_.GetWorld().SetIsBombMode(true);
      }
      break;
    }

    // moves the block left
    case KeyEvent::KEY_LEFT: {
      engine_.Move(Block::kMoveLeft);
      break;
    }

    // moves the block right
    case KeyEvent::KEY_RIGHT: {
      engine_.Move(Block::kMoveRight);
      break;
    }

    // Speeds up the block's downward velocity
    case KeyEvent::KEY_DOWN: {
      engine_.Move(Block::kMoveDown);
      break;
    }

    // rotates block clockwise
    case KeyEvent::KEY_z: {
      engine_.Move(Block::kRotate);
      break;
    }

    // Exits and ends the game
    case KeyEvent::KEY_ESCAPE: {
      exit(EXIT_SUCCESS);
    }
  }
}

void TetrisGame::DrawStartScreen() {
  double canvas_width = GetCanvasWidth();
  double canvas_height = GetCanvasHeight();
  // Printing title
  cinder::Rectf title_rectangle(
      (canvas_width / kTextBoxWidth) - kTextBoxWidth,
      canvas_height / 8.0,
      (canvas_width / kTextBoxWidth) + kTextBoxWidth,
      canvas_height / 4.0);
  const cinder::vec2 position_title = {title_rectangle.getCenter().x,
                                       title_rectangle.getCenter().y};
  const cinder::ivec2 title_size =
      {canvas_width / 2, canvas_height / 8.0};
  std::strstream title_stream;
  title_stream << "Tetris Reloaded";
  PrintText(title_stream.str(), cinder::Color::white(),
            title_size, position_title, 32);

  // Printing controls
  cinder::Rectf control_rectangle(
      (canvas_width / kTextBoxWidth) - kTextBoxWidth,
      canvas_height / 4.0,
      (canvas_width / kTextBoxWidth) + kTextBoxWidth,
      canvas_height);
  const cinder::vec2 position_score = {control_rectangle.getCenter().x,
                                       control_rectangle.getCenter().y};
  const cinder::ivec2 control_size =
      {canvas_width * 0.55, canvas_height / 4.0};
  std::strstream control_stream;
  // extra spacing added for line separation
  control_stream
      << "Press 1 to play Classic Mode "
      << "Press 2 to play Reloaded Mode "
      << "Press 3 to play Disjointed Mode "
      << "Press 4 to play Bomb Mode     "
      << "     ---------------------------------      "
      << "Game Controls:                                  "
      << "z to rotate block                               "
      << "Left key to move block left                 "
      << "Right key to move block right "
      << "Down key to move block right ";
  PrintText(control_stream.str(), cinder::Color::white(),
            control_size, position_score, 18);
}

void TetrisGame::DrawEndingScreen() {
  // stop regular background music and play ending theme
  background_music_->stop();
  ending_music_->start();

  double canvas_width = GetCanvasWidth();
  double canvas_height = GetCanvasHeight();
  // Print game over
  cinder::Rectf ending_rectf(
      (canvas_width / kTextBoxWidth) - kTextBoxWidth,
      canvas_height / 4.0,
      (canvas_width / kTextBoxWidth) + kTextBoxWidth,
      canvas_height * 3.0 / 4.0);
  const cinder::vec2 position_title = {ending_rectf.getCenter().x,
                                       ending_rectf.getCenter().y};
  const cinder::ivec2 title_size =
      {canvas_width / 2, canvas_height / kTextBoxWidth};
  std::strstream game_over_stream;
  // extra spacing added for line separation
  game_over_stream << "Game Over     "
      << "Your final score: "
      << engine_.GetWorld().GetScore() << "       "
      <<  "     -----------------      "
      << "Press Esc to exit the game";
  PrintText(game_over_stream.str(), cinder::Color::white(),
            title_size, position_title, 40);
}

void TetrisGame::DrawFloor() {
  std::vector<std::vector<Block::Tile>>& tile_array =
      engine_.GetWorld().GetFloorTileArray();

  for (size_t row = 0; row < tile_array.size(); row++) {
    for (size_t col = 0; col < tile_array[row].size(); col++) {
      Block::Tile &tile = tile_array[row][col];

      // don't color null fixtures with black
      // draw white colored tiles to simulate explosion effect
      if ((tile.fixture_ == nullptr)
          && (tile.color_ != cinder::Color::white())) {
        continue;
      }

      cinder::gl::color(tile.color_);
      double adjust_height = 0.95;
      double adjust_offset = 0.05;
      double adjust_tile_to_display = GetTileDisplaySize();
      cinder::Rectf rectangle(
          adjust_tile_to_display * (col + adjust_offset),
          adjust_tile_to_display *
          (engine_.GetWorld().GetTotalNumRow() - (row + adjust_height)),
          adjust_tile_to_display * (col + adjust_height),
          adjust_tile_to_display *
          (engine_.GetWorld().GetTotalNumRow() - (row + adjust_offset)));
      cinder::gl::drawSolidRect(rectangle);

      // change color back to black for the white explosion to disappear
      // only want white color to show for 1 frame before dissapearing
      if (tile.color_ == cinder::Color::white()) {
        tile.color_ = cinder::Color::black();
      }
    }
  }
}

void TetrisGame::DrawCurrentScore() {
  double canvas_width = GetCanvasWidth();

  cinder::Rectf rectf((canvas_width / kTextBoxWidth) - kTextBoxWidth,
      kTextBoxWidth,
      (canvas_width / kTextBoxWidth) + kTextBoxWidth,
      kTextBoxWidth * kTextBoxWidth);

  // print value of food on top
  const cinder::vec2 position_score = {rectf.getCenter().x,
                                       rectf.getCenter().y};
  const cinder::ivec2 size_value = {80, 15};
  std::strstream strstream;
  strstream << "Score : "
            << std::to_string(engine_.GetWorld().GetScore());
  PrintText(strstream.str(), cinder::Color::white(),
            size_value, position_score, 15);
}

// Function is taken from snake
void TetrisGame::PrintText(const std::string& text, const cinder::Color& color,
    const cinder::ivec2& size, const cinder::vec2& loc,
    size_t font_size) {
  cinder::gl::color(color);
  auto box = cinder::TextBox()
      .alignment(cinder::TextBox::CENTER)
      .font(cinder::Font(kNormalFont, font_size))
      .size(size)
      .color(color)
      .backgroundColor(cinder::ColorA(0, 0, 0, 0))
      .text(text);

  const auto box_size = box.getSize();
  const cinder::vec2 locp =
      {loc.x - box_size.x / 2, loc.y - box_size.y / 2};
  const auto surface = box.render();
  const auto texture = cinder::gl::Texture::create(surface);
  cinder::gl::draw(texture, locp);

}

double TetrisGame::GetTileDisplaySize() const {
  return kDefaultBlockToDisplaySize / engine_.GetWorld().GetBlockToTileWidthRatio();
}


}  // namespace tetris

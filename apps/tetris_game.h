// Copyright (c) 2020 CS126SP20. All rights reserved.

#ifndef FINALPROJECT_APPS_MYAPP_H_
#define FINALPROJECT_APPS_MYAPP_H_

#include <cinder/app/App.h>
#include <tetris_engine.h>
#include <cinder/audio/Voice.h>

#include "physics/world.h"

namespace tetris {

/**
 * Main app that updates the game and logic as it runs.
 */
class TetrisGame : public cinder::app::App {
 public:
  static const int kDefaultBlockToDisplaySize = 50;

 private:
  TetrisEngine engine_;
  // Audio files
  cinder::audio::VoiceSamplePlayerNodeRef background_music_;
  cinder::audio::VoiceSamplePlayerNodeRef ending_music_;

  /**
   * Convert polyshape into a 4 x 4 format and draw in UI
   * @param block the current block to draw
   */
  void DrawPolygonBlock(const Block* block);

  /**
   * Draws the starting screen of the game
   */
  void DrawStartScreen();

  /**
   * Draws the floor
   */
  void DrawFloor();

  /**
   * Draws the score of the player at the top of the game board
   */
  void DrawCurrentScore();

  /**
   * Draws the ending screen of the game
   */
  void DrawEndingScreen();

  /**
   * Function taken from snake. Prints out the inputted text
   * @param text text to print
   * @param color color of text
   * @param size size of the text box
   * @param loc where to print
   * @param font_size font size
   */
  void PrintText(const std::string& text, const cinder::Color& color,
      const cinder::ivec2& size, const cinder::vec2& loc,
      size_t font_size);

  /**
   * Get the tile's display size
   * @return tile display size
   */
  double GetTileDisplaySize() const;

  double GetCanvasWidth() const {
    return engine_.GetWorld().GetTotalNumCol()
                          * GetTileDisplaySize();
  }

  double GetCanvasHeight() const {
    return engine_.GetWorld().GetTotalNumRow()
           * GetTileDisplaySize();
  }

 public:
  TetrisGame();
  void setup() override;
  void update() override;
  void draw() override;
  void keyDown(cinder::app::KeyEvent) override;
};

}  // namespace tetris

#endif  // FINALPROJECT_APPS_MYAPP_H_

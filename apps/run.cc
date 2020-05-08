// Copyright (c) 2020 [Your Name]. All rights reserved.

#include <cinder/app/App.h>
#include <cinder/app/RendererGl.h>

#include "tetris_game.h"

using cinder::app::App;
using cinder::app::RendererGl;


namespace tetris {
const int kSamples = 8;

void SetUp(App::Settings* settings) {
  double canvas_width = World::kDefaultWorldNumCol
      * TetrisGame::kDefaultBlockToDisplaySize;
  double canvas_height = World::kDefaultWorldNumRow
                        * TetrisGame::kDefaultBlockToDisplaySize;
  settings->setWindowSize(canvas_width, canvas_height);
  settings->setTitle("Tetris Reloaded");
}

}  // namespace tetris


// This is a macro that runs the application.
CINDER_APP(tetris::TetrisGame,
           RendererGl(RendererGl::Options().msaa(tetris::kSamples)),
           tetris::SetUp)

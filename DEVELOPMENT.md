# Development
 - **4/19/20** Wrote simple code to test physics library box2d
   - Create basic skeleton of app front and backend design
   - Connect to physics engine
   - Testing block collision support
 - **4/20/20** Fix collision between tetris blocks
   - Collision working properly  
   - Tests for world generation and world
 - **4/25/20** Fix collision between tetris blocks with input
   - When illegal input is given, the current moving block will
   not follow the input
   - Fix test errors from refactoring code
 - **4/26/20** Upon forming a row of blocks, the row will dissapear
   - The rest of the rows move down to replace the old row
 - **4/27/20** Fix rotation of blocks to be more natural and added a scoreboard
  with music/sound support
   - Change point of rotation and positioning of blocks to be easier 
   as the player to figure out
   - Score shows the number of rows cleared in the current game
   - Background tetris music plays
   - Sound is played when a block hits the ground or clears a row
 - **4/27/20** Fix test cases 
   - Change/refactor test cases to reflect the changes made to the code
   - Add test cases for new functionality like creating specific blocks
 - **5/2/20** Add begin/end/separate mode screen
   - Added a beginning screen when first compiling
   - Added an ending screen when the blocks hit the ceiling
   - Added distinction between class and reload mode using keys 1 and 2
 - **5/3/20** Improve physics engine and added third mode
   - When hitting a block at the edge, block no longer spins out of
   control, instead locking into place
   - Added third mode(disjointed mode), where block collision is disjoint, allowing for 
   spinning of the tiles, eventually landing on the floor/wall
   - Adjusted speed overall in the game to allow for more fluid gameplay,
   not having a large disparity of speed between class and reloaded mode
 - **5/4/20** Added 4th mode, bomb mode
   - Played in classic mode, but with additional bomb
   - Bomb explodes surrounding 3 by 3 tiles when hitting the floor
   - Bomb color changes from white to grey randomly
 - **5/5/20** Adjusted spawn rates and fixed bugs
   - Bomb now spawns less often so player will be able to complete a row more easily than before
   - Bomb explodes, leaving behind 1 frame of white tiles to show the explosion radius
   - Fix randomness by changing to c++ 11 random for non-deterministic order
   - Add more tests to check engine functions and world functions
   - Add additioal soundtrack that plays at the end of a game
   - When bomb collides, a new sound effect of an explosion plays
   - Fix rotate bug where rotating against a wall caused non-intended effects 
   when attempting to rotate after
 - **5/6/20** Add more tests and refactored code
   - Moved code around for readability
   - Fix bug where blocks would stick to walls when performing an illegal move
---

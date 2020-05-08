# Final Project

[![license](https://img.shields.io/badge/license-MIT-green)](LICENSE)
[![docs](https://img.shields.io/badge/docs-yes-brightgreen)](docs/README.md)

Read [this document](https://cliutils.gitlab.io/modern-cmake/chapters/basics/structure.html) to understand the project
layout.

# [Tetris Reloaded]

Author: [Henrik Tseng] - hyt2@illinois.edu

## General Information/Ideas

This project pulls a spin on the standard arcade game Tetris. Gone are the old blocks, replaced by new additions, with additional modes with new blocks! Experience the classic again, with new blocks and modes that gives a new experience with similar mechanics! Half blocks now may randomly spawn depending on the mode, attached to blocks to form new shapes, increasing the difficulty, and providing a brand new experience.


## Build/Dependency

The project is built on CMake files, which will be nessessary to run the project. CMake already comes along with CLion, but can be manually installed [here](https://cmake.org/install/), along with instructions [here](https://cmake.org/cmake/help/latest/guide/tutorial/index.html).

This project was built through C++ on MacOS with CLion. The program utilizes cinder and the cinderblock Box2d. Cinder will be needed to run the game, which can be downloaded [here](https://libcinder.org/) with instructions. The cinderblock will automatically be added once compiled.

## How To Play
At the start, the player chooses between 4 modes, classic, reloaded, disconnected. 
(1) Classic mode utilizes the standard blocks of tetris.
(2) Reloaded mode uses different blocks blocks made up of half sized and regular tiles incuded for extra difficulty, which is also played on a larger board.
(3) Disconnected mode plays using reloaded blocks, but limits the collision of blocks, allowing fast falling blocks to collide, splitting apart and continue falling to the ground.
(4) Bomb mode plays like classic mode, but with an addition of a bomb block, which explodes and removes nearby blocks when colliding with the ground.

## Key Inputs

| Key         | Action                                                                         |
|------------ |--------------------------------------------------------------------------------|
| `1`         | Start game in default tetris with regular blocks                               |
| `2`         | Start game with half blocks for added difficulty                               |
| `3`         | Start disconnected game for loose block collision, falling into fragments      |
| `4`         | Start game in default tetris with regular blocks plus a bomb that explodes nearby blocks when hitting the ground |
| `z`         | Rotate tetris block clockwise                                                  |
|`Left Arrow` | Moves block to the left                                                        |
|`Right Arrow`| Moves block to the Right                                                       |
|`Down Arrow` | Increase block's downward velocity                                             |
| `Escape`    | Exits the game                                                                 |

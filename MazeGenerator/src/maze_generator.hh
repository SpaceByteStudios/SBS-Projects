#pragma once

#include "maze.hh"
#include "maze_renderer.hh"

void generate_depth_first_maze(Maze &maze);
void animate_generate_depth_first_maze(MazeRenderer &renderer, Maze &maze);

#pragma once

#include "maze.hh"
#include "maze_renderer.hh"

void solve_depth_first_maze(Maze &maze);
void animate_solve_depth_first_maze(MazeRenderer &renderer, Maze &maze);

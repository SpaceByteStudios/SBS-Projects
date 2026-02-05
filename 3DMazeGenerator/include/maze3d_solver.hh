#pragma once

#include "maze3d.hh"
#include "maze3d_renderer.hh"

void solve_random_walk_maze(Maze3D& maze);

void solve_depth_first_maze(Maze3D& maze);
void animate_solve_depth_first_maze(Maze3DRenderer& renderer, Maze3D& maze);

void solve_breadth_first_maze(Maze3D& maze);

void solve_dijkstra_maze(Maze3D& maze);

void solve_astar_maze(Maze3D& maze);

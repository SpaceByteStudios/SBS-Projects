#pragma once

#include "maze.hh"
#include "maze_renderer.hh"

void solve_random_walk_maze(Maze& maze);

void solve_wall_follower_maze(bool follow_right, Maze& maze);

void solve_depth_first_maze(Maze& maze);
void animate_solve_depth_first_maze(MazeRenderer& renderer, Maze& maze);

void solve_breadth_first_maze(Maze& maze);

void solve_dijkstra_maze(Maze& maze);

void solve_astar_maze(Maze& maze);

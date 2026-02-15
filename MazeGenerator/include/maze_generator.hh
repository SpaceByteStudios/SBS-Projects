#pragma once

#include <SFML/System/Vector2.hpp>

#include "maze.hh"
#include "maze_renderer.hh"

void generate_depth_first_maze(Maze& maze);
void generate_depth_first_maze_log(Maze& maze);
void animate_generate_depth_first_maze(MazeRenderer& renderer, Maze& maze);

void generate_chunked_depth_first_maze(Maze& maze, int chunk_size);

void generate_prim_maze(Maze& maze);
void generate_kruskal_maze(Maze& maze);

void generate_wilson_maze(Maze& maze);
void animate_generate_wilson_maze(MazeRenderer& renderer, Maze& maze);

void generate_recursive_division_maze(Maze& maze);
void animate_generate_recursive_division_maze(MazeRenderer& renderer, Maze& maze);

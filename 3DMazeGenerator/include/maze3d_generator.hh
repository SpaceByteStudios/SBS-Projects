#pragma once

#include <SFML/System/Vector2.hpp>

#include "maze3d.hh"
#include "maze3d_renderer.hh"

void generate_depth_first_maze(Maze3D& maze);
void animate_generate_depth_first_maze(Maze3DRenderer& renderer, Maze3D& maze);

void generate_chunked_depth_first_maze(Maze3D& maze, int chunk_size);

void generate_prim_maze(Maze3D& maze);
void generate_kruskal_maze(Maze3D& maze);

void generate_wilson_maze(Maze3D& maze);
void animate_generate_wilson_maze(Maze3DRenderer& renderer, Maze3D& maze);

void recursive_divide_grid(Maze3D& maze, sf::Vector2u& pos, sf::Vector2u& dim);
void generate_recursive_division_maze(Maze3D& maze);

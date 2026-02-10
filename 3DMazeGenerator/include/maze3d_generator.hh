#pragma once

#include <SFML/System/Vector2.hpp>

#include "maze3d.hh"
#include "maze3d_renderer.hh"

void generate_depth_first_maze(Maze3D& maze);
void animate_generate_depth_first_maze(Maze3DRenderer& renderer, Maze3D& maze);

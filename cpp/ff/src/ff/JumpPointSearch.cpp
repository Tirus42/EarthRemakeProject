#include "ff/JumpPointSearch.h"

#include <list>
#include <map>
#include <set>

#include "ff/MinHeap.h"
#include "e2150/Map.h"

namespace ff {

// Diagonal:
// 670 | 6 is forced <=> !(Enterable(6) => Enterable(5))
// 5x1 | 6 is only forced when 6 is enterable, but not 5.
// /32 | Same for 2 and 3: !(Enterable(2) => Enterable(3))
//
// Streight:
// 567 | 7 is forced <=> !(Enterable(7) => Enterable(6))
// -x0 | 7 is only forced when 7 is enterable, but not 6.
// 321 | Same for 1 and 2: !(Enterable(1) => Enterable(2))
dir_set_t JumpPointSearch::ForcedNeighbours(coord_t coord, dir_t dir) {
  if (dir == NO_DIRECTION) {
    // This is probaly the first step and this has no direction.
    return 0;
  }

  dir_set_t dirs = 0;
  if (DirectionIsDiagonal(dir)) {
    coord_t new_coord_1 = Move(coord, Rotate(dir, 6));
    coord_t new_coord_2 = Move(coord, Rotate(dir, 5));
    if (!Implies(map.isFieldFree(map.position(new_coord_1.x, new_coord_1.y)),
                 map.isFieldFree(map.position(new_coord_2.x, new_coord_2.y)))) {
      AddDirectionToSet(dirs, Rotate(dir, 6));
    }
    new_coord_1 = Move(coord, Rotate(dir, 2));
    new_coord_2 = Move(coord, Rotate(dir, 3));
    if (!Implies(map.isFieldFree(map.position(new_coord_1.x, new_coord_1.y)),
                 map.isFieldFree(map.position(new_coord_2.x, new_coord_2.y)))) {
      AddDirectionToSet(dirs, Rotate(dir, 2));
    }
  } else {
    coord_t new_coord_1 = Move(coord, Rotate(dir, 7));
    coord_t new_coord_2 = Move(coord, Rotate(dir, 6));
    if (!Implies(map.isFieldFree(map.position(new_coord_1.x, new_coord_1.y)),
                 map.isFieldFree(map.position(new_coord_2.x, new_coord_2.y)))) {
      AddDirectionToSet(dirs, Rotate(dir, 7));
    }
    new_coord_1 = Move(coord, Rotate(dir, 1));
    new_coord_2 = Move(coord, Rotate(dir, 2));
    if (!Implies(map.isFieldFree(map.position(new_coord_1.x, new_coord_1.y)),
                 map.isFieldFree(map.position(new_coord_2.x, new_coord_2.y)))) {
      AddDirectionToSet(dirs, Rotate(dir, 1));
    }
  }
  
  return dirs;
}

// In case of a streight move:
// 567 |
// -x0 | 0 is the only natural neighbour here.
// 321 |
// 
// Diagonal move:
// 670 |
// 5x1 | In this case 0, 1 and 7 are natural neighbours.
// /32 |
dir_set_t JumpPointSearch::NaturalNeighbours(coord_t coord, dir_t dir) {
  if (dir == NO_DIRECTION) {
    // If no direction was specified, happens on the first move,
    // All directions are natural neighbours
    return 255;
  }
  
  dir_set_t dirs = 0;
  
  coord_t new_coord = Move(coord, dir);
  if (map.isFieldFree(map.position(new_coord.x, new_coord.y))) {
    AddDirectionToSet(dirs, dir);
  }
  if (DirectionIsDiagonal(dir)) {
    new_coord = Move(coord, Rotate(dir, 1));
    if (map.isFieldFree(map.position(new_coord.x, new_coord.y))) {
      AddDirectionToSet(dirs, Rotate(dir, 1));
    }
    new_coord = Move(coord, Rotate(dir, 7));
    if (map.isFieldFree(map.position(new_coord.x, new_coord.y))) {
      AddDirectionToSet(dirs, Rotate(dir, 7));
    }
  }
  
  return dirs;
}

coord_t JumpPointSearch::Jump(coord_t pos, dir_t dir, coord_t goal) {
  coord_t new_pos = Move(pos, dir);

  // Reached a wall or the end of the grid.
  if (map.isFieldFree(map.position(new_pos.x, new_pos.y))) {
    return -1;
  }

  // Check if this node has one or more forced neighbour.
  // If so this is a jump point.
  if (ForcedNeighbours(new_pos, dir)) {
    //std::cout << "Blubb: " << (int) ForcedNeighbours(new_pos, dir) << std::endl;
    return new_pos;
  }

  // Check if we reached the goal.
  if (new_pos.index == goal.index) { return new_pos; }

  //if (grid_->GetFieldColor(new_pos.x, new_pos.y) == sf::Color::Black)
  //  grid_->SetFieldColor(new_pos.x, new_pos.y, sf::Color(0, 0, 125));

  // Check if it is a diagonal move. Once again the example:
  // 670 | If it's a diagonal move, we jump
  // 5x1 | to direction 1 and 7 and check if
  // /32 | one of the jumps returns a valid jump point.
  if (DirectionIsDiagonal(dir)) {
    coord_t new_new_pos = Jump(new_pos, Rotate(dir, 1), goal);
    if (new_new_pos.index != NO_COORD) { return new_pos; }
    new_new_pos = Jump(new_pos, Rotate(dir, 7), goal);
    if (new_new_pos.index != NO_COORD) { return new_pos; }
  }

  // If nothing of the above applies, jump further in the same direction.
  return Jump(new_pos, dir, goal);
}

bool JumpPointSearch::Solve(std::list<coord_t>& out_list,
                            uint16_t start_x, uint16_t start_y,
                            uint16_t goal_x, uint16_t goal_y, bool use_astar) {
  BinaryMinHeap open_list;
  std::set<uint32_t> closed_list;
  std::map<uint32_t, uint32_t> parent;
  coord_t start(start_x, start_y);
  coord_t goal(goal_x, goal_y);
  coord_t last_pos(start.index);
  dir_t last_direction = NO_DIRECTION;
  
  open_list.Insert(start.index, 0.f, EstDistance(start, goal));
  while (!open_list.IsEmpty()) {
    float cost = open_list.MinCost();
    coord_t coord = open_list.ExtractMin();

    if (last_pos.index != start.index) {
      last_direction = GetDirection(coord_t(parent[coord.index]), coord);
    }
    last_pos.index = coord.index;

    // Check if we reached the goal
    if (coord.index == goal.index) {
      coord_t par_coord(NO_COORD);
      while (parent.find(coord.index) != parent.end()) {
        par_coord.index = parent[coord.index];
        while (coord.index != par_coord.index) {
          out_list.push_front(coord);
          coord = Move(coord, GetDirection(coord, par_coord));
        }
      }

      return true;
    }
    
    closed_list.insert(coord.index);

    // The directions to jump to are the natural combined with the
    // forced neighbours.
    dir_set_t dirs;
    if (!use_astar) {
      dirs = NaturalNeighbours(coord, last_direction) |
             ForcedNeighbours(coord, last_direction);
    } else {
      dirs = 0xff;
    }
                      
    dir_t dir = NextDirectionInSet(dirs);
    while (dir != NO_DIRECTION) {
      coord_t jump_point(NO_COORD);
      if (!use_astar) {
        jump_point = Jump(coord, dir, goal);
      } else {
        jump_point = Move(coord, dir);
        if (!map.isFieldFree(map.position(jump_point.x, jump_point.y))) {
          dir = NextDirectionInSet(dirs);
          continue;
        }
      }
      // Check for valid jump point.
      if (jump_point.index != NO_COORD) {
        if (closed_list.find(jump_point.index) != closed_list.end()) {
          // Already in the closed list -> Skip this direction
          dir = NextDirectionInSet(dirs);
          continue;
        }
        
        // Calculate the new precise path length from the start to this point.
        float new_cost = cost + PrecDistance(coord, jump_point);
        float new_est_cost_to_goal = new_cost + EstDistance(jump_point, goal);
        
        // Now check if the node is already in the open list
        // and we can update the cost.
        int ret = open_list.UpdateCost(jump_point.index, new_cost, new_est_cost_to_goal);
        if (ret < 0) {
          // Already in the heap and cost not lower, so skip
          dir = NextDirectionInSet(dirs);
          continue;
        }
        else if (ret == 0) {
          // Node not in the heap yet, add it now
          open_list.Insert(jump_point.index, new_cost, new_est_cost_to_goal);
        }
        
        parent[jump_point.index] = coord.index;
      }
      dir = NextDirectionInSet(dirs);

    }
  }
  // No path found
  return false;
}

} // namespace ff

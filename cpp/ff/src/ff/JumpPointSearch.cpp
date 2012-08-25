#include "ff/JumpPointSearch.h"

#include <iostream>
#include <list>
#include <map>
#include <set>

#include "ff/MinHeap.h"

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
dir_set_t JumpPointSearch::ForcedNeighbours(uint32_t coord, dir_t dir) {
  if (dir == NO_DIRECTION) {
    // This is probaly the first step and this has no direction.
    return 0;
  }

  dir_set_t dirs = 0;
  if (DirectionIsDiagonal(dir)) {
    if (!Implies(map.isFieldWalkable(coord, Rotate(dir, 6)),
                 map.isFieldWalkable(coord, Rotate(dir, 5)))) {
      AddDirectionToSet(dirs, Rotate(dir, 6));
    }
    if (!Implies(map.isFieldWalkable(coord, Rotate(dir, 2)),
                 map.isFieldWalkable(coord, Rotate(dir, 3)))) {
      AddDirectionToSet(dirs, Rotate(dir, 2));
    }
  } else {
    if (!Implies(map.isFieldWalkable(coord, Rotate(dir, 7)),
                 map.isFieldWalkable(coord, Rotate(dir, 6)))) {
      AddDirectionToSet(dirs, Rotate(dir, 7));
    }
    if (!Implies(map.isFieldWalkable(coord, Rotate(dir, 1)),
                 map.isFieldWalkable(coord, Rotate(dir, 2)))) {
      AddDirectionToSet(dirs, Rotate(dir, 1));
    }
  }
  
  //std::cout << "Forced neighbours: " << std::hex << (int) dirs << std::dec << std::endl;
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
dir_set_t JumpPointSearch::NaturalNeighbours(uint32_t coord, dir_t dir) {
  if (dir == NO_DIRECTION) {
    // If no direction was specified, happens on the first move,
    // All directions are natural neighbours
    return 255;
  }
  
  dir_set_t dirs = 0;
  
  if (map.isFieldWalkable(coord, dir)) {
    AddDirectionToSet(dirs, dir);
  }
  if (DirectionIsDiagonal(dir)) {
    dir_t new_dir = Rotate(dir, 1);
    if (map.isFieldWalkable(coord, new_dir)) {
      AddDirectionToSet(dirs, new_dir);
    }
    new_dir = Rotate(dir, 7);
    if (map.isFieldWalkable(coord, new_dir)) {
      AddDirectionToSet(dirs, new_dir);
    }
  }
  
  //std::cout << "Natural neighbours: " << std::hex << (int) dirs << std::dec << std::endl;
  return dirs;
}

uint32_t JumpPointSearch::Jump(uint32_t pos, dir_t dir, uint32_t goal) {
  uint32_t new_pos = Move(pos, dir);
  //std::cout << std::endl << "Trying jump to " << new_pos.x << ":" << new_pos.y << " ";

  // Reached a wall or the end of the grid.
  if (!map.isFieldWalkable(pos, dir)) {
    //std::cout << "wall!" << std::endl;
    return -1;
  }

  // Check if this node has one or more forced neighbour.
  // If so this is a jump point.
  if (ForcedNeighbours(new_pos, dir)) {
    return new_pos;
  }

  // Check if we reached the goal.
  if (new_pos == goal) {
    //std::cout << "goal!" << std::endl;
    return new_pos;
  }

  //if (grid_->GetFieldColor(new_pos.x, new_pos.y) == sf::Color::Black)
  //  grid_->SetFieldColor(new_pos.x, new_pos.y, sf::Color(0, 0, 125));

  // Check if it is a diagonal move. Once again the example:
  // 670 | If it's a diagonal move, we jump
  // 5x1 | to direction 1 and 7 and check if
  // /32 | one of the jumps returns a valid jump point.
  if (DirectionIsDiagonal(dir)) {
    uint32_t new_new_pos = Jump(new_pos, Rotate(dir, 1), goal);
    if (new_new_pos != NO_COORD) { return new_pos; }
    new_new_pos = Jump(new_pos, Rotate(dir, 7), goal);
    if (new_new_pos != NO_COORD) { return new_pos; }
  }

  // If nothing of the above applies, jump further in the same direction.
  return Jump(new_pos, dir, goal);
}

bool JumpPointSearch::Solve(uint32_t start, uint32_t goal,
                            std::list<uint32_t>& path_list) {
  bool use_astar = false;
  BinaryMinHeap open_list;
  std::set<uint32_t> closed_list;
  std::map<uint32_t, uint32_t> parent;
  bool first_pos = true;
  dir_t last_direction = NO_DIRECTION;
  
  open_list.Insert(start, 0.f, EstDistance(map.positionX(start),
											map.positionY(start),
											map.positionX(goal),
											map.positionY(goal)));
  while (!open_list.IsEmpty()) {
    float cost = open_list.MinCost();
    uint32_t coord = open_list.ExtractMin();
    //std::cout << "Checking field " << coord.x << ":" << coord.y << std::endl;

    if (!first_pos) {
	  uint32_t par_coord = parent[coord];
      last_direction = GetDirection(map.positionX(par_coord), map.positionY(par_coord),
									map.positionX(coord), map.positionY(coord));
    } else {
		first_pos = false;
    }

    // Check if we reached the goal
    if (coord == goal) {
      uint32_t par_coord(NO_COORD);
      while (parent.find(coord) != parent.end()) {
        par_coord = parent[coord];
        while (coord != par_coord) {
          path_list.push_front(coord);
          coord = Move(coord, GetDirection(map.positionX(coord),
                                           map.positionY(coord),
										   map.positionX(par_coord),
										   map.positionY(par_coord)));
        }
      }
      return true;
    }
    
    closed_list.insert(coord);

    // The directions to jump to are the natural combined with the
    // forced neighbours.
    dir_set_t dirs;
    if (!use_astar) {
      dirs = NaturalNeighbours(coord, last_direction) |
             ForcedNeighbours(coord, last_direction);
    } else {
      dirs = 0xff;
    }
    
    //std::cout << "Directions to check: " << std::hex << (int) dirs << std::dec << std::endl;
                      
    dir_t dir = NextDirectionInSet(dirs);
    while (dir != NO_DIRECTION) {
      uint32_t jump_point(NO_COORD);
      if (!use_astar) {
        jump_point = Jump(coord, dir, goal);
      } else {
        jump_point = Move(coord, dir);
        if (!map.isFieldWalkable(coord, dir)) {
          dir = NextDirectionInSet(dirs);
          continue;
        }
      }

      //std::cout << "Jumped into direction " << std::hex << (int) dir << std::dec << " to point " << jump_point.x << ":" << jump_point.y << std::endl;

      // Check for valid jump point.
      if (jump_point != NO_COORD) {
        if (closed_list.find(jump_point) != closed_list.end()) {
          // Already in the closed list -> Skip this direction
          dir = NextDirectionInSet(dirs);
          continue;
        }
        
        // Calculate the new precise path length from the start to this point.
        float new_cost = cost + PrecDistance(map.positionX(coord), map.positionY(coord),
                                             map.positionX(jump_point), map.positionY(jump_point));
        float new_est_cost_to_goal = new_cost + EstDistance(
            map.positionX(jump_point), map.positionY(jump_point),
            map.positionX(goal), map.positionY(goal));
        
        // Now check if the node is already in the open list
        // and we can update the cost.
        int ret = open_list.UpdateCost(jump_point, new_cost, new_est_cost_to_goal);
        if (ret < 0) {
          // Already in the heap and cost not lower, so skip
          dir = NextDirectionInSet(dirs);
          continue;
        }
        else if (ret == 0) {
          // Node not in the heap yet, add it now
          open_list.Insert(jump_point, new_cost, new_est_cost_to_goal);
        }
        
        parent[jump_point] = coord;
      }
      dir = NextDirectionInSet(dirs);

    }
  }
  // No path found
  return false;
}

} // namespace ff

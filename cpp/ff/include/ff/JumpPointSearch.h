#ifndef FF_JUMP_POINT_SEARCH_H_INCLUDED
#define FF_JUMP_POINT_SEARCH_H_INCLUDED

#include <list>

#include <inttypes.h>
#include <math.h>
#include <stdlib.h>

#include "e2150/Map.h"

namespace ff {

// C structure for easy and fast conversation between a node index and a position
struct coord_t {
  coord_t() : index((uint32_t) -1) {}
  coord_t(uint32_t _index) : index(_index) {}
  coord_t(uint16_t _x, uint16_t _y) : x(_x), y(_y) {}
  
  union {
    struct {
      uint16_t x;
      uint16_t y;
    };
    uint32_t index;
  };
};

// Uses the following bit order:
// North, NorthEast, East, SouthEast, South, SouthWest, West, NorthWest
typedef uint8_t dir_t;
typedef uint8_t dir_set_t;

class JumpPointSearch {
 public:
  JumpPointSearch(const Map& _map) : map(_map) {}
  
  bool Solve(std::list<coord_t>& out_list,
             uint16_t start_x, uint16_t start_y,
             uint16_t goal_x, uint16_t goal_y,
             bool use_astar = false);

 private:
  // Use the "9th" direction to indicate an invalid direction.
  static const dir_t NO_DIRECTION = 255;
  static const uint32_t NO_COORD = (uint32_t) -1;

  // Calculates an estimated distance from one point to another
  // Using the Chebyshev distance.
  inline static float EstDistance(coord_t from, coord_t to) {
    return fmaxf(abs(from.x - to.x), abs(from.y - to.y));
  }
  
  // Calculates the precise distance between two points assuming there are
  // only streight and 45deg diagonale moves.
  inline static float PrecDistance(coord_t from, coord_t to) {
    if (from.x != to.x && from.y != to.y) {
      // It is a diagonal move, so calculate the number of moves and
      // multiply it with sqrt(2) ~ 1.14142
      return (abs(from.x - to.x) * 1.14142f);
    } else {
      // Add the differences between the x and y coordinates.
      // 1 of them is always zero but we don't have to add an additional if.
      return (abs(from.x - to.x) + abs(from.y - to.y));
    }
  }

  // Checks if the given direction is diagonal
  inline static bool DirectionIsDiagonal(dir_t dir) {
    return (dir & 0xaa); // 0b10101010
  }

  // Removes and returns the next (clockwise order) direction from
  // the direction set.
  inline static dir_t NextDirectionInSet(dir_set_t& dir_set) {
    if (dir_set & Map::NORTH) { dir_set ^= Map::NORTH; return Map::NORTH; }
    if (dir_set & Map::NORTH_EAST) { dir_set ^= Map::NORTH_EAST; return Map::NORTH_EAST; }
    if (dir_set & Map::EAST) { dir_set ^= Map::EAST; return Map::EAST; }
    if (dir_set & Map::SOUTH_EAST) { dir_set ^= Map::SOUTH_EAST; return Map::SOUTH_EAST; }
    if (dir_set & Map::SOUTH) { dir_set ^= Map::SOUTH; return Map::SOUTH; }
    if (dir_set & Map::SOUTH_WEST) { dir_set ^= Map::SOUTH_WEST; return Map::SOUTH_WEST; }
    if (dir_set & Map::WEST) { dir_set ^= Map::WEST; return Map::WEST; }
    if (dir_set & Map::NORTH_WEST) { dir_set ^= Map::NORTH_WEST; return Map::NORTH_WEST; }

    // The direction set is empty
    return NO_DIRECTION;
  }

  // Adds a direction to the set. Does not check if the direction is valid.
  inline static void AddDirectionToSet(dir_set_t& dir_set, dir_t dir) {
    dir_set |= dir;
  }

  // Logical implication
  inline static bool Implies(bool a, bool b) {
    return a ? b : true;
  }

  // Moves one step in the given direction
  inline static coord_t Move(coord_t coord, dir_t dir) {
    switch (dir) {
      case Map::NORTH: return coord_t(coord.x, coord.y - 1);
      case Map::NORTH_EAST: return coord_t(coord.x + 1, coord.y - 1);
      case Map::EAST: return coord_t(coord.x + 1, coord.y);
      case Map::SOUTH_EAST: return coord_t(coord.x + 1, coord.y + 1);
      case Map::SOUTH: return coord_t(coord.x, coord.y + 1);
      case Map::SOUTH_WEST: return coord_t(coord.x - 1, coord.y + 1);
      case Map::WEST: return coord_t(coord.x - 1, coord.y);
      case Map::NORTH_WEST: return coord_t(coord.x - 1, coord.y - 1);
    }

    return coord_t(NO_COORD);
  }

  // Returns the direction of move.
  inline static dir_t GetDirection(coord_t from, coord_t to) {
    if (from.x == to.x) {
      if (from.y == to.y) { return NO_DIRECTION; }
      else if (from.y < to.y) { return Map::SOUTH; }
      else { return Map::NORTH; }
    } else if (from.x < to.x) {
      if (from.y == to.y) { return Map::EAST; }
      else if (from.y < to.y) { return Map::SOUTH_EAST; }
      else { return Map::NORTH_EAST; }
    } else {
      if (from.y == to.y) { return Map::WEST; }
      else if (from.y < to.y) { return Map::SOUTH_WEST; }
      else { return Map::NORTH_WEST; }
    }
  }

  // Rotates the given direction. Can only rotate clockwise and
  // not further than one complete rotation (angle = 8)
  inline static dir_t Rotate(dir_t dir, uint8_t angle) {
    return ((dir << angle) | (dir >> (8 - angle)));
  }

  // Returns the forced neighbours of the position with the
  // given direction.
  dir_set_t ForcedNeighbours(coord_t coord, dir_t dir);

  // Returns the natural neighbours of the position with the
  // given direction. Natural neighborus can't be pruned
  // and are therforce forced to check.
  dir_set_t NaturalNeighbours(coord_t coord, dir_t dir);

  // The Jump algorithm.
  // It will jump into one direction as long as no jump point has been reached.
  coord_t Jump(coord_t coord, dir_t dir, coord_t goal);

  // The map we operate on.
  const Map& map;
};

} // namespace ff

#endif // JUMP_POINT_SEARCH_H_INCLUDED

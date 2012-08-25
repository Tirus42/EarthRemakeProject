#ifndef FF_JUMP_POINT_SEARCH_H_INCLUDED
#define FF_JUMP_POINT_SEARCH_H_INCLUDED

#include <list>

#include <inttypes.h>
#include <math.h>
#include <stdlib.h>

// Forward decleration of the map used.
class Map;

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
  static const dir_t NO_DIRECTION = 8;
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
    return (dir % 2) != 0;
  }

  // Removes and returns the next (clockwise order) direction from
  // the direction set.
  inline static dir_t NextDirectionInSet(dir_set_t& dir_set) {
    if (dir_set & (1 << 0)) { dir_set ^= (1 << 0); return 0; }
    if (dir_set & (1 << 1)) { dir_set ^= (1 << 1); return 1; }
    if (dir_set & (1 << 2)) { dir_set ^= (1 << 2); return 2; }
    if (dir_set & (1 << 3)) { dir_set ^= (1 << 3); return 3; }
    if (dir_set & (1 << 4)) { dir_set ^= (1 << 4); return 4; }
    if (dir_set & (1 << 5)) { dir_set ^= (1 << 5); return 5; }
    if (dir_set & (1 << 6)) { dir_set ^= (1 << 6); return 6; }
    if (dir_set & (1 << 7)) { dir_set ^= (1 << 7); return 7; }

    // The direction set is empty
    return NO_DIRECTION;
  }

  // Adds a direction to the set. Does not check if the direction is valid.
  inline static void AddDirectionToSet(dir_set_t& dir_set, dir_t dir) {
    dir_set |= (1 << dir);
  }

  // Logical implication
  inline static bool Implies(bool a, bool b) {
    return a ? b : true;
  }

  // Moves one step in the given direction
  inline static coord_t Move(coord_t coord, dir_t dir) {
    switch (dir) {
      case 0: return coord_t(coord.x, coord.y - 1);
      case 1: return coord_t(coord.x + 1, coord.y - 1);
      case 2: return coord_t(coord.x + 1, coord.y);
      case 3: return coord_t(coord.x + 1, coord.y + 1);
      case 4: return coord_t(coord.x, coord.y + 1);
      case 5: return coord_t(coord.x - 1, coord.y + 1);
      case 6: return coord_t(coord.x - 1, coord.y);
      case 7: return coord_t(coord.x - 1, coord.y - 1);
    }

    return coord_t(-1, -1);
  }

  // Returns the direction of move.
  inline static dir_t GetDirection(coord_t from, coord_t to) {
    if (from.x == to.x) {
      if (from.y == to.y) { return NO_DIRECTION; }
      else if (from.y < to.y) { return 4; }
      else { return 0; }
    } else if (from.x < to.x) {
      if (from.y == to.y) { return 2; }
      else if (from.y < to.y) { return 3; }
      else { return 1; }
    } else {
      if (from.y == to.y) { return 6; }
      else if (from.y < to.y) { return 5; }
      else { return 7; }
    }
  }

  inline static dir_t Rotate(dir_t dir, uint8_t rotation) {
    return (dir + rotation) % 8;
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

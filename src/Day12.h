#ifndef AOC2020_DAY12_H
#define AOC2020_DAY12_H

#include <stdlib.h>

/// Possible actions the boat can take
typedef enum {
  AC_NORTH,
  AC_EAST,
  AC_SOUTH,
  AC_WEST,
  AC_LEFT,
  AC_RIGHT,
  AC_FORWARD,
  AC_COUNT
} Action;

/// String representation of actions for printing
static char* actions[AC_COUNT] = {"North", "East", "South", "West", "Left", "Right", "Forward"};

/// Figure out the Manhattan Distance after processing the instructions
/// in `filename`.
int part1(const char* filename);

/// The state of the boat for part 2
typedef struct {
  int x;    ///< The E/W location of the boat (E+)
  int y;    ///< The N/S location of the boat (N+)
  int wx;   ///< The E/W location of the waypoint
  int wy;   ///< The N/S location of the waypoint
} ShipState;

/// Move the ship or waypoint based on the action requested
///
/// AC_NORTH: move the waypoint north
/// AC_EAST: move the waypoint east
/// AC_SOUTH: move the waypoint south
/// AC_WEST: move the waypoint west
/// AC_LEFT: rotate the waypoint left `amount` degrees about the ship
/// AC_RIGHT: rotate the waypoint right `amount` degrees about the ship
/// AC_FORWARd: move the ship to the waypoint `amount` times (waypoint
///              always relocates so its final position relative to
///              the ship is the same.)
ShipState move(ShipState current, Action action, int amount);

/// Following the new rules about waypoints followed in `move`, 
/// what is the Manhattan distance from the origin that the ship
/// achieves by following the instructions in `filename`?
int part2(const char* filename);

/// Run both parts
int day12(void);
#endif

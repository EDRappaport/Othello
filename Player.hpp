#ifndef _PLAYER_H
#define _PLAYER_H

#include <chrono>

#include "Board.hpp"
#include "PlayerMover.hpp"

typedef std::chrono::high_resolution_clock Clock;
typedef std::chrono::milliseconds milliseconds;


class Player
{
public:
  Player(Color playerColor, bool isComputer, int moveTimeoutSeconds = 5);
  
  // returns true if EOG
  bool MakeMove(Board& board);
  
private:
  bool _isComputer;
  Color _color;
  PlayerMover* _playerMover;
};

#endif

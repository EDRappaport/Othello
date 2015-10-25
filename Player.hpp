#ifndef _PLAYER_H
#define _PLAYER_H

#include "Board.hpp"
#include "PlayerMover.hpp"

class Player
{
public:
  Player(Color playerColor, bool isComputer);
  
  // returns true if EOG
  bool MakeMove(Board& board);
  
private:
  bool _isComputer;
  Color _color;
  PlayerMover* _playerMover;
};

#endif

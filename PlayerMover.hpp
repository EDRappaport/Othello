#ifndef _PLAYERMOVER_H
#define _PLAYERMOVER_H

#include "OthelloPoint.hpp"
#include "Board.hpp"

class PlayerMover
{
public:
  virtual OthelloPoint SelectMove(Board board) = 0;
protected:
  Color _color;
};

class ComputerPlayerMover : public PlayerMover
{
public:
  ComputerPlayerMover(Color color);
  OthelloPoint SelectMove(Board board);
};

class PersonPlayerMover : public PlayerMover
{
public:
  PersonPlayerMover(Color color);
  OthelloPoint SelectMove(Board board);
private:
  void GetUserMove(int& row, int& column);
};

#endif
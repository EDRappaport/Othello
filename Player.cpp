#include <ctime>

#include "Player.hpp"
#include "PlayerMover.hpp"

Player::Player(Color playerColor, bool isComputer)
{
  _color = playerColor;
  _isComputer = isComputer;
  
  if (_isComputer)
  {
    _playerMover = new ComputerPlayerMover(_color);
  }
  else
  {
    _playerMover = new PersonPlayerMover(_color);
  }
}

bool Player::MakeMove(Board& board)
{
  unsigned int start = clock();
  if (_color != board.GetTurn())
  {
    std::cerr << "Player is trying to make a move even though it's not his turn!!!";
    exit(-1);
  }
  
  OthelloPoint move = OthelloPoint();
  bool selectedLegalMove = false;
  while(!selectedLegalMove)
  {
    move = _playerMover->SelectMove(board);
    selectedLegalMove = board.IsLegalMove(_color, move);
  }
  std::cout << "Player selected move:" << move.ToString() << std::endl;
  std::cout << "Time to select move: " << clock() - start <<  " ms" << std::endl;
  return board.MakeMove(_color, move);
}


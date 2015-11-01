
#include "Player.hpp"
#include "PlayerMover.hpp"

Player::Player(Color playerColor, bool isComputer, int moveTimeoutSeconds)
{
  _color = playerColor;
  _isComputer = isComputer;
  
  if (_isComputer)
  {
    _playerMover = new ComputerPlayerMover(_color, moveTimeoutSeconds);
  }
  else
  {
    _playerMover = new PersonPlayerMover(_color);
  }
}

bool Player::MakeMove(Board& board)
{  
  Clock::time_point start_time = Clock::now();

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

  Clock::time_point current_time = Clock::now();
  double moveTime = (double)std::chrono::duration_cast<milliseconds>(current_time - start_time).count() / 1000;
  std::cout << "Time to select move: " << moveTime << " s." << std::endl;
  std::cout << "Player selected move: " << move.ToString() << std::endl;
  return board.MakeMove(_color, move);
}


#include "PlayerMover.hpp"
#include "Constants.hpp"

PersonPlayerMover::PersonPlayerMover(Color color)
{
  _color = color;
}

OthelloPoint PersonPlayerMover::SelectMove(Board board)
{
  if(board.GetAllLegalMoves(_color).size() == 0)
  {
    std::cout << "Player has no moves, so will not do anything." << std::endl;
    return OthelloPoint();
  }
  int row, column;
  GetUserMove(row, column);
  return OthelloPoint(row, column);
}

void PersonPlayerMover::GetUserMove(int& row, int& column)
{
  std::string userCommand;
  int localRow, localColumn1, localColumn2;
  while(true)
  {
    std::cout << "Enter the row and column of your move - number followed by letter (for ex: 1D)" << std::endl;
    std::cin >> userCommand;
    
    if (userCommand.length() == 2)
    {
      localRow = userCommand[0] - '0' - 1;
      localColumn1 = (int) userCommand[1] - (int) 'A';
      localColumn2 = (int) userCommand[1] - (int) 'a';
      
      if (localRow >= 0 && localRow < 8)
      {
	if(localColumn1 >= 0 && localColumn1 < 8)
	{
	  row = localRow;
	  column = localColumn1;
	  break;
	}
	if(localColumn2 >= 0 && localColumn2 < 8)
	{
	  row = localRow;
	  column = localColumn2;
	  break;
	}
      }
    }
    
    std::cout << defaultBackgroundRedText << 
      "Nope, I need a valid command - please ensure that all you put down is the number of the row and the letter of the column."
      << defaultBackgroundDefaultText << std::endl;
  }
}
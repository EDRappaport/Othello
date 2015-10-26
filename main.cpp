#include <iostream>
#include <stdint.h>
#include <stdlib.h>
#include <unordered_set>

#include "Board.hpp"
#include "Player.hpp"

char GetUserInput(std::unordered_set<char> legalEntries)
{
  std::string input;
  while(true)
  {
    std::cin >> input;
    if (input.length() == 1 && legalEntries.count(input[0]) != 0)
    {
      return input[0];
    }
    std::cout << "Please enter a legal entry!" << std::endl;
  }
}

int main(int argc, char **argv) {
  std::cout << "Hello, welcome to Othello (Reversi)!" << std::endl;
  
  std::unordered_set<char> legalColors = std::unordered_set<char>({'b', 'w'});
  std::unordered_set<char> legalPlayers = std::unordered_set<char>({'h', 'c'});
  std::cout << "Please decide who shall be Player 1." << std::endl <<
    "Player 1 will go first - you may select whether Player 1 is Black or" << 
    "White and whether Player 1 is a human user or a Computer." << std::endl;
  std::cout << "What color shall player 1 be?? Please type either \'b\' or \'w\'." << std::endl;
  char player1Color = GetUserInput(legalColors);
  std::cout << "Should player 1 be a human or computer?? Please type either \'h\' or \'c\'." << std::endl;
  char player1Player = GetUserInput(legalPlayers);
  std::cout << "Should player 2 be a human or computer?? Please type either \'h\' or \'c\'." << std::endl;
  char player2Player = GetUserInput(legalPlayers);  
  
  Board board = Board(player1Color == 'b' ? BlackPlayer : WhitePlayer);
  board.DisplayBoard();
  
  Player p1 = Player(player1Color == 'b' ? BlackPlayer : WhitePlayer, player1Player == 'c');
  Player p2 = Player(player1Color == 'b' ? WhitePlayer : BlackPlayer, player2Player == 'c');

  while(true)
  {
    if (p1.MakeMove(board)) {break;}
    board.DisplayBoard();
    if (p2.MakeMove(board)) {break;}
    board.DisplayBoard();
  }
  std::cout << "FINAL BOARD:" << std::endl;
  board.DisplayBoard();
  std::cout << "Final Score: Black has: " << board.GetScore(BlackPlayer) << " points.  White has: "
    << board.GetScore(WhitePlayer) << " points." << std::endl;
  if (board.GetScore(WhitePlayer) > board.GetScore(BlackPlayer))
  {
    std::cout << "White has won!!!" << std::endl;
  }
  else if (board.GetScore(BlackPlayer) > board.GetScore(WhitePlayer))
  {
    std::cout << "Black has won!!!" << std::endl;
  }
  else
  {
    std::cout << "Black and White have tied." << std::endl;
  }
  
  return 0;
}

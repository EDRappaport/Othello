#include <iostream>
#include <fstream>
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

std::ifstream* GetBoardFile()
{
  while (true)
  {
    std::string filename = "/home/elli/Documents/Dropbox/The Cooper Union/ArtificialIntelligence/Othello/Othello/sampleOthello2.txt";
    //std::cin >> filename;
    std::ifstream boardFile;
    boardFile.open(filename.c_str());
    if (boardFile.is_open())
    {
      return &boardFile;
    }
    std::cout << "Unable to open file '" << filename << "' - please enter a filename of an openable file:" << std::endl;
  }
}

int GetTimeoutSeconds()
{
  int timeout;
  while(true)
  {
    std::cin >> timeout;
    if (!std::cin)
    {
      //non-integer
      std::cin.clear();
      while (std::cin.get() != '\n'); // clear
    }
    else
    {
      if (timeout > 0)
      {
	return timeout;
      }
    }
    std::cout << "Please enter a legal entry!" << std::endl;
  }
}

int main(int argc, char **argv) {
  std::cout << "Hello, welcome to Othello (Reversi)!" << std::endl;
  
  std::unordered_set<char> legalPlayers = std::unordered_set<char>({'h', 'c'});
  std::unordered_set<char> legalYN = std::unordered_set<char>({'y', 'Y', 'n', 'N'});
  std::cout << "Should player 1 (Black) be a human or computer?? Please type either \'h\' or \'c\'." << std::endl;
  char player1Player = GetUserInput(legalPlayers);
  std::cout << "Should player 2 (White) be a human or computer?? Please type either \'h\' or \'c\'." << std::endl;
  char player2Player = GetUserInput(legalPlayers);
 
  std::cout << "Would you like to load a game from file (Y/N)?" << std::endl;
  char loadResponse = GetUserInput(legalYN);
  bool shouldLoad = loadResponse == 'y' || loadResponse == 'Y';
  
  Board board;
  int timeoutSeconds;
  if (shouldLoad)
  {
    std::cout << "Please enter the filename to load:" << std::endl;
    std::ifstream boardFile;
    while (true)
    {
      std::string filename;// = "/home/elli/Documents/Dropbox/The Cooper Union/ArtificialIntelligence/Othello/Othello/test1.txt";
      std::cin >> filename;
      boardFile.open(filename.c_str());
      if (boardFile.is_open())
      {
	break;
      }
      std::cout << "Unable to open file '" << filename << "' - please enter a filename of an openable file:" << std::endl;
    }
    board = Board(boardFile, &timeoutSeconds);
  }
  else
  {
    board = Board();
  }
  
  if ((player1Player == 'c' || player2Player == 'c') && !shouldLoad)
  {
    std::cout << "Choose an integer number of seconds for how long the computer can think: " << std::endl;
    timeoutSeconds = GetTimeoutSeconds();
  }
  
  Player p1 = Player(BlackPlayer, player1Player == 'c', timeoutSeconds);
  Player p2 = Player(WhitePlayer, player2Player == 'c', timeoutSeconds);
  
  // HACK for when we start with white (if user eneters starting board) -- should fix
  Color currentTurn = board.GetTurn();
  if (currentTurn == WhitePlayer)
  {
    p1 = Player(WhitePlayer, player2Player == 'c', timeoutSeconds);
    p2 = Player(BlackPlayer, player1Player == 'c', timeoutSeconds);
  }
  
  board.DisplayBoard();
  while(true)
  {
    if (p1.MakeMove(board)) {break;}
    board.DisplayBoard();
    if (p2.MakeMove(board)) {break;}
    board.DisplayBoard();
  }
  std::cout << "FINAL BOARD:" << std::endl;
  board.DisplayBoard();
  int i;
  std::cout << "Final Score: Black has: " << board.GetScore(BlackPlayer,i,i) << " points.  White has: "
    << board.GetScore(WhitePlayer,i,i) << " points." << std::endl;
  if (board.GetScore(WhitePlayer,i,i) > board.GetScore(BlackPlayer,i,i))
  {
    std::cout << "White has won!!!" << std::endl;
  }
  else if (board.GetScore(BlackPlayer,i,i) > board.GetScore(WhitePlayer,i,i))
  {
    std::cout << "Black has won!!!" << std::endl;
  }
  else
  {
    std::cout << "Black and White have tied." << std::endl;
  }
  
  return 0;
}

#include <iostream>
#include <sstream>
#include <vector>
#include <ctime>
#include "Board.hpp"

Board::Board()
{
  for (int ii = 0; ii < BoardWidth; ii++)
  {
    for (int jj = 0; jj < BoardHeight; jj++)
    {
      _board[ii][jj] = Empty;
    }
  }
  _board[3][3] = White;
  _board[4][4] = White;
  _board[3][4] = Black;
  _board[4][3] = Black;
  
  _turn = BlackPlayer;
  
  UpdateLegalMoves();
}

Board::Board(SpaceHolder currentBoard[8][8], SpaceHolder legalMoves[8][8], Color turn, OthelloPoint move)
{
  for (int ii = 0; ii < BoardWidth; ii++)
  {
    for (int jj = 0; jj < BoardHeight; jj++)
    {
      _board[ii][jj] = currentBoard[ii][jj];
      _legalMovesBoard[ii][jj] = legalMoves[ii][jj];
    }
  }
  _turn = turn;
  
  MakeMove(turn, move);
}

Board::Board(std::ifstream& savedBoardFile, int* timeoutSeconds)
{
  int c;
  for (int ii = 0; ii < BoardWidth; ii++)
  {
    for (int jj = 0; jj < BoardHeight; jj++)
    {
      savedBoardFile >> c;
      if(!savedBoardFile || c < 0 || c >2)
      {
	std::cerr <<"Invalid file format, try again!";
	exit(-1);
      }
      _board[ii][jj] = (SpaceHolder)(c);
    }
  }
  
  savedBoardFile >> c;
  if(!savedBoardFile || c < 1 || c >2)
  {
    std::cerr <<"Invalid file format, try again!";
    exit(-1);
  }
  _turn = (Color)(c);
  
  int i;
  savedBoardFile >> i;
  if(!savedBoardFile || c < 0)
  {
    std::cerr <<"Invalid file format, try again!";
    exit(-1);
  }
  *timeoutSeconds = i;
  
  UpdateLegalMoves();
}


Board Board::GetResultantBoard(Color color, OthelloPoint othelloPoint)
{
  if (_turn != color)
  {
    std::cerr << "Somebody is going out of turn!";
    exit(-1);
  }
  
  return Board(_board, _legalMovesBoard, _turn, othelloPoint);
}

int Board::GetNumCornersOccupied(Color color)
{
  int numCorners;
  if(_board[0][0] != Empty && _board[0][0] == color)
  {
    numCorners++;
  }
  if(_board[0][BoardHeight-1] != Empty && _board[0][BoardHeight-1] == color)
  {
    numCorners++;
  }
  if(_board[BoardWidth-1][0] != Empty && _board[BoardWidth-1][0] == color)
  {
    numCorners++;
  }
  if(_board[BoardWidth-1][BoardHeight-1] != Empty && _board[BoardWidth-1][BoardHeight-1] == color)
  {
    numCorners++;
  }
  return numCorners;
}

int Board::GetCornerClosenessScore(Color color)
{
  Color opposingColor = GetOpposingColor(color);
  int l;
  if(_board[0][0] == Empty){
    if(_board[1][1]==color) l-=13; else if(_board[1][1]==opposingColor) l+=13; 
    if(_board[1][0]==color) l-=6; else if(_board[1][0]==opposingColor) l+=6; 
    if(_board[0][1]==color) l-=6; else if(_board[0][1]==opposingColor) l+=6;
  }
  if(_board[0][7]==Empty){				
    if(_board[1][6]==color) l-=13; else if(_board[1][6]==opposingColor) l+=13; 
    if(_board[1][7]==color) l-=6; else if(_board[1][7]==opposingColor) l+=6; 
    if(_board[0][6]==color) l-=6; else if(_board[0][6]==opposingColor) l+=6;
  }
    if(_board[7][0]==Empty){	
    if(_board[6][1]==color) l-=13; else if(_board[6][1]==opposingColor) l+=13; 
    if(_board[7][1]==color) l-=6; else if(_board[7][1]==opposingColor) l+=6; 
    if(_board[6][0]==color) l-=6; else if(_board[6][0]==opposingColor) l+=6;
  }
    if(_board[7][7]==Empty){
    if(_board[6][6]==color) l-=13; else if(_board[6][6]==opposingColor) l+=13;
    if(_board[7][6]==color) l-=6; else if(_board[7][6]==opposingColor) l+=6; 
    if(_board[6][7]==color) l-=6; else if(_board[6][7]==opposingColor) l+=6;
  }
  return l;
}


int Board::GetScore(Color color, int& detailedPointScore, int& frontierCount)
{
  int score = 0;
  detailedPointScore = 0;
  frontierCount = 0;
  for (int ii = 0; ii < BoardWidth; ii++)
  {
    for (int jj = 0; jj < BoardHeight; jj++)
    {
      if(_board[ii][jj] != Empty)
      {
	if (_board[ii][jj] == color)
	{
	  score++;
	  detailedPointScore += _pieceScore[ii][jj];
	  for (int kk = 0; kk < 8; kk++)
	  {
	    int x = ii + _xDirections[kk];
	    int y = jj + _yDirections[kk];
	    if(x >= 0 && x < BoardWidth && y >= 0 && y < BoardHeight && _board[x][y] == Empty)
	    {
	      frontierCount++;
	    }
	  }
	}
	else
	{
	  detailedPointScore -= _pieceScore[ii][jj];
	}
      }
    }
  }
  return score;
}

bool Board::MakeMove(Color color, OthelloPoint othelloPointMove)
{
  if (_turn != color)
  {
    std::cerr << "Somebody is going out of turn!";
    exit(-1);
  }
  _turn = (Color) GetOpposingColor((SpaceHolder) _turn);
  
  if (!(othelloPointMove.GetX() == -1 || othelloPointMove.GetY() == -1))
  {
    if (!IsLegalMove(color, othelloPointMove))
    {
      std::cerr << color << " is trying to make illegal move! Player should check IsLegalMove() first." << std::endl;
      exit(-1);
    }
    UpdateLegalMoves(true, color, othelloPointMove);
    _board[othelloPointMove.GetX()][othelloPointMove.GetY()] = (SpaceHolder) color;
  }
  
  blackLegalMovestSet.clear();
  whiteLegalMovesSet.clear();
  UpdateLegalMoves();
  if(blackLegalMovestSet.empty() && whiteLegalMovesSet.empty())
  {
    return true; //EOG
  }
  return false;
}

bool Board::IsLegalMove(Color color, OthelloPoint othelloPoint)
{
  return (GetAllLegalMoves(color).size() == 0 && othelloPoint.GetX() == -1) //if no legal moves, then no move is legal
    || (_legalMovesBoard[othelloPoint.GetX()][othelloPoint.GetY()] == (SpaceHolder) color
    || _legalMovesBoard[othelloPoint.GetX()][othelloPoint.GetY()] == Both);
}

std::vector<OthelloPoint> Board::GetAllLegalMoves(Color color)
{
  std::vector<OthelloPoint> legalMoves;
  for (int ii = 0; ii < BoardWidth; ii++)
  {
    for (int jj = 0; jj < BoardHeight; jj++)
    {
      if (_legalMovesBoard[ii][jj] == (SpaceHolder) color || _legalMovesBoard[ii][jj] == Both)
      {
	legalMoves.push_back(OthelloPoint(ii, jj));
      }
    }
  }
  return legalMoves;
}

Color Board::GetTurn()
{
  return _turn;
}

void Board::DisplayBoard()
{
  std::cout << "------------------------------------------------" << std::endl;
  std::cout << "Current Board: " << std::endl;
  std::cout << "  A  B  C  D  E  F  G  H" << std::endl;
  for (int ii = 0; ii < BoardWidth; ii++)
  {
    std::cout << ii+1;
    for (int jj = 0; jj < BoardHeight; jj++)
    {
      PrintColorSpace(_board[ii][jj], _legalMovesBoard[ii][jj]);
    }
    std::cout << std::endl;
  }
  std::cout << "------------------------------------------------" << std::endl;
  std::cout << "------------------------------------------------" << std::endl << std::endl << std::endl;
}

void Board::PrintColorSpace(SpaceHolder spaceHolder, SpaceHolder legalMove)
{
  switch (spaceHolder)
  {
    case Empty:
      std::cout << greenBackgroundBlackText;
      break;
    case White:
      std::cout << whiteBackgroundBlackText;
      break;
    case Black:
      std::cout << blueBackgroundBlackText;
      break;
    default:
      //what!!
      std::cerr << "Somebody handed in an unaccpetable value to PrintColorSpace!!!" << std::endl;
      exit(-1);
  }
  switch (legalMove)
  {
    case Empty:
      std::cout << "  ";
      break;
    case White:
      std::cout << "ww";
      break;
    case Black:
      std::cout << "bb";
      break;
    case Both:
      std::cout << "aa";
      break;
    default:
      //what!!
      std::cerr << "Somebody handed in an unaccpetable value to PrintColorSpace!!!" << std::endl;
      exit(-1);
  }
   std::cout << defaultBackgroundDefaultText << "|";
}

void Board::AddToLegalSet(SpaceHolder color, OthelloPoint point)
{
  if (color == White)
  {
    whiteLegalMovesSet.insert(point);
  }
  else
  {
    blackLegalMovestSet.insert(point);
  }
}

void Board::UpdateLegalMoves(bool flip, Color thisPlayer, OthelloPoint point)
{
  for (int ii = 0; ii < BoardWidth; ii++)
  {
    for (int jj = 0; jj < BoardHeight; jj++)
    {   
      if(flip)
      {
	//this is currently the hacky way to update the pieces for the move
	ii = point.GetX();
	jj = point.GetY();
      }
      // init to Empty
      _legalMovesBoard[ii][jj] = Empty;
      
      if (_board[ii][jj] != Empty)
      {
	//not a legal move if already occupied
	continue;
      }
      
      // checking direct left
      if (ii - 2 >= 0 && _board[ii-1][jj] != Empty &&
	(flip || (_legalMovesBoard[ii][jj] == _board[ii-1][jj] || _legalMovesBoard[ii][jj] == Empty)))
      {
	SpaceHolder color = _board[ii-1][jj];
	SpaceHolder opposingColor = GetOpposingColor(color);
	// since the spot directly to the left is of color, maybe opposingColor can go here
	int kk;
	for (kk = ii-2; kk > 0 && _board[kk][jj] == color; kk--)
	{
	  //keep moving while we hit the same color
	}
	if (_board[kk][jj] == opposingColor)
	{
	  //then we found the same color along this route - the current pos is legal for him!
	  if (_legalMovesBoard[ii][jj] == Empty || _legalMovesBoard[ii][jj] == opposingColor) {_legalMovesBoard[ii][jj] = opposingColor;}
	  else {_legalMovesBoard[ii][jj] = Both;}
	  if (!flip)
	  {
	    AddToLegalSet(opposingColor, OthelloPoint(ii,jj));
	  }
	  
	  if (flip && opposingColor == thisPlayer)
	  {
	    for (kk = ii-1; kk > 0 && _board[kk][jj] == color; kk--)
	    {
	      _board[kk][jj] = opposingColor;
	    }
	  }
	}
      }
      
      //checking direct right
      if (ii + 2 < BoardWidth && _board[ii+1][jj] != Empty &&
	(flip || (_legalMovesBoard[ii][jj] == _board[ii+1][jj] || _legalMovesBoard[ii][jj] == Empty)))
      {
	SpaceHolder color = _board[ii+1][jj];
	SpaceHolder opposingColor = GetOpposingColor(color);
	// since the spot directly to the right is of color, maybe opposingColor can go here
	int kk;
	for (kk = ii+2; kk < BoardWidth-1 && _board[kk][jj] == color; kk++)
	{
	  //keep moving while we hit the same color
	}
	if (_board[kk][jj] == opposingColor)
	{
	  //then we found the same color along this route - the current pos is legal for him!
	  if (_legalMovesBoard[ii][jj] == Empty || _legalMovesBoard[ii][jj] == opposingColor) {_legalMovesBoard[ii][jj] = opposingColor;}
	  else {_legalMovesBoard[ii][jj] = Both;}
	  if (!flip)
	  {
	    AddToLegalSet(opposingColor, OthelloPoint(ii,jj));
	  }
	  
	  if (flip && opposingColor == thisPlayer)
	  {
	    for (kk = ii+1; kk < BoardWidth && _board[kk][jj] == color; kk++)
	    {
	      _board[kk][jj] = opposingColor;
	    }
	  }
	}
      }
      
      //checking direct up
      if (jj - 2 >= 0 && _board[ii][jj-1] != Empty &&
	(flip || (_legalMovesBoard[ii][jj] == _board[ii][jj-1] || _legalMovesBoard[ii][jj] == Empty)))
      {
	SpaceHolder color = _board[ii][jj-1];
	SpaceHolder opposingColor = GetOpposingColor(color);
	// since the spot directly up is of color, maybe opposingColor can go here
	int kk;
	for (kk = jj-2; kk > 0 && _board[ii][kk] == color; kk--)
	{
	  //keep moving while we hit the same color
	}
	if (_board[ii][kk] == opposingColor)
	{
	  //then we found the same color along this route - the current pos is legal for him!
	  if (_legalMovesBoard[ii][jj] == Empty || _legalMovesBoard[ii][jj] == opposingColor) {_legalMovesBoard[ii][jj] = opposingColor;}
	  else {_legalMovesBoard[ii][jj] = Both;}
	  if (!flip)
	  {
	    AddToLegalSet(opposingColor, OthelloPoint(ii,jj));
	  }
	  
	  if (flip && opposingColor == thisPlayer)
	  {
	    for (kk = jj-1; kk > 0 && _board[ii][kk] == color; kk--)
	    {
	      _board[ii][kk] = opposingColor;
	    }
	  }
	}
      }
      
      //checking direct down
      if (jj + 2 < BoardHeight && _board[ii][jj+1] != Empty &&
	(flip || (_legalMovesBoard[ii][jj] == _board[ii][jj+1] || _legalMovesBoard[ii][jj] == Empty)))
      {
	SpaceHolder color = _board[ii][jj+1];
	SpaceHolder opposingColor = GetOpposingColor(color);
	// since the spot directly down is of color, maybe opposingColor can go here
	int kk;
	for (kk = jj+2; kk < BoardHeight-1 && _board[ii][kk] == color; kk++)
	{
	  //keep moving while we hit the same color
	}
	if (_board[ii][kk] == opposingColor)
	{
	  //then we found the same color along this route - the current pos is legal for him!
	  if (_legalMovesBoard[ii][jj] == Empty || _legalMovesBoard[ii][jj] == opposingColor) {_legalMovesBoard[ii][jj] = opposingColor;}
	  else {_legalMovesBoard[ii][jj] = Both;}
	  if (!flip)
	  {
	    AddToLegalSet(opposingColor, OthelloPoint(ii,jj));
	  }
	  
	  if (flip && opposingColor == thisPlayer)
	  {
	    for (kk = jj+1; kk < BoardHeight && _board[ii][kk] == color; kk++)
	    {
	      _board[ii][kk] = opposingColor;
	    }
	  }
	}
      }
      
      //checking up left
      if (ii - 2 >= 0 && jj - 2 >= 0 && _board[ii-1][jj-1] != Empty &&
	(flip || (_legalMovesBoard[ii][jj] == _board[ii-1][jj-1] || _legalMovesBoard[ii][jj] == Empty)))
      {
	SpaceHolder color = _board[ii-1][jj-1];
	SpaceHolder opposingColor = GetOpposingColor(color);
	int kk, ll;
	for (kk = ii-2, ll = jj-2 ; kk > 0 && ll > 0 && _board[kk][ll] == color; kk--, ll--)
	{
	  //keep moving while we hit the same color
	}
	if (_board[kk][ll] == opposingColor)
	{
	  //then we found the same color along this route - the current pos is legal for him!
	  if (_legalMovesBoard[ii][jj] == Empty || _legalMovesBoard[ii][jj] == opposingColor) {_legalMovesBoard[ii][jj] = opposingColor;}
	  else {_legalMovesBoard[ii][jj] = Both;}
	  if (!flip)
	  {
	    AddToLegalSet(opposingColor, OthelloPoint(ii,jj));
	  }
	  
	  if (flip && opposingColor == thisPlayer)
	  {
	    for (kk = ii-1, ll = jj-1; kk > 0 && ll > 0 && _board[kk][ll] == color; kk--, ll--)
	    {
	      _board[kk][ll] = opposingColor;
	    }
	  }
	}
      }
      
      //checking up right
      if (ii + 2 < BoardWidth && jj - 2 >= 0 && _board[ii+1][jj-1] != Empty &&
	(flip || (_legalMovesBoard[ii][jj] == _board[ii+1][jj-1] || _legalMovesBoard[ii][jj] == Empty)))
      {
	SpaceHolder color = _board[ii+1][jj-1];
	SpaceHolder opposingColor = GetOpposingColor(color);
	int kk, ll;
	for (kk = ii+2, ll = jj-2 ; kk < BoardWidth-1 && ll > 0 && _board[kk][ll] == color; kk++, ll--)
	{
	  //keep moving while we hit the same color
	}
	if (_board[kk][ll] == opposingColor)
	{
	  //then we found the same color along this route - the current pos is legal for him!
	  if (_legalMovesBoard[ii][jj] == Empty || _legalMovesBoard[ii][jj] == opposingColor) {_legalMovesBoard[ii][jj] = opposingColor;}
	  else {_legalMovesBoard[ii][jj] = Both;}
	  if (!flip)
	  {
	    AddToLegalSet(opposingColor, OthelloPoint(ii,jj));
	  }
	  
	  if (flip && opposingColor == thisPlayer)
	  {
	    for (kk = ii+1, ll = jj-1; kk < BoardWidth && ll > 0 && _board[kk][ll] == color; kk++, ll--)
	    {
	      _board[kk][ll] = opposingColor;
	    }
	  }
	}
      }
      
      //checking down left
      if (ii - 2 >= 0 && jj + 2 < BoardHeight && _board[ii-1][jj+1] != Empty &&
	(flip || (_legalMovesBoard[ii][jj] == _board[ii-1][jj+1] || _legalMovesBoard[ii][jj] == Empty)))
      {
	SpaceHolder color = _board[ii-1][jj+1];
	SpaceHolder opposingColor = GetOpposingColor(color);
	int kk, ll;
	for (kk = ii-2, ll = jj+2 ; kk > 0 && ll < BoardHeight-1 && _board[kk][ll] == color; kk--, ll++)
	{
	  //keep moving while we hit the same color
	}
	if (_board[kk][ll] == opposingColor)
	{
	  //then we found the same color along this route - the current pos is legal for him!
	  if (_legalMovesBoard[ii][jj] == Empty || _legalMovesBoard[ii][jj] == opposingColor) {_legalMovesBoard[ii][jj] = opposingColor;}
	  else {_legalMovesBoard[ii][jj] = Both;}
	  if (!flip)
	  {
	    AddToLegalSet(opposingColor, OthelloPoint(ii,jj));
	  }
	  
	  if (flip && opposingColor == thisPlayer)
	  {
	    for (kk = ii-1, ll = jj+1; kk > 0 && ll < BoardHeight && _board[kk][ll] == color; kk--, ll++)
	    {
	      _board[kk][ll] = opposingColor;
	    }
	  }
	}
      }
      
      //checking down right
      if (ii + 2 < BoardWidth && jj + 2 < BoardHeight && _board[ii+1][jj+1] != Empty &&
	(flip || (_legalMovesBoard[ii][jj] == _board[ii+1][jj+1] || _legalMovesBoard[ii][jj] == Empty)))
      {
	SpaceHolder color = _board[ii+1][jj+1];
	SpaceHolder opposingColor = GetOpposingColor(color);
	int kk, ll;
	for (kk = ii+2, ll = jj+2 ; kk < BoardWidth-1 && ll < BoardHeight-1 && _board[kk][ll] == color; kk++, ll++)
	{
	  //keep moving while we hit the same color
	}
	if (_board[kk][ll] == opposingColor)
	{
	  //then we found the same color along this route - the current pos is legal for him!
	  if (_legalMovesBoard[ii][jj] == Empty || _legalMovesBoard[ii][jj] == opposingColor) {_legalMovesBoard[ii][jj] = opposingColor;}
	  else {_legalMovesBoard[ii][jj] = Both;}
	  if (!flip)
	  {
	    AddToLegalSet(opposingColor, OthelloPoint(ii,jj));
	  }
	  
	  if (flip && opposingColor == thisPlayer)
	  {
	    for (kk = ii+1, ll = jj+1; kk < BoardWidth && ll < BoardHeight && _board[kk][ll] == color; kk++, ll++)
	    {
	      _board[kk][ll] = opposingColor;
	    }
	  }
	}
      }
      
      if (flip)
      {
	return;
      }
    }
  }
  //std::cout << "Time to UpdateLegalMoves: " << clock() - start <<  " ms" << std::endl;
}

Color Board::GetOpposingColor(Color color)
{
  switch (color)
  {
    case WhitePlayer:
      return BlackPlayer;
    case BlackPlayer:
      return WhitePlayer;
    default:
      std::cerr << "NOPE!!!!!" << std::endl;
      exit(-1);
  }
}

SpaceHolder Board::GetOpposingColor(SpaceHolder color)
{
  switch (color)
  {
    case White:
      return Black;
    case Black:
      return White;
    default:
      return Empty;
  }
}





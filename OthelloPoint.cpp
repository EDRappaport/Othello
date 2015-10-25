#include "OthelloPoint.hpp"


OthelloPoint::OthelloPoint(int x, int y)
{
  if (x < 0 || x > 7 || y < 0 || y > 7)
  {
    std::cerr << "OthelloPoint ctor received point that is off the board!" << std::endl;
    exit(-1);
  }
  _x = x;
  _y = y;
}

OthelloPoint::OthelloPoint()
{
  _x = -1;
  _y = -1;
}


bool OthelloPoint::operator==(OthelloPoint other) const
{
  return _x == other.GetX() && _y == other._y;
}


std::string OthelloPoint::ToString()
{
  const std::string openPoint("<");
  const std::string middlePoint (", ");
  const std::string endPoint (">");;
  return openPoint + std::to_string(_x) + middlePoint + std::to_string(_y) +  endPoint;
}

int OthelloPoint::GetX() const
{
  return _x;
}

int OthelloPoint::GetY() const
{
  return _y;
}

bool OthelloPointComparer::operator()(OthelloPoint p1, OthelloPoint p2)
{
  return p1.GetX() < p2.GetX();
}

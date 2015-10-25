#ifndef _OTHELLOPOINT_H
#define _OTHELLOPOINT_H

#include <iostream>
#include <string>
#include <unordered_set>

class OthelloPoint
{
public:
  OthelloPoint(int x, int y);
  OthelloPoint(); //used if no move (like a NULL)
  
  bool operator==(OthelloPoint) const;
  
  std::string ToString();
  
  int GetX() const;
  int GetY() const;
  
private:
  int _x, _y;
};

class OthelloPointComparer
{
public:
  bool operator() (OthelloPoint, OthelloPoint);
};

namespace std {
  template <>
  struct hash<OthelloPoint>
  {
    typedef OthelloPoint argument_type;
    typedef std::size_t result_type;

    std::size_t operator()(const OthelloPoint & othelloPoint) const
    {
       return (std::hash<int>() (othelloPoint.GetX()) ^ (std::hash<int>() (othelloPoint.GetY())<<1));
    }
  };
}

#endif
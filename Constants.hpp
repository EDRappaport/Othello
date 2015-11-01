#ifndef _CONSTANTS_H
#define _CONSTANTS_H

enum SpaceHolder : int8_t
{
  Empty = 0,
  Black = 1,
  White = 2,
  Both = 3, // used for the legalMovesBoard when both players can go there (if it was their turn)
};

enum Color : int8_t
{
  BlackPlayer = 1,
  WhitePlayer = 2,
};
  
const std::string greenBackgroundBlackText = "\x1b[1;4;30;42m";
const std::string whiteBackgroundBlackText = "\x1b[1;4;30;47m";
const std::string blueBackgroundBlackText = "\x1b[1;4;30;44m";
const std::string defaultBackgroundDefaultText = "\x1b[0m";
const std::string defaultBackgroundRedText = "\x1b[0;31m";

#endif
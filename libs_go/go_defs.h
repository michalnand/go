#ifndef _GO_DEFS_H_
#define _GO_DEFS_H_

#define GOBAN_NO_LEGAL_MOVES        ((int)-3)
#define GOBAN_WHITE_ILLEGAL_MOVE    ((int)-2)
#define GOBAN_BLACK_ILLEGAL_MOVE    ((int)-1)
#define GOBAN_GAME_PLAYING          ((int)0)
#define GOBAN_GAME_END              ((int)1)
#define GOBAN_PASS                  ((int)2)
#define GOBAN_GAME_END_AREA         ((int)3)
#define GOBAN_GAME_END_MOVES        ((int)4)




#define GO_PLAYER_EMPTY             ((int)0)
#define GO_PLAYER_BLACK             ((int)1)
#define GO_PLAYER_WHITE             ((int)-1)


#define GO_PLAYER_EMPTY_UCHAR       ((unsigned char)0)
#define GO_PLAYER_BLACK_UCHAR       ((unsigned char)1)
#define GO_PLAYER_WHITE_UCHAR       ((unsigned char)2)


#include <vector>

typedef std::vector<std::vector<int>> GoBoard;

struct sGoPoint
{
    int x, y;
};


enum GoDatasetMode
{
    GoDatasetMode_MovePrediction,
    GoDatasetMode_WinnerPrediction
};

#endif

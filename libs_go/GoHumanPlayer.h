#ifndef _GO_HUMAN_PLAYER_H_
#define _GO_HUMAN_PLAYER_H_

#include "GoPlayer.h"


class GoHumanPlayer: public GoPlayer
{
    public:
        GoHumanPlayer();
        virtual ~GoHumanPlayer();

    private:
        GoMove choose_move();
        bool is_legal(GoMove &move);
};

#endif

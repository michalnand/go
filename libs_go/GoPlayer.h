#ifndef _GO_PLAYER_H_
#define _GO_PLAYER_H_

#include "Goban.h"


class GoPlayer
{
    public:
        GoPlayer();
        virtual ~GoPlayer();

        void play(Goban &goban);

        GoMove get_move();

        virtual void new_game();
        virtual void set_result(bool winner);

    protected:
        virtual GoMove choose_move();


    protected:
        GoBoard board;
        std::vector<GoMove> legal_moves;
        int current_player;

        GoMove move;
};

#endif

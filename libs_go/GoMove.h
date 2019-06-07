#ifndef _GO_MOVE_H_
#define _GO_MOVE_H_

#include "go_defs.h"
#include <string>

class GoMove
{
    public:
        GoMove();
        GoMove(GoMove &other);
        GoMove(const GoMove &other);

        GoMove(bool pass, int player);
        GoMove(int x, int y, int player);

        GoMove& operator =(GoMove &rhs);
        GoMove& operator =(const GoMove &rhs);

        virtual ~GoMove();

        void pass_move(int player);
        void black_move(int x, int y);
        void white_move(int x, int y);

    public:
        int player();
        bool pass();
        int x();
        int y();
        bool operator ==(const GoMove &rhs);
        bool operator !=(const GoMove &rhs);

        unsigned int move_to_idx(unsigned int board_size);
        void print();
        std::string asString();

    private:
        void copy(GoMove& other);
        void copy(const GoMove& other);

    private:
        int m_player;
        bool m_pass;
        int m_x, m_y;
};


#endif

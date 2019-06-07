#ifndef _GO_SGF_PLAYER_H_
#define _GO_SGF_PLAYER_H_

#include "GoPlayer.h"
#include <string>

class GoSgfPlayer: public GoPlayer
{
    public:
        GoSgfPlayer(std::string sgf_file_name);
        virtual ~GoSgfPlayer();

        int get_winner();
        unsigned int get_moves_count();
        GoMove get_move(unsigned int idx);

    protected:
        GoMove choose_move();
        void parse(std::string sgf_file_name);

        bool is_pattern(std::vector<char> &raw, std::string &pattern, unsigned int ofs);
        GoMove parse_raw_move(int player, std::vector<char> &raw, unsigned int ofs);

    private:
        int board_size;
        unsigned int ptr;

        std::vector<GoMove> moves;

        int winner;
};

#endif

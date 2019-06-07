#ifndef _GO_RL_PLAYER_H_
#define _GO_RL_PLAYER_H_

#include <string>
#include <json_config.h>
#include <dqn.h>

#include "GoPlayer.h"

class GoRLPlayer: public GoPlayer
{
    public:
        GoRLPlayer(std::string config_file_name, unsigned int board_size);
        virtual ~GoRLPlayer();

        void set_result(bool winner);
        void learn();

        void test();
        DQNCompare& get_compare_result();

        void new_batch();
        void print();

    private:
        GoMove choose_move();

        std::vector<float> create_nn_input(GoBoard &board);
        std::vector<GoMove> find_top_moves();

        std::vector<unsigned int> get_top_n(std::vector<float> &confidence, unsigned int top_n_count);
        bool not_in(std::vector<unsigned int> &vect, unsigned int value);

        bool is_legal(GoMove &move);


    private:
        JsonConfig config;

        unsigned int board_size, padding;
        unsigned int width, height, channels;

        unsigned int top_moves_count;
        unsigned int moves_decay;

    private:
        DQN dqn;
        std::vector<float> q_values;
};

#endif

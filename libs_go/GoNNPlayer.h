#ifndef _GO_NN_PLAYER_H_
#define _GO_NN_PLAYER_H_

#include "GoPlayer.h"
#include <string>
#include <json_config.h>

#include <cnn.h>

class GoNNPlayer: public GoPlayer
{
    public:
        GoNNPlayer(std::string config_file_name, unsigned int board_size);
        virtual ~GoNNPlayer();

        void new_game();

    private:
        GoMove choose_move();

        std::vector<float> create_nn_input();
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
        CNN *nn;
        std::vector<float> nn_output;
};

#endif

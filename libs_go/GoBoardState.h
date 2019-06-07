#ifndef _GO_BOARD_STATE_H_
#define _GO_BOARD_STATE_H_

#include "go_defs.h"
#include "GoMove.h"
#include <dataset_interface.h>

class GoBoardState
{
    public:
        GoBoardState();
        GoBoardState(GoBoard &board, GoMove &move_now, GoMove &move_next, int winner = GO_PLAYER_EMPTY);
        GoBoardState(const GoBoardState &rhs);
        GoBoardState& operator =(const GoBoardState &rhs);

        virtual ~GoBoardState();

        void set(const GoBoard &board, const GoMove &move_now, const GoMove &move_next, int winner = GO_PLAYER_EMPTY);
        void set(const GoBoardState &board_state);

        sDatasetItem get(unsigned int padding, unsigned int rotation, float noise_level = 0.0);
        sDatasetItem get_winner_value(unsigned int padding, unsigned int rotation, float noise_level = 0.0);

        unsigned int get_rotations_count()
        {
            return 8;
        }

        std::string ToString();
        void print();

    private:

        void board_to_input(    std::vector<float> &input,
                                std::vector<std::vector<unsigned char>> &board,
                                GoMove &move,
                                unsigned int rotation, unsigned int padding, float noise_level = 0.0);

        void moves_to_output(std::vector<float> &output, GoMove &move, unsigned int rotation);
        sGoPoint augmentation(int x, int y, unsigned int rotation);

    private:
        unsigned int m_board_size;
        unsigned int width, height, channels;

        std::vector<std::vector<unsigned char>> m_board;

        GoMove m_move_now;
        GoMove m_move_next;

        int m_winner;
};

#endif

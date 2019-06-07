#ifndef _GO_LEGAL_MOVE_CHECK_H_
#define _GO_LEGAL_MOVE_CHECK_H_

#include <GoMove.h>
#include <vector>

class GoLegalMoveCheck
{
    public:
        GoLegalMoveCheck(unsigned int size = 1);
        GoLegalMoveCheck(GoLegalMoveCheck& other);
        GoLegalMoveCheck(const GoLegalMoveCheck& other);

        virtual ~GoLegalMoveCheck();

        GoLegalMoveCheck& operator= (GoLegalMoveCheck& other);
        GoLegalMoveCheck& operator= (const GoLegalMoveCheck& other);

    private:
        void copy(GoLegalMoveCheck& other);
        void copy(const GoLegalMoveCheck& other);

    public:
        void set_size(unsigned int size);

        void check( int player,
                    GoMove &move,
                    GoBoard &board_now,
                    GoBoard &board_old);

        unsigned int get_legal_moves_count();
        std::vector<GoMove>& get_legal_moves();
        bool is_legal_move(GoMove &move);

    private:
        void init_variables();

        int count_neighbour_fields(int x, int y, int player);
        int count_freedom_fields(int x, int y, int player);
        void compute_freedoms();
        int count_captured(int player);
        void capture_stones(int player);

        bool compare_board(GoBoard &board_a, GoBoard &board_b);
        bool near_move(int prev_x, int prev_y, int x, int y);

    private:
        unsigned int size;

        std::vector<GoMove> legal_moves;

        GoBoard   board;
        std::vector<std::vector<bool>>  legal;
        std::vector<std::vector<bool>>  freedoms;
        GoBoard   territory;
};

#endif

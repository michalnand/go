#ifndef _GOBAN_H_
#define _GOBAN_H_

#include <string>
#include <vector>

#include "go_defs.h"
#include "GoMove.h"

#include "GoLegalMoveCheck.h"

#include <gl_gui/glvisualisation.h>



class GoPlayer;

class Goban
{
    public:
        Goban(unsigned int size = 19, float komi = 6.5);
        Goban(std::string json_file_name);

        Goban(Goban& other);
        Goban(const Goban& other);

        virtual ~Goban();

        Goban& operator= (Goban& other);
        Goban& operator= (const Goban& other);

    public:
        void clear();
        unsigned int get_size();
        float get_komi();
        int get_active_player();
        float get_score();

        GoBoard& get_board();

        unsigned int get_moves_count();
        int get_result();

        bool is_legal_move(GoMove &move);

    public:
        unsigned int get_legal_moves_count();
        std::vector<GoMove>& get_legal_moves();

        void play_move_no_check(GoMove &move);
        int play_move(GoMove &move);

        void load(std::string json_file_name);
        void save(std::string json_file_name);
        std::string asString();
        void print();
        void render(unsigned int delay_time = 300, bool verbose = false);

        float compute_score();

    protected:
        void copy(Goban& other);
        void copy(const Goban& other);

    private:
        void init_variables();

        int count_neighbour_fields(int x, int y, int player);
        int count_freedom_fields(int x, int y, int player);
        int max_abs_value(int x, int y);

        void compute_freedoms();
        void capture_stones(int player);

    private:
        GoLegalMoveCheck legal_move_check;

        unsigned int size;
        float komi;

        int active_player;

        GoMove move;

        GoBoard  board, board_prev_1, board_prev_2;

        std::vector<std::vector<bool>>  freedoms;
        std::vector<std::vector<int>>   territory;

        float score;
        unsigned int moves_count;
        int result;

    private:
        GLVisualisation visualisation;
};


#endif

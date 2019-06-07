#ifndef _GO_TRIAL_H_
#define _GO_TRIAL_H_

#include "GoPlayer.h"
#include "Goban.h"

#include <string>

struct sGoPlayerResult
{
    unsigned int wins;
    unsigned int wins_black;
    unsigned int wins_white;

    unsigned int lose;
    unsigned int lose_black;
    unsigned int lose_white;

    float win_ratio;
};

struct sGoTrialResult
{
    sGoPlayerResult player_a, player_b, winner;
};

class GoTrial
{
    public:
        GoTrial();
        virtual ~GoTrial();

        void process(   GoPlayer &player_a,
                        GoPlayer &player_b,
                        unsigned int count,
                        unsigned int board_size,
                        float komi,
                        bool visualisation = false);

        sGoTrialResult get();
        std::string asString(bool verbose = false);
        void print();

    private:
        void init_result();
        sGoPlayerResult init_player_result();

        float play_match(   GoPlayer &player_a,
                            GoPlayer &player_b,
                            Goban &goban,
                            bool visualisation = false);

    private:
        sGoTrialResult result;
};

#endif

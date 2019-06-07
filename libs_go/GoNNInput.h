#ifndef _GO_NN_INPUT_H_
#define _GO_NN_INPUT_H_

#include "go_defs.h"

class GoNNInput
{
    public:
        GoNNInput();
        virtual ~GoNNInput();

        std::vector<float> create(  GoBoard &board,
                                    int current_player,
                                    unsigned int padding = 0);
};

#endif

#include "GoBoardState.h"
#include <iostream>


GoBoardState::GoBoardState()
{
    channels = 4;
}

GoBoardState::GoBoardState(GoBoard &board, GoMove &move_now, GoMove &move_next, int winner)
{
    set(board, move_now, move_next, winner);
}

GoBoardState::GoBoardState(const GoBoardState &rhs)
{
    set(rhs);
}

GoBoardState& GoBoardState::operator =(const GoBoardState &rhs)
{
    set(rhs);
    return *this;
}

GoBoardState::~GoBoardState()
{

}

void GoBoardState::set(const GoBoard &board, const GoMove &move_now, const GoMove &move_next, int winner)
{
    this->m_board_size = board.size();

    m_board.resize(m_board_size);

    for (unsigned int j = 0; j < m_board_size; j++)
    {
        m_board[j].resize(m_board_size);

        for (unsigned int i = 0; i < m_board_size; i++)
        {
            switch (board[j][i])
            {
                case GO_PLAYER_BLACK: m_board[j][i] = GO_PLAYER_BLACK_UCHAR; break;
                case GO_PLAYER_WHITE: m_board[j][i] = GO_PLAYER_WHITE_UCHAR; break;
                default             : m_board[j][i] = GO_PLAYER_EMPTY_UCHAR;
            }
        }
    }

    this->m_move_now  = move_now;
    this->m_move_next = move_next;
    this->m_winner    = winner;

    width    = m_board_size;
    height   = m_board_size;
    channels = 4;
}


void GoBoardState::set(const GoBoardState &board_state)
{
    this->m_board_size = board_state.m_board_size;

    m_board.resize(m_board_size);

    for (unsigned int j = 0; j < m_board_size; j++)
    {
        m_board[j].resize(m_board_size);

        for (unsigned int i = 0; i < m_board_size; i++)
            m_board[j][i] = board_state.m_board[j][i];
    }

    this->m_move_now    = board_state.m_move_now;
    this->m_move_next   = board_state.m_move_next;
    this->m_winner      = board_state.m_winner;

    width    = m_board_size;
    height   = m_board_size;
    channels = 4;
}

sGoDatasetItem GoBoardState::get(unsigned int padding, unsigned int rotation, float noise_level)
{
    sGoDatasetItem result;

    width    = m_board_size + 2*padding;
    height   = m_board_size + 2*padding;
    channels = 4;

    result.input.resize(width*height*channels);
    result.output.resize(m_board_size*m_board_size + 1);

    board_to_input(result.input, m_board, m_move_now, rotation, padding, noise_level);
    moves_to_output(result.output, m_move_next, rotation);

    return result;
}


sGoDatasetItem GoBoardState::get_winner_value(unsigned int padding, unsigned int rotation, float noise_level)
{
    sGoDatasetItem result;

    width    = m_board_size + 2*padding;
    height   = m_board_size + 2*padding;
    channels = 4;

    result.input.resize(width*height*channels);
    result.output.resize(2);

    board_to_input(result.input, m_board, m_move_now, rotation, padding, noise_level);

    if (m_winner == GO_PLAYER_BLACK)
    {
        result.output[0] = 1.0;
        result.output[1] = 0.0;
    }
    else
    {
        result.output[0] = 0.0;
        result.output[1] = 1.0;
    }

    return result;
}

std::string GoBoardState::ToString()
{
    std::string result;

    for (unsigned int j = 0; j < m_board_size; j++)
    {
        for (unsigned int i = 0; i < m_board_size; i++)
        {
            std::string s;

            switch (m_board[j][i])
            {
                case GO_PLAYER_EMPTY_UCHAR: s = "."; break;
                case GO_PLAYER_BLACK_UCHAR: s = "B"; break;
                case GO_PLAYER_WHITE_UCHAR: s = "W"; break;
            }

            result+= s + " ";
        }

        result+= "\n";
    }

    result+= std::to_string(m_move_now.player()) + " ";
    result+= std::to_string(m_move_now.x()) + " ";
    result+= std::to_string(m_move_now.y()) + " ";

    result+= std::to_string(m_move_next.player()) + " ";
    result+= std::to_string(m_move_next.x()) + " ";
    result+= std::to_string(m_move_next.y()) + " ";

    result+= std::to_string(m_winner) + " ";

    result+= "\n\n";

    return result;
}

void GoBoardState::print()
{
    std::string result = this->ToString();
    std::cout << result;
}



void GoBoardState::board_to_input(  std::vector<float> &input,
                                    std::vector<std::vector<unsigned char>> &board, GoMove &move,
                                    unsigned int rotation, unsigned int padding,
                                    float noise_level)
{
    for (unsigned int i = 0; i < input.size(); i++)
        input[i] = 0.0;

    for (unsigned int y = 0; y < m_board_size; y++)
    for (unsigned int x = 0; x < m_board_size; x++)
    {
        sGoPoint rotated = augmentation(x, y, rotation);
        unsigned char in = board[rotated.y][rotated.x];

        unsigned int ch = 0;

        if (in == GO_PLAYER_BLACK_UCHAR)
            ch = 0;   //black stones layer
        else if (in == GO_PLAYER_WHITE_UCHAR)
            ch = 1;   //white stones layer
        else if (in == GO_PLAYER_EMPTY)
            ch = 2;   //empty fields layer

        unsigned int result_idx = (ch*height + y + padding)*width + x + padding;
        input[result_idx] = 1.0;
    }

    //current player layer, 1 for black, 0 for white
    if (move.player() == GO_PLAYER_BLACK)
    {
        for (unsigned int y = 0; y < m_board_size; y++)
        for (unsigned int x = 0; x < m_board_size; x++)
        {
            unsigned int ch = 3;
            unsigned int side_idx = (ch*height + y + padding)*width + x + padding;
            input[side_idx] = 1.0;
        }
    }

    //add noise
    if (noise_level > 0.0)
    for (unsigned int i = 0; i < input.size(); i++)
    {
        float rndf = (rand()%10000)/10000.0;

        if (rand()%2)
            rndf = -rndf;

        input[i] = (1.0 - noise_level)*input[i] + noise_level*rndf;
    }
}

void GoBoardState::moves_to_output(std::vector<float> &output, GoMove &move, unsigned int rotation)
{
    for (unsigned int i = 0; i < output.size(); i++)
        output[i] = 0.0;

    if (move.pass())
    {
        unsigned int idx = m_board_size*m_board_size;
        output[idx] = 1.0;
    }
    else
    {
        sGoPoint rotated = augmentation(move.x(), move.y(), rotation);
        unsigned int idx = rotated.y*m_board_size + rotated.x;

        if (idx < m_board_size*m_board_size)
            output[idx] = 1.0;
    }
}


sGoPoint GoBoardState::augmentation(int x, int y, unsigned int rotation)
{
    sGoPoint result;

    result.x = x;
    result.y = y;

    unsigned int size = m_board_size - 1;

    switch (rotation)
    {
        case 0:
                result.x = x;
                result.y = y;
                break;

        case 1:
                result.x = size - x;
                result.y = y;
                break;

        case 2:
                result.x = x;
                result.y = size - y;
                break;

        case 3:
                result.x = size - x;
                result.y = size - y;
                break;

        case 4:
                result.x = y;
                result.y = x;
                break;

        case 5:
                result.x = y;
                result.y = size - x;
                break;

        case 6:
                result.x = size - y;
                result.y = x;
                break;

        case 7:
                result.x = size - y;
                result.y = size - x;
                break;
    }

    return result;
}

#include "GoLegalMoveCheck.h"
#include <iostream>

GoLegalMoveCheck::GoLegalMoveCheck(unsigned int size)
{
    this->size = size;
    init_variables();
}

GoLegalMoveCheck::GoLegalMoveCheck(GoLegalMoveCheck& other)
{
    copy(other);
}

GoLegalMoveCheck::GoLegalMoveCheck(const GoLegalMoveCheck& other)
{
    copy(other);
}

GoLegalMoveCheck::~GoLegalMoveCheck()
{

}

GoLegalMoveCheck& GoLegalMoveCheck::operator= (GoLegalMoveCheck& other)
{
    copy(other);
    return *this;
}

GoLegalMoveCheck& GoLegalMoveCheck::operator= (const GoLegalMoveCheck& other)
{
    copy(other);
    return *this;
}


void GoLegalMoveCheck::copy(GoLegalMoveCheck& other)
{
    size = other.size;

    legal_moves = other.legal_moves;
    board       = other.board;
    legal       = other.legal;
    freedoms    = other.freedoms;
    territory   = other.territory;
}

void GoLegalMoveCheck::copy(const GoLegalMoveCheck& other)
{
    size = other.size;

    legal_moves = other.legal_moves;
    board       = other.board;
    legal       = other.legal;
    freedoms    = other.freedoms;
    territory   = other.territory;
}

void GoLegalMoveCheck::set_size(unsigned int size)
{
    this->size = size;
    init_variables();
}

void GoLegalMoveCheck::check( int player,
                              GoMove &move,
                              GoBoard &board_now,
                              GoBoard &board_old)
{
    board = board_now;

    //only empty fields are legal
    for (unsigned int y = 0; y < board.size(); y++)
    for (unsigned int x = 0; x < board[y].size(); x++)
    {
        if (board[y][x] == GO_PLAYER_EMPTY)
            legal[y][x] = true;
        else
            legal[y][x] = false;
    }

    //avoid suicide
    for (unsigned int y = 0; y < board.size(); y++)
    for (unsigned int x = 0; x < board[y].size(); x++)
    if (board[y][x] == GO_PLAYER_EMPTY)
    if (count_neighbour_fields(x, y, GO_PLAYER_EMPTY) == 0)
    {
        board[y][x] = player;

        compute_freedoms();

        int black_captured = count_captured(GO_PLAYER_BLACK);
        int white_captured = count_captured(GO_PLAYER_WHITE);

        if (player == GO_PLAYER_BLACK)
        {
            if ((black_captured != 0)&&(white_captured == 0))
                legal[y][x] = false;
        }
        else
        {
            if ((white_captured != 0)&&(black_captured == 0))
                legal[y][x] = false;
        }

        board[y][x] = GO_PLAYER_EMPTY;
    }

    bool ko_warning = false;
    int ko_y = -1;
    int ko_x = -1;

    //avoid ko
    for (unsigned int y = 0; y < board.size(); y++)
    for (unsigned int x = 0; x < board[y].size(); x++)
    if (legal[y][x] == true)
    //go possible only when near move played
    if (near_move(move.x(), move.y(), x, y))
    {
        auto board_tmp = board;

        board[y][x] = player;

        compute_freedoms();

        if (player == GO_PLAYER_BLACK)
            capture_stones(GO_PLAYER_WHITE);
        else
            capture_stones(GO_PLAYER_BLACK);

        if (compare_board(board, board_old) == true)
        {
            legal[y][x] = false;

            ko_warning = true;
            ko_y = y;
            ko_x = x;
        }

        board = board_tmp;
	}

    if (ko_warning)
    {
        std::cout << "KO warning\n";

        for (unsigned int y = 0; y < board.size(); y++)
        {
            for (unsigned int x = 0; x < board[y].size(); x++)
            {
                std::string result;
                switch (board[y][x])
    			{
    				case GO_PLAYER_EMPTY: result = ". "; break;
    				case GO_PLAYER_BLACK: result = "B "; break;
    				case GO_PLAYER_WHITE: result = "W "; break;
    			}
 
                if (((int)y == ko_y)&&((int)x == ko_x))
                    result = "A ";

                std::cout << result;
            }
            std::cout << "\n";
        }

        std::cout << "\n\n";
    }


	legal_moves.clear();

    //add pass move
    GoMove tmp_move(true, player);
    legal_moves.push_back(tmp_move);

    //add other legal moves
    for (unsigned int y = 0; y < board.size(); y++)
    for (unsigned int x = 0; x < board[y].size(); x++)
    if (legal[y][x] == true)
    {
        GoMove tmp_move(x, y, player);
        legal_moves.push_back(tmp_move);
    }
}

unsigned int GoLegalMoveCheck::get_legal_moves_count()
{
    return legal_moves.size();
}

std::vector<GoMove>& GoLegalMoveCheck::get_legal_moves()
{
    return legal_moves;
}

bool GoLegalMoveCheck::is_legal_move(GoMove &move)
{
    for (unsigned int i = 0; i < legal_moves.size(); i++)
        if (move == legal_moves[i])
            return true;

    return false;
}



void GoLegalMoveCheck::init_variables()
{
    board.resize(size);

    for (unsigned int y = 0; y < board.size(); y++)
        board[y].resize(size);

    for (unsigned int y = 0; y < board.size(); y++)
    for (unsigned int x = 0; x < board[y].size(); x++)
        board[y][x] = GO_PLAYER_EMPTY;

    legal.resize(size);
    for (unsigned int y = 0; y < legal.size(); y++)
        legal[y].resize(size);

    for (unsigned int y = 0; y < legal.size(); y++)
    for (unsigned int x = 0; x < legal[y].size(); x++)
        legal[y][x] = true;

    freedoms.resize(size);

    for (unsigned int y = 0; y < freedoms.size(); y++)
        freedoms[y].resize(size);

    for (unsigned int y = 0; y < freedoms.size(); y++)
	for (unsigned int x = 0; x < freedoms[y].size(); x++)
        freedoms[y][x] = true;

    territory.resize(size);

    for (unsigned int y = 0; y < territory.size(); y++)
    {
        territory[y].resize(size);
    }

	for (unsigned int y = 0; y < territory.size(); y++)
	for (unsigned int x = 0; x < territory[y].size(); x++)
		territory[y][x] = 0;
}

int GoLegalMoveCheck::count_neighbour_fields(int x, int y, int player)
{
 	int result = 0;

    if (x > 0)
    if (board[y][x-1] == player)
        result++;

    if (x+1 < (int)size)
    if (board[y][x+1] == player)
        result++;

    if (y > 0)
    if (board[y-1][x] == player)
        result++;

    if (y+1 < (int)size)
    if (board[y+1][x] == player)
        result++;

    return result;
}

int GoLegalMoveCheck::count_freedom_fields(int x, int y, int player)
{
    int result = count_neighbour_fields(x, y, GO_PLAYER_EMPTY);

    if (x > 0)
	   if (board[y][x-1] == player)
	      if (freedoms[y][x-1])
            result++;

	if (x+1 < (int)size)
	   if (board[y][x+1] == player)
	      if (freedoms[y][x+1])
            result++;

	if (y > 0)
	   if (board[y-1][x] == player)
	      if (freedoms[y-1][x])
            result++;

	if (y+1 < (int)size)
        if (board[y+1][x] == player)
            if (freedoms[y+1][x])
                result++;

    return result;
}


void GoLegalMoveCheck::compute_freedoms()
{
	for (unsigned int y = 0; y < board.size(); y++)
	for (unsigned int x = 0; x < board[y].size(); x++)
	if (board[y][x] == GO_PLAYER_EMPTY)
	 	freedoms[y][x] = true;
	else
	 	freedoms[y][x] = false;

	bool change = true;

    while (change)
	{
	 	change = false;

		for (unsigned int y = 0; y < board.size(); y++)
		for (unsigned int x = 0; x < board[y].size(); x++)
		if (board[y][x] != GO_PLAYER_EMPTY)
	 	if (freedoms[y][x] == false)
		{
		  if (count_freedom_fields(x, y, board[y][x]) > 0)
		  {
			freedoms[y][x] = true;
			change = true;
		  }
		}
	}
}

int GoLegalMoveCheck::count_captured(int player)
{
    int result = 0;

	for (unsigned int y = 0; y < board.size(); y++)
	for (unsigned int x = 0; x < board[y].size(); x++)
	if (board[y][x] == player)
	if (freedoms[y][x] == false)
		result++;

	return result;
}

void GoLegalMoveCheck::capture_stones(int player)
{
	for (unsigned int y = 0; y < board.size(); y++)
	for (unsigned int x = 0; x < board[y].size(); x++)
	if (board[y][x] == player)
	if (freedoms[y][x] == false)
		board[y][x] = GO_PLAYER_EMPTY;
}

bool GoLegalMoveCheck::compare_board(GoBoard &board_a, GoBoard &board_b)
{
	for (unsigned int y = 0; y < size; y++)
	for (unsigned int x = 0; x < size; x++)
		if (board_a[y][x] != board_b[y][x])
			return false;

	return true;
}

bool GoLegalMoveCheck::near_move(int prev_x, int prev_y, int x, int y)
{
	if ((abs(prev_x - x) <= 1) && (abs(prev_y - y) <= 1))
		return true;

    return false;
}

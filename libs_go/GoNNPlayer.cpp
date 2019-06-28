#include "GoNNPlayer.h"

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>


GoNNPlayer::GoNNPlayer(std::string config_file_name, unsigned int board_size)
	    :GoPlayer()
{
	config.load(config_file_name);

	this->board_size 	= board_size;
	padding 			= config.result["padding"].asInt();
	top_moves_count 	= config.result["top_moves_count"].asInt();
	moves_decay			= config.result["moves_decay"].asInt();

	width 	= board_size + 2*padding;
	height 	= board_size + 2*padding;
	depth = 4;

	Shape input_shape(width, height, depth);
	Shape output_shape(1, 1, board_size*board_size + 1);

	nn = new CNN(config.result["network_config_file_name"].asString(), input_shape, output_shape);
	nn_output.resize(output_shape.size());
}

GoNNPlayer::~GoNNPlayer()
{
	if (nn != nullptr)
		delete nn;
}

void GoNNPlayer::new_game()
{
	moves_decay				=	config.result["moves_decay"].asInt();
	top_moves_count 	= config.result["top_moves_count"].asInt();
}

GoMove GoNNPlayer::choose_move()
{
	auto nn_input = create_nn_input();

	nn->forward(nn_output, nn_input);

	auto top_moves = find_top_moves();


	if (moves_decay > 0)
	{
		moves_decay--;
	}
	else
	{
		top_moves_count = 1;
	}


	//if there is no move choose only pass move
	if (top_moves.size() == 0)
	{
		GoMove pass_move(true, current_player);
		move = pass_move;
	}
	else
	{
		move = top_moves[rand()%top_moves.size()];
	}


	return move;
}


std::vector<float> GoNNPlayer::create_nn_input()
{
	std::vector<float> result(depth*height*width);


	for (unsigned int i = 0; i < result.size(); i++)
		result[i] = 0.0;

	for (unsigned int y = 0; y < board_size; y++)
	for (unsigned int x = 0; x < board_size; x++)
	{
		int in = board[y][x];

		unsigned int ch = 0;

		if (in == GO_PLAYER_BLACK)
			ch = 0;   //black stones layer
		else
		if (in == GO_PLAYER_WHITE)
			ch = 1;   //white stones layer
		else
		if (in == GO_PLAYER_EMPTY)
			ch = 2;   //empty fields layer

		unsigned int result_idx = (ch*height + y + padding)*width + x + padding;
		result[result_idx] = 1.0;
	}

	//current player layer, 1 for black, 0 for white
	if (current_player == GO_PLAYER_BLACK)
	{
		for (unsigned int y = 0; y < board_size; y++)
		for (unsigned int x = 0; x < board_size; x++)
		{
			unsigned int ch = 3;
			unsigned int side_idx = (ch*height + y + padding)*width + x + padding;
			result[side_idx] = 1.0;
		}
	}

	return result;
}


std::vector<GoMove> GoNNPlayer::find_top_moves()
{
	std::vector<GoMove> result;

	//find top top N only legal moves
	auto top_n = get_top_n(nn_output, top_moves_count);

	//idx to moves
	for (auto idx: top_n)
	{
		int x = idx%board_size;
		int y = idx/board_size;
		GoMove move(x, y, current_player);

		result.push_back(move);
	}

	return result;
}




std::vector<unsigned int> GoNNPlayer::get_top_n(std::vector<float> &confidence, unsigned int top_n_count)
{
	std::vector<unsigned int> result;

	for (unsigned int j = 0; j < top_n_count; j++)
	{
		float max 	= -1000000000.0;
		int max_idx = -1;

		for (unsigned int i = 0; i < confidence.size(); i++)
		{
			GoMove move(i%board_size, i/board_size, current_player);

			if (confidence[i] > max)
			if (not_in(result, i))
			if (is_legal(move))
			{
				max = confidence[i];
				max_idx = i;
			}
		}

		if (max_idx != -1)
			result.push_back(max_idx);
	}

	return result;
}

bool GoNNPlayer::not_in(std::vector<unsigned int> &vect, unsigned int value)
{
	for (unsigned int i = 0; i < vect.size(); i++)
		if (value == vect[i])
			return false;

	return true;
}

bool GoNNPlayer::is_legal(GoMove &move)
{
	for (unsigned int i = 0; i < legal_moves.size(); i++)
		if (move == legal_moves[i])
			return true;

	return false;
}

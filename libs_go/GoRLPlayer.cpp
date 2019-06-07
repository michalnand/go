#include "GoRLPlayer.h"

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>


GoRLPlayer::GoRLPlayer(std::string config_file_name, unsigned int board_size)
	    		 :GoPlayer()
{
	config.load(config_file_name);

	this->board_size	= board_size;
	padding 					= config.result["padding"].asInt();
	top_moves_count 	= config.result["top_moves_count"].asInt();
	moves_decay				=	config.result["moves_decay"].asInt();

	width 		= board_size + 2*padding;
	height 		= board_size + 2*padding;
	channels 	= 4;

	unsigned int actions_count = board_size*board_size + 1;

	unsigned int experience_buffer_size 	= config.result["dqn"]["experience_buffer_size"].asInt();
	float gamma 													= config.result["dqn"]["gamma"].asFloat();
	std::string network_config_file_name 	= config.result["dqn"]["network_config_file_name"].asString();


	sGeometry state_geometry;
	state_geometry.w = width;
	state_geometry.h = height;
	state_geometry.d = channels;

	JsonConfig dqn_json(network_config_file_name);

	dqn.init(  dqn_json.result,
						 gamma,
						 state_geometry,
						 actions_count,
						 experience_buffer_size,
		false);

	q_values.resize(actions_count);
}

GoRLPlayer::~GoRLPlayer()
{

}


void GoRLPlayer::set_result(bool winner)
{
	float reward = 0.0;

	if (winner)
		reward = 1.0;
	else
		reward = -1.0;

	auto state = create_nn_input(board);
	dqn.add_final(state, q_values, move.move_to_idx(board_size), reward);
}


void GoRLPlayer::learn()
{
	dqn.learn();
}

void GoRLPlayer::test()
{
	dqn.test();
}

DQNCompare& GoRLPlayer::get_compare_result()
{
	return dqn.get_compare_result();
}

void GoRLPlayer::new_batch()
{
	dqn.new_batch();
}

void GoRLPlayer::print()
{
	dqn.print();
}


GoMove GoRLPlayer::choose_move()
{
	auto state = create_nn_input(board);
	dqn.compute_q_values(state);

	q_values = dqn.get_q_values();

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

	dqn.add(state, q_values, move.move_to_idx(board_size), 0.0);

	return move;
}


std::vector<float> GoRLPlayer::create_nn_input(GoBoard &board)
{
	std::vector<float> result(channels*height*width);


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




std::vector<GoMove> GoRLPlayer::find_top_moves()
{
	std::vector<GoMove> result;

	//find top top N only legal moves
	auto top_n = get_top_n(q_values, top_moves_count);

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




std::vector<unsigned int> GoRLPlayer::get_top_n(std::vector<float> &confidence, unsigned int top_n_count)
{
  std::vector<unsigned int> result;

  for (unsigned int j = 0; j < top_n_count; j++)
  {
    float max = -1000000000.0;
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

bool GoRLPlayer::not_in(std::vector<unsigned int> &vect, unsigned int value)
{
  for (unsigned int i = 0; i < vect.size(); i++)
    if (value == vect[i])
      return false;

  return true;
}

bool GoRLPlayer::is_legal(GoMove &move)
{
		for (unsigned int i = 0; i < legal_moves.size(); i++)
			if (move == legal_moves[i])
				return true;

		return false;
}

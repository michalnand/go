#include "Goban.h"
#include <sstream>
#include <iomanip>
#include <iostream>
#include <json_config.h>
#include <math.h>
#include <timer.h>

Goban::Goban(unsigned int size, float komi)
{
	this->size = size;
	this->komi = komi;

	init_variables();
	legal_move_check.check(active_player, move, board, board_prev_2);
}


Goban::Goban(std::string json_file_name)
{
	load(json_file_name);
}





Goban::Goban(Goban& other)
{
	copy(other);
}

Goban::Goban(const Goban& other)
{
	copy(other);
}

Goban::~Goban()
{

}

Goban& Goban::operator= (Goban& other)
{
	copy(other);
	return *this;
}

Goban& Goban::operator= (const Goban& other)
{
	copy(other);
	return *this;
}

void Goban::clear()
{
	init_variables();
	legal_move_check.check(active_player, move, board, board_prev_2);
}

unsigned int Goban::get_size()
{
	return size;
}

float Goban::get_komi()
{
	return komi;
}

int Goban::get_active_player()
{
	return active_player;
}

float Goban::get_score()
{
	return score;
}

GoBoard& Goban::get_board()
{
	return board;
}

unsigned int Goban::get_moves_count()
{
	return moves_count;
}

int Goban::get_result()
{
	return result;
}

unsigned int Goban::get_legal_moves_count()
{
	return legal_move_check.get_legal_moves_count();
}

std::vector<GoMove>& Goban::get_legal_moves()
{
	return legal_move_check.get_legal_moves();
}

bool Goban::is_legal_move(GoMove &move)
{
	return legal_move_check.is_legal_move(move);
}


void Goban::play_move_no_check(GoMove &move)
{
	if (move.pass() == false)
		board[move.y()][move.x()] = move.player();

	active_player = move.player();

	compute_freedoms();
	capture_stones(active_player);
}

int Goban::play_move(GoMove &move)
{
	if (get_legal_moves_count() == 0)
	{
		result = GOBAN_NO_LEGAL_MOVES;
		return result;
	}

	if (is_legal_move(move) == false)
	{
		if (active_player == GO_PLAYER_BLACK)
			result = GOBAN_BLACK_ILLEGAL_MOVE;
		else
			result = GOBAN_WHITE_ILLEGAL_MOVE;

		return result;
	}

	this->move = move;
	board_prev_2 = board_prev_1;
	board_prev_1 = board;

	if (move.pass() == false)
		board[move.y()][move.x()] = move.player();

	compute_freedoms();
	capture_stones(active_player);

	if (active_player == GO_PLAYER_BLACK)
		active_player = GO_PLAYER_WHITE;
	else
		active_player = GO_PLAYER_BLACK;

	legal_move_check.check(active_player, move, board, board_prev_2);

	moves_count++;

	if (move.pass())
	{
		result = GOBAN_PASS;
		return result;
	}

	if (fabs(compute_score()) > size*size*0.70)
	{
		result = GOBAN_GAME_END_AREA;
		return result;
	}

	if (moves_count > size*size*0.8)
	{
		result = GOBAN_GAME_END_MOVES;
		return result;
	}

	result = GOBAN_GAME_PLAYING;
	return result;
}

void Goban::load(std::string json_file_name)
{
	JsonConfig json(json_file_name);

	size 						= json.result["size"].asInt();
	komi 						= json.result["komi"].asFloat();

	init_variables();

	active_player = json.result["active_player"].asInt();

	for (unsigned int j = 0; j < size; j++)
	for (unsigned int i = 0; i < size; i++)
		board[j][i] = json.result["board"][j][i].asInt();

	board_prev_1 = board;
	board_prev_2 = board;

	legal_move_check.check(active_player, move, board, board_prev_2);
}

void Goban::save(std::string json_file_name)
{
	JsonConfig json;

	json.result["size"] 					= size;
	json.result["komi"] 					= komi;
	json.result["active_player"] 	= active_player;

	for (unsigned int j = 0; j < size; j++)
		for (unsigned int i = 0; i < size; i++)
			json.result["board"][j][i] = board[j][i];

	json.save(json_file_name);
}


std::string Goban::asString()
{
	std::string result;

	for (unsigned int j = 0; j < size; j++)
	{
		for (unsigned int i = 0; i < size; i++)
		{
			switch (board[j][i])
			{
				case GO_PLAYER_EMPTY: result+= ". "; break;
				case GO_PLAYER_BLACK: result+= "B "; break;
				case GO_PLAYER_WHITE: result+= "W "; break;
			}
		}

		result+= "\n";
	}

	return result;
}

void Goban::print()
{
	std::cout << asString() << "\n";
}

void Goban::render(unsigned int delay_time, bool verbose)
{
	compute_score();

	visualisation.start();

	float element_size = 1.0/size;
	if (element_size > 0.09)
		element_size = 0.09;

	float width  = 2.0 + element_size*3.0;
	float height = 2.0 + element_size*3.0;
	float back_width  = 1.7*width;
	float back_height = 1.7*height;

	visualisation.push();
	visualisation.translate(0.0, 0.0, 0.01);
	visualisation.paint_textured_rectangle(back_width, back_height, 1);
	visualisation.pop();

	visualisation.push();

	std::stringstream stream;
	stream << "score = ";
	stream << std::fixed;
	stream << std::setprecision(1);
	stream << get_score();
	std::string score = stream.str();

	visualisation.translate(-0.2, 1.15, 0.01);
	visualisation.set_color(0.0, 0.0, 0.0);
	visualisation.print(0, 0, 0, score);
	visualisation.pop();

	visualisation.push();
	visualisation.translate(0.0, 0.0, 0.02);
	visualisation.paint_textured_rectangle(width, height, 2);
	visualisation.pop();

	for (unsigned int j = 0; j < size; j++)
	{
		float y0 = (0*1.0/(size - 1) - 0.5)*2.0;
		float x0 = (j*1.0/(size - 1) - 0.5)*2.0;
		float y1 = ((size-1)*1.0/(size - 1) - 0.5)*2.0;
		float x1 = (j*1.0/(size - 1) - 0.5)*2.0;

		visualisation.push();
		visualisation.set_color(0, 0, 0);
		visualisation.paint_line(x0, y0, 0.03, x1, y1, 0.03);
		visualisation.pop();

		visualisation.push();
		visualisation.set_color(0, 0, 0);
		visualisation.paint_line(y0, x0, 0.03, y1, x1, 0.03);
		visualisation.pop();

	}

	for (unsigned int j = 0; j < size; j++)
	for (unsigned int i = 0; i < size; i++)
	if (board[j][i] != GO_PLAYER_EMPTY)
	{
		float cr, cg, cb;
		if (board[j][i] == GO_PLAYER_BLACK)
		{
			cr = 0.1;
			cg = 0.1;
			cb = 0.1;
		}
		else
		{
			cr = 0.9;
			cg = 0.9;
			cb = 0.9;
		}

		float y = -(j*1.0/(size - 1) - 0.5)*2.0;
		float x = (i*1.0/(size - 1) - 0.5)*2.0;


		visualisation.push();
		visualisation.set_color(cr, cg, cb);
		visualisation.translate(x, y, 0.04);
		visualisation.paint_circle(element_size);
		visualisation.pop();
	}

	if (verbose)
	{
		for (unsigned int j = 0; j < size; j++)
		for (unsigned int i = 0; i < size; i++)
		if (territory[j][i] != 0)
		{
			float y = -(j*1.0/(size - 1) - 0.5)*2.0;
			float x = (i*1.0/(size - 1) - 0.5)*2.0;

			std::string str = std::to_string(territory[j][i]);
			visualisation.push();

			visualisation.translate(x - element_size/2.0, y - element_size/4.0, 0.05);


				float cr, cg, cb;
				if (territory[j][i] > 0)
				{
					cr = 0.1;
					cg = 0.1;
					cb = 0.1;
				}
				else
				{
					cr = 0.9;
					cg = 0.9;
					cb = 0.9;
				}

				visualisation.set_color(cr, cg, cb);
				visualisation.print(0, 0, 0, str, false);
			visualisation.pop();
		}

		for (unsigned int i = 0; i < get_legal_moves_count(); i++)
		{
			auto move = get_legal_moves()[i];
			if (move.pass() == false)
			{
				float y = -(move.y()*1.0/(size - 1) - 0.5)*2.0;
				float x = (move.x()*1.0/(size - 1) - 0.5)*2.0;


				visualisation.push();
				visualisation.set_color(1.0, 0.0, 0.0);
				visualisation.translate(x, y, 0.04);
				visualisation.paint_circle(element_size*0.5);
				visualisation.pop();
			}
		}
	}

	visualisation.finish();

	Timer timer;
	timer.sleep_ms(delay_time);
}

float Goban::compute_score()
{
	score = -komi;

	unsigned int rounds = 4;

	for (unsigned int j = 0; j < size; j++)
	for (unsigned int i = 0; i < size; i++)
	{
		territory[j][i] = 0;

		if (board[j][i] == GO_PLAYER_BLACK)
			territory[j][i] = (int)rounds;

		if (board[j][i] == GO_PLAYER_WHITE)
			territory[j][i] = -(int)rounds;
	}


	for (unsigned int k = 0; k < rounds; k++)
	{
		for (unsigned int j = 0; j < size; j++)
		for (unsigned int i = 0; i < size; i++)
		if (territory[j][i] == 0)
		{
			int v = max_abs_value(i, j);

			if (v > 0)
				v-= 1;
			else
			if (v < 0)
				v+= 1;

			territory[j][i] = v;
		}

	}

	for (unsigned int j = 0; j < size; j++)
	for (unsigned int i = 0; i < size; i++)
	{
		if (territory[j][i] > 0)
			score+= 1.0;

		if (territory[j][i] < 0)
			score-= 1.0;
	}

	return score;
}

void Goban::copy(Goban& other)
{
	size 					= other.size;
	komi 					= other.komi;
	active_player = other.active_player;
	move 					= other.move;
	result				= other.result;

	board 				= other.board;
	board_prev_1 	= other.board_prev_1;
	board_prev_2 	= other.board_prev_2;

	freedoms 			= other.freedoms;
	score 				= other.score;

	legal_move_check = other.legal_move_check;
}

void Goban::copy(const Goban& other)
{
	size 					= other.size;
	komi 					= other.komi;
	active_player = other.active_player;
	move 					= other.move;
	result				= other.result;

	board 				= other.board;
	board_prev_1 	= other.board_prev_1;
	board_prev_2 	= other.board_prev_2;

	freedoms 			= other.freedoms;
	score 				= other.score;

	legal_move_check = other.legal_move_check;
}



void Goban::init_variables()
{
	active_player = GO_PLAYER_BLACK;
	score 				= 0.0;
	moves_count 	= 0;

	result 				= GOBAN_GAME_PLAYING;

	legal_move_check.set_size(size);

	board.resize(size);

	for (unsigned int y = 0; y < board.size(); y++)
		board[y].resize(size);

	for (unsigned int y = 0; y < board.size(); y++)
	for (unsigned int x = 0; x < board[y].size(); x++)
		board[y][x] = GO_PLAYER_EMPTY;

	board_prev_1 = board;
	board_prev_2 = board;

	freedoms.resize(size);

	for (unsigned int y = 0; y < freedoms.size(); y++)
	 freedoms[y].resize(size);

	for (unsigned int y = 0; y < freedoms.size(); y++)
	for (unsigned int x = 0; x < freedoms[y].size(); x++)
		freedoms[y][x] = true;


	territory.resize(size);

	for (unsigned int y = 0; y < territory.size(); y++)
		territory[y].resize(size);

	for (unsigned int y = 0; y < territory.size(); y++)
	for (unsigned int x = 0; x < territory[y].size(); x++)
		territory[y][x] = x + territory.size()*y;
}

int Goban::count_neighbour_fields(int x, int y, int player)
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

int Goban::count_freedom_fields(int x, int y, int player)
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


int Goban::max_abs_value(int x, int y)
{
 	int result = 0;


	if (x > 0)
	{
		if (abs(territory[y][x-1]) > abs(result))
			result = territory[y][x-1];
	}

	if ((x+1) < (int)size)
	{
		if (abs(territory[y][x+1]) > abs(result))
			result = territory[y][x+1];
	}


	if (y > 0)
	{
		if (abs(territory[y-1][x]) > abs(result))
			result = territory[y-1][x];
	}

	if ((y+1) < (int)size)
	{
		if (abs(territory[y+1][x]) > abs(result))
			result = territory[y+1][x];
	}

	return result;
}

void Goban::compute_freedoms()
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

void Goban::capture_stones(int player)
{
	for (unsigned int y = 0; y < board.size(); y++)
	for (unsigned int x = 0; x < board[y].size(); x++)
	if (board[y][x] != player)
	if (freedoms[y][x] == false)
		board[y][x] = GO_PLAYER_EMPTY;
}

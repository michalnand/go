#include "GoSgfPlayer.h"

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>


GoSgfPlayer::GoSgfPlayer(std::string sgf_file_name)
	    :GoPlayer()
{
	winner = GO_PLAYER_EMPTY;

	parse(sgf_file_name);

	current_player = GO_PLAYER_BLACK;
	ptr = 0;
}

GoSgfPlayer::~GoSgfPlayer()
{

}



GoMove GoSgfPlayer::choose_move()
{
	GoMove result(true, current_player);

	if (current_player == GO_PLAYER_BLACK)
		current_player = GO_PLAYER_WHITE;
	else
		current_player = GO_PLAYER_BLACK;

	if (ptr < moves.size())
	{
		result = moves[ptr];
		ptr++;
	}

	return result;
}




void GoSgfPlayer::parse(std::string sgf_file_name)
{
	std::ifstream file(sgf_file_name);
 	std::vector<char> raw;

 	if (file.is_open())
 	{
		while ( !file.eof())
	 	{
			char c;
			file >> c;
			raw.push_back(c);
		}
 	}


 	unsigned int ptr = 0;
 	unsigned int move_record_size = 6;

 	std::string size_19_pattern 	= "SZ[19";
 	std::string size_13_pattern 	= "SZ[13";
 	std::string size_9_pattern 	= "SZ[9";

 	std::string black_pattern = ";B[";
 	std::string white_pattern = ";W[";
 	std::string black_pass_pattern = ";B[]";
 	std::string white_pass_pattern = ";W[]";
	std::string black_pass_pattern_b = ";B[tt]";
	std::string white_pass_pattern_b = ";W[tt]";

	std::string white_win_pattern = "RE[W";
	std::string black_win_pattern = "RE[B";

 	moves.clear();


 	while (ptr <= (raw.size()-move_record_size) )
 	{
		if (is_pattern(raw, black_pass_pattern, ptr))
		{
			GoMove move(true, GO_PLAYER_BLACK);
			move.pass();
			moves.push_back(move);
			ptr+= black_pass_pattern.size();
		}
		else
		if (is_pattern(raw, white_pass_pattern, ptr))
		{
			GoMove move(true, GO_PLAYER_WHITE);
			move.pass();
			moves.push_back(move);
			ptr+= black_pass_pattern.size();
		}
	 else
	 if (is_pattern(raw, black_pass_pattern_b, ptr))
	 {
		 GoMove move(true, GO_PLAYER_BLACK);
		 move.pass();
		 moves.push_back(move);
		 ptr+= black_pass_pattern_b.size();
	 }
	 else
	 if (is_pattern(raw, white_pass_pattern_b, ptr))
	 {
		 GoMove move(true, GO_PLAYER_WHITE);
		 move.pass();
		 moves.push_back(move);
		 ptr+= black_pass_pattern_b.size();
	 }
	else
	 if (is_pattern(raw, size_19_pattern, ptr))
	 {
		 board_size = 19;
		 ptr+= size_19_pattern.size();
	 }
	 else
	 if (is_pattern(raw, size_13_pattern, ptr))
	 {
		 board_size = 13;
		 ptr+= size_13_pattern.size();
	 }
	 else
	 if (is_pattern(raw, size_9_pattern, ptr))
	 {
		 board_size = 9;
		 ptr+= size_9_pattern.size();
	 }
	 else
	 if (is_pattern(raw, black_pattern, ptr))
	 {
		 GoMove move = parse_raw_move(GO_PLAYER_BLACK, raw, ptr);
		 moves.push_back(move);
		 ptr+= move_record_size;
	 }
	 else
	 if (is_pattern(raw, white_pattern, ptr))
	 {
		 GoMove move = parse_raw_move(GO_PLAYER_WHITE, raw, ptr);
		 moves.push_back(move);
		 ptr+= move_record_size;
	 }
	 else
	 if (is_pattern(raw, white_win_pattern, ptr))
	 {
		 winner = GO_PLAYER_WHITE;
		 ptr+= white_win_pattern.size();
	 }
	 else
	 if (is_pattern(raw, black_win_pattern, ptr))
	 {
		 winner = GO_PLAYER_BLACK;
		 ptr+= black_win_pattern.size();
	 }
	 else
		 ptr++;
 }
}


bool GoSgfPlayer::is_pattern(std::vector<char> &raw, std::string &pattern, unsigned int ofs)
{
  for (unsigned int i = 0; i < pattern.size(); i++)
    if (raw[ofs + i] != pattern[i])
      return false;

  return true;
}

GoMove GoSgfPlayer::parse_raw_move(int player, std::vector<char> &raw, unsigned int ofs)
{
  int x = (raw[ofs+3] - 'a');
  int y = (raw[ofs+4] - 'a');

	GoMove result(x, y, player);
  return result;
}

int GoSgfPlayer::get_winner()
{
	return winner;
}

unsigned int GoSgfPlayer::get_moves_count()
{
	return moves.size();
}

GoMove GoSgfPlayer::get_move(unsigned int idx)
{
	return moves[idx];
}

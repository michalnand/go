#include "GoNNInput.h"

GoNNInput::GoNNInput()
{

}

GoNNInput::~GoNNInput()
{

}

std::vector<float> GoNNInput::create( GoBoard &board,
                                      int current_player,
                                      unsigned int padding)
{
  unsigned int board_size = board.size();

  unsigned int height   = board_size + 2*padding;
  unsigned int width    = board_size + 2*padding;
  unsigned int channels = board_size + 2*padding;

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

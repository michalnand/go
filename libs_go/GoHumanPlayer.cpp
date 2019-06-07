#include "GoHumanPlayer.h"

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>


GoHumanPlayer::GoHumanPlayer()
	    :GoPlayer()
{

}

GoHumanPlayer::~GoHumanPlayer()
{

}


GoMove GoHumanPlayer::choose_move()
{
	GoMove move;

	printf("ENTER (letter number) (0,0) for pass: \n");

	do
	{
		int num = 0;
		char let = 0;
		int res = scanf("%c %i\n", &let, &num);
		(void)res;

		printf(">>>> %c %i\n", let, num);

		if (num == 0)
		{
			move.pass_move(current_player);
		}
		else
		{
			int x, y;
			y = num - 1;

			if (let >= 'j')
				let--;
			x = let - 'a';

			if (current_player == GO_PLAYER_BLACK)
				move.black_move(x, y);
			else
				move.white_move(x, y);
		}

		if (is_legal(move) != true)
			printf("illegal move\n");
	}
	while (is_legal(move) != true);


	return move;
}


bool GoHumanPlayer::is_legal(GoMove &move)
{
		for (unsigned int i = 0; i < legal_moves.size(); i++)
			if (move == legal_moves[i])
				return true;

		return false;
}

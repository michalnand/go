#include "GoPlayer.h"

GoPlayer::GoPlayer()
{

}

GoPlayer::~GoPlayer()
{

}

void GoPlayer::play(Goban &goban)
{
	board 				= goban.get_board();
	legal_moves 		= goban.get_legal_moves();
	current_player 		= goban.get_active_player();


	choose_move();

	goban.play_move(move);
}

void GoPlayer::new_game()
{

}

void GoPlayer::set_result(bool winner)
{
	(void)winner;
}

GoMove GoPlayer::get_move()
{
	return move;
}

GoMove GoPlayer::choose_move()
{
	if (legal_moves.size() > 0)
		move = legal_moves[rand()%legal_moves.size()];

	return move;
}

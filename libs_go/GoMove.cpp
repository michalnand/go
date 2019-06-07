#include "GoMove.h"
#include <iostream>


GoMove::GoMove()
{
	m_pass 		= false;
	m_player 	= GO_PLAYER_EMPTY;
	m_x 			= -1;
	m_y 			= -1;
}

GoMove::GoMove(GoMove &other)
{
	copy(other);
}

GoMove::GoMove(const GoMove &other)
{
	copy(other);
}

GoMove::GoMove(bool pass, int player)
{
	m_pass 		= pass;
	m_player 	= player;
	m_x 			= -1;
	m_y 			= -1;
}

GoMove::GoMove(int x, int y, int player)
{
	m_x 			= x;
	m_y 			= y;
	m_player 	= player;
	m_pass 		= false;
}


GoMove& GoMove::operator =(GoMove &other)
{
	copy(other);
	return *this;
}

GoMove& GoMove::operator =(const GoMove &other)
{
	copy(other);
	return *this;
}





GoMove::~GoMove()
{

}

void GoMove::pass_move(int player)
{
 	m_pass 		= true;
	m_player 	= player;
	m_x 			= -1;
	m_y 			= -1;
}

void GoMove::black_move(int x, int y)
{
	m_pass 		= false;
	m_player 	= GO_PLAYER_BLACK;
	m_x 			= x;
	m_y 			= y;
}

void GoMove::white_move(int x, int y)
{
	m_pass 		= false;
	m_player 	= GO_PLAYER_WHITE;
	m_x 			= x;
	m_y 			= y;
}


int GoMove::player()
{
	return m_player;
}

bool GoMove::pass()
{
	return m_pass;
}

int GoMove::x()
{
	return m_x;
}

int GoMove::y()
{
	return m_y;
}

bool GoMove::operator ==(const GoMove &rhs)
{
	if (m_player != rhs.m_player)
		return false;

	if (m_pass != rhs.m_pass)
		return false;

	if (m_x != rhs.m_x)
		return false;

	if (m_y != rhs.m_y)
		return false;

	return true;
}

bool GoMove::operator !=(const GoMove &rhs)
{
	if (m_player != rhs.m_player)
		return true;

	if (m_pass != rhs.m_pass)
		return true;

	if (m_x != rhs.m_x)
		return true;

	if (m_y != rhs.m_y)
		return true;

	return false;
}

unsigned int GoMove::move_to_idx(unsigned int board_size)
{
	if (m_pass)
		return board_size*board_size;

	return m_y*board_size + m_x;
}

void GoMove::print()
{
	std::cout << asString() << "\n";
}

std::string GoMove::asString()
{
	std::string result;

	if (m_player == GO_PLAYER_BLACK)
		result+= "B " + std::to_string(m_x) + " " + std::to_string(m_y) + " " + std::to_string(m_pass);
	else
		result+= "W " + std::to_string(m_x) + " " + std::to_string(m_y) + " " + std::to_string(m_pass);

	return result;
}




void GoMove::copy(GoMove &other)
{
	m_pass 		= other.m_pass;
	m_player 	= other.m_player;
	m_x 			= other.m_x;
	m_y 			= other.m_y;
}

void GoMove::copy(const GoMove &other)
{
	m_pass 		= other.m_pass;
	m_player 	= other.m_player;
	m_x 			= other.m_x;
	m_y 			= other.m_y;
}

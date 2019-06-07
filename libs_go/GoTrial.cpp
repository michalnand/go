#include "GoTrial.h"
#include <iostream>
#include <math.h>

GoTrial::GoTrial()
{
    srand(time(NULL));
    init_result();
}

GoTrial::~GoTrial()
{

}


float GoTrial::play_match(GoPlayer &player_a,
                          GoPlayer &player_b,
                          Goban &goban,
                          bool visualisation)
{
  goban.clear();
  player_a.new_game();
  player_b.new_game();

  int player_a_result = GOBAN_GAME_PLAYING, player_b_result = GOBAN_GAME_PLAYING;

  unsigned int moves = 0;

  while(1)
  {
    player_a.play(goban);
    player_a_result = goban.get_result();

    if (visualisation)
      goban.render();

    if (player_a_result != GOBAN_GAME_PLAYING)
      break;

    player_b.play(goban);
    player_b_result = goban.get_result();

    if (visualisation)
      goban.render();

    if (player_b_result != GOBAN_GAME_PLAYING)
      break;

    moves++;
  }


  float game_score = goban.compute_score();

  if (game_score > 0.0)
  {
    player_a.set_result(1.0);
    player_b.set_result(-1.0);
  }
  else
  {
    player_a.set_result(-1.0);
    player_b.set_result(1.0);
  }

  return game_score;
}

void GoTrial::process(  GoPlayer &player_a,
                        GoPlayer &player_b,
                        unsigned int count,
                        unsigned int board_size,
                        float komi,
                        bool visualisation
                      )
{
  init_result();

  float game_score;

  Goban goban(board_size, komi);


  for (unsigned int match = 0; match < count; match+= 2)
  {
    game_score = play_match(player_a, player_b, goban, visualisation);

    if (game_score > 0.0)
    {
      result.player_a.wins++;
      result.player_a.wins_black++;

      result.player_b.lose++;
      result.player_b.lose_white++;
    }
    else
    {
      result.player_a.lose++;
      result.player_a.lose_black++;

      result.player_b.wins++;
      result.player_b.wins_white++;
    }


    game_score = play_match(player_b, player_a, goban, visualisation);

    if (game_score > 0.0)
    {
      result.player_b.wins++;
      result.player_b.wins_black++;

      result.player_a.lose++;
      result.player_a.lose_white++;
    }
    else
    {
      result.player_b.lose++;
      result.player_b.lose_black++;

      result.player_a.wins++;
      result.player_a.wins_white++;
    }


    /*
    std::cout << match << " ";
    std::cout << result.player_a.wins << " ";
    std::cout << result.player_a.lose << " ";
    std::cout << result.player_b.wins << " ";
    std::cout << result.player_b.lose << " ";

    std::cout << "\n";
    */
  }

  result.player_a.win_ratio = result.player_a.wins*1.0/(result.player_a.wins + result.player_a.lose);
  result.player_b.win_ratio = result.player_b.wins*1.0/(result.player_b.wins + result.player_b.lose);

  if (result.player_a.wins > result.player_b.wins)
    result.winner = result.player_a;
  else
    result.winner = result.player_b;
}

sGoTrialResult GoTrial::get()
{
  return result;
}

std::string GoTrial::asString(bool verbose)
{
  std::string s_result;

  if (verbose)
  {
    s_result+= "player A :\n";
    s_result+= "win ratio : " + std::to_string(result.player_a.win_ratio) + "\n";
    s_result+= "total wins : " + std::to_string(result.player_a.wins) + "\n";
    s_result+= "total loses : " + std::to_string(result.player_a.lose) + "\n";
    s_result+= "black wins : " + std::to_string(result.player_a.wins_black) + "\n";
    s_result+= "white wins : " + std::to_string(result.player_a.wins_white) + "\n";
    s_result+= "\n";
    s_result+= "player B :\n";
    s_result+= "win ratio : " + std::to_string(result.player_b.win_ratio) + "\n";
    s_result+= "total wins : " + std::to_string(result.player_b.wins) + "\n";
    s_result+= "total loses : " + std::to_string(result.player_b.lose) + "\n";
    s_result+= "black wins : " + std::to_string(result.player_b.wins_black) + "\n";
    s_result+= "white wins : " + std::to_string(result.player_b.wins_white) + "\n";
    s_result+= "\n";
  }
  else
  {
    s_result+= std::to_string(result.player_a.wins) + " ";
    s_result+= std::to_string(result.player_a.wins_black) + " ";
    s_result+= std::to_string(result.player_a.wins_white) + " ";
    s_result+= std::to_string(result.player_a.lose) + " ";
    s_result+= std::to_string(result.player_a.lose_black) + " ";
    s_result+= std::to_string(result.player_a.lose_white) + " ";
    s_result+= std::to_string(result.player_a.win_ratio) + " ";

    s_result+= std::to_string(result.player_b.wins) + " ";
    s_result+= std::to_string(result.player_b.wins_black) + " ";
    s_result+= std::to_string(result.player_b.wins_white) + " ";
    s_result+= std::to_string(result.player_b.lose) + " ";
    s_result+= std::to_string(result.player_b.lose_black) + " ";
    s_result+= std::to_string(result.player_b.lose_white) + " ";
    s_result+= std::to_string(result.player_b.win_ratio) + " ";

    s_result+= std::to_string(result.winner.wins) + " ";
    s_result+= std::to_string(result.winner.wins_black) + " ";
    s_result+= std::to_string(result.winner.wins_white) + " ";
    s_result+= std::to_string(result.winner.lose) + " ";
    s_result+= std::to_string(result.winner.lose_black) + " ";
    s_result+= std::to_string(result.winner.lose_white) + " ";
    s_result+= std::to_string(result.winner.win_ratio) + " ";
  }



  return s_result;
}

void GoTrial::print()
{
  std::cout << asString(true) << "\n";
}

void GoTrial::init_result()
{
  result.player_a = init_player_result();
  result.player_b = init_player_result();
  result.winner   = init_player_result();
}

sGoPlayerResult GoTrial::init_player_result()
{
  sGoPlayerResult result;

  result.wins = 0;
  result.wins_black = 0;
  result.wins_white = 0;

  result.lose = 0;
  result.lose_black = 0;
  result.lose_white = 0;

  result.win_ratio  = 0.0;

  return result;
}

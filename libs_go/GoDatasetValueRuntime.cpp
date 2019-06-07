#include "GoDatasetValueRuntime.h"

#include "GoSgfPlayer.h"

#include <iostream>
#include <experimental/filesystem>


GoDatasetValueRuntime::GoDatasetValueRuntime(const char* json_file_name)
          :DatasetInterface()
{
  std::string json_file_name_ = json_file_name;

  JsonConfig json_config(json_file_name_);
  init(json_config.result);
}

GoDatasetValueRuntime::GoDatasetValueRuntime(std::string json_file_name)
          :DatasetInterface()
{
  JsonConfig json_config(json_file_name);
  init(json_config.result);
}

GoDatasetValueRuntime::GoDatasetValueRuntime(Json::Value json_init)
          :DatasetInterface()
{
  init(json_init);
}

GoDatasetValueRuntime::~GoDatasetValueRuntime()
{

}

int GoDatasetValueRuntime::init(Json::Value json_init)
{
  srand(time(NULL));

  this->json_init = json_init;

  board_size  = json_init["board_size"].asInt();
  padding     = json_init["padding"].asInt();
  noise_level = json_init["noise_level"].asFloat();


  width       = board_size + 2*padding;
  height      = board_size + 2*padding;
  channels    = 4;

  return 0;
}


void GoDatasetValueRuntime::create()
{
  training_boards.resize(2);

  for (unsigned int i = 0; i < json_init["testing_sgf"].size(); i++)
  {
    unsigned int max_count = json_init["testing_positions_count"].asInt();
    play_path(json_init["testing_sgf"][i].asString(), max_count, true);
    if (get_testing_size() > max_count)
      break;
  }


  for (unsigned int i = 0; i < json_init["training_sgf"].size(); i++)
  {
    unsigned int max_count = json_init["training_positions_count"].asInt();
    play_path(json_init["training_sgf"][i].asString(), max_count, false);
    if (get_training_size() > max_count)
      break;
  }


  output_size   = training_boards.size();
  training_size = 0;
  for (unsigned int i = 0; i < training_boards.size(); i++)
    training_size+= training_boards[i].size();

  for (unsigned int i = 0; i < 10; i++)
    testing_boards[rand()%testing_boards.size()].print();



  print();
}

void GoDatasetValueRuntime::play_path(std::string path, unsigned int max_count, bool testing)
{
  for (auto & p : std::experimental::filesystem::directory_iterator(path))
  {
    std::string game_record_file_name;
    game_record_file_name = p.path();

    if (std::experimental::filesystem::path(game_record_file_name).extension() == ".sgf")
    {
      play_game(game_record_file_name, testing);

      if (testing)
      {
        std::cout << "game " << game_record_file_name << " " << testing_boards.size() << "\n";
      }
      else
      {
        unsigned int training_size = 0;
        for (unsigned int i = 0; i < training_boards.size(); i++)
          training_size+= training_boards[i].size();

        std::cout << "game " << game_record_file_name << " " << training_size << "\n";
      }

      if (testing)
      {
        if (testing_boards.size() > max_count)
          return;
      }
      else
      {
        unsigned int training_size = 0;
        for (unsigned int i = 0; i < training_boards.size(); i++)
          training_size+= training_boards[i].size();
        if (training_size > max_count)
          return;
      }
    }
  }
}

void GoDatasetValueRuntime::play_game(std::string sgf_file_name, bool testing)
{
  Goban       goban(board_size);
  GoSgfPlayer player(sgf_file_name);

  if (player.get_winner() == GO_PLAYER_EMPTY)
    return;

  std::vector<GoMove> moves;
  std::vector<GoBoard> boards;

  for (unsigned int i = 0; i < player.get_moves_count(); i++)
  {
    auto move = player.get_move(i);
    goban.play_move_no_check(move);

    moves.push_back(move);
    boards.push_back(goban.get_board());
  }

  for (unsigned int i = 0; i < (boards.size()-1); i++)
  {
    GoBoardState board_state(boards[i], moves[i], moves[i+1], player.get_winner());

    if (testing)
    {
      testing_boards.push_back(board_state);
    }
    else
    {
      auto output = board_state.get_winner_value(padding, 0).output;
      unsigned int class_id = argmax(output);
      training_boards[class_id].push_back(board_state);
    }
  }
}

unsigned int GoDatasetValueRuntime::get_training_size()
{
  return training_size;
}

unsigned int GoDatasetValueRuntime::get_testing_size()
{
  return testing_boards.size();
}

unsigned int GoDatasetValueRuntime::get_output_size()
{
  return 2;
}


sDatasetItem GoDatasetValueRuntime::get_random_training()
{
  unsigned int class_idx  = rand()%training_boards.size();
  unsigned int item_id    = rand()%training_boards[class_idx].size();
  unsigned int rotation   = rand()%training_boards[class_idx][item_id].get_rotations_count();

  return training_boards[class_idx][item_id].get_winner_value(padding, rotation, noise_level);
}

sDatasetItem GoDatasetValueRuntime::get_testing(unsigned int idx)
{
  return testing_boards[idx].get_winner_value(padding, 0);
}

sDatasetItem GoDatasetValueRuntime::get_random_testing()
{
  return get_testing(rand()%testing_boards.size());
}

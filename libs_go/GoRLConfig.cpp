#include "GoRLConfig.h"
#include <json_config.h>

GoRLConfig::GoRLConfig()
{

}

GoRLConfig::GoRLConfig(std::string config_file_name)
{
  load(config_file_name);
}

void GoRLConfig::load(std::string config_file_name)
{
  JsonConfig json(config_file_name);

  m_verbose       = json.result["verbose"].asBool();
  m_visualisation = json.result["visualisation"].asBool();

  m_trials_count  = json.result["trials_count"].asInt();
  m_board_size    = json.result["board_size"].asInt();
  m_games_count   = json.result["games_count"].asInt();

  m_komi   = json.result["komi"].asFloat();

  m_result_dir   = json.result["result_dir"].asString();


  m_padding           = json.result["padding"].asInt();
  m_top_moves_count   = json.result["top_moves_count"].asInt();
  m_moves_decay       = json.result["moves_decay"].asInt();

  m_experience_buffer_size    = json.result["dqn"]["experience_buffer_size"].asInt();
  m_gamma                     = json.result["dqn"]["gamma"].asFloat();
  m_network_config_file_name  = json.result["dqn"]["network_config_file_name"].asString();
}

GoRLConfig::~GoRLConfig()
{

}

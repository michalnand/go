#ifndef _GO_RL_CONFIG_H_
#define _GO_RL_CONFIG_H_

#include <string>

class GoRLConfig
{
    public:
        GoRLConfig();
        GoRLConfig(std::string config_file_name);
        virtual ~GoRLConfig();

        void load(std::string config_file_name);

    public:
        bool verbose()
        {
            return m_verbose;
        }

        bool visualisation()
        {
            return m_visualisation;
        }

        unsigned int trials_count()
        {
            return m_trials_count;
        }

        unsigned int games_count()
        {
            return m_games_count;
        }

        unsigned int board_size()
        {
            return m_board_size;
        }

        float komi()
        {
            return m_komi;
        }

        std::string result_dir()
        {
            return m_result_dir;
        }

        unsigned int padding()
        {
            return m_padding;
        }

        unsigned int top_moves_count()
        {
            return m_top_moves_count;
        }

        unsigned int moves_decay()
        {
            return m_moves_decay;
        }

        unsigned int experience_buffer_size()
        {
            return m_experience_buffer_size;
        }

        float gamma()
        {
            return m_gamma;
        }

        std::string network_config_file_name()
        {
            return m_network_config_file_name;
        }


    private:

        bool m_verbose, m_visualisation;
        unsigned int m_trials_count, m_games_count;
        unsigned int m_board_size;
        float m_komi;
        std::string m_result_dir;
        unsigned int m_padding;
        unsigned int m_top_moves_count;
        unsigned int m_moves_decay;
        unsigned int m_experience_buffer_size;
        float m_gamma;
        std::string m_network_config_file_name;
};

#endif

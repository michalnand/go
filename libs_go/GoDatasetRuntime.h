#ifndef _GO_DATASET_RUNTIME_H_
#define _GO_DATASET_RUNTIME_H_

#include "Goban.h"

#include <json_config.h>
#include <dataset_interface.h>

#include "GoBoardState.h"


class GoDatasetRuntime: public DatasetInterface
{
    public:
        GoDatasetRuntime(const char* json_file_name);
        GoDatasetRuntime(std::string json_file_name);
        GoDatasetRuntime(Json::Value json_init);

        virtual ~GoDatasetRuntime();

        void create();

    public:
        unsigned int get_training_size();
        unsigned int get_testing_size();
        unsigned int get_output_size();

        sDatasetItem get_random_training();
        sDatasetItem get_testing(unsigned int idx);
        sDatasetItem get_random_testing();


    private:
        int init(Json::Value json_init);
        void play_path(std::string path, unsigned int max_count, bool testing);
        void play_game(std::string sgf_file_name, bool testing);

    private:
        unsigned int board_size, padding;
        float noise_level;

        Json::Value json_init;

        std::vector<std::vector<GoBoardState>> training_boards;
        std::vector<GoBoardState> testing_boards;
};


#endif

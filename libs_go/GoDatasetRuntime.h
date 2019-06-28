#ifndef _GO_DATASET_RUNTIME_H_
#define _GO_DATASET_RUNTIME_H_

#include <Goban.h>

#include <json_config.h>
#include <dataset_interface.h>

#include <GoBoardState.h>


class GoDatasetRuntime: public DatasetInterface
{
    public:
        GoDatasetRuntime();
        GoDatasetRuntime(GoDatasetRuntime& other);
        GoDatasetRuntime(const GoDatasetRuntime& other);
        GoDatasetRuntime(std::string json_file_name);
        GoDatasetRuntime(Json::Value json_init);

        virtual ~GoDatasetRuntime();

        GoDatasetRuntime& operator= (GoDatasetRuntime& other);
        GoDatasetRuntime& operator= (const GoDatasetRuntime& other);


    private:
        void copy_go_dataset_runtime(GoDatasetRuntime& other);
        void copy_go_dataset_runtime(const GoDatasetRuntime& other);

    public:
        unsigned int get_training_count();
        unsigned int get_testing_count();

        unsigned int get_classes_count();

    public:
        void set_random_training_idx();

        std::vector<float>& get_training_input();
        std::vector<float>& get_training_output();

        std::vector<float>& get_testing_input(unsigned int idx);
        std::vector<float>& get_testing_output(unsigned int idx);

    public:
        void create();



    private:
        int init(Json::Value json_init);
        void play_path(std::string path, unsigned int max_count, bool testing);
        void play_game(std::string sgf_file_name, bool testing);

        void update_training_count();
        void update_testing_count();

        unsigned int argmax(std::vector<float> &v);

    private:
        Json::Value json_init;

        unsigned int board_size;
        unsigned int padding;
        float noise_level;

        unsigned int training_count, testing_count;

        unsigned int training_class_idx;
        unsigned int training_item_idx;
        unsigned int training_rotation_idx;

        std::vector<std::vector<GoBoardState>> training_boards;
        std::vector<GoBoardState> testing_boards;

        int idx_testing_item_now;
        sGoDatasetItem training_item, testing_item;
};


#endif

#include <GoDatasetRuntime.h>
#include <GoSgfPlayer.h>
#include <experimental/filesystem>
#include <iostream>


GoDatasetRuntime::GoDatasetRuntime()
                 :DatasetInterface()
{
    this->board_size        = 0;
    this->padding           = 0;
    this->noise_level       = 0;

    this->training_count = 0;
    this->testing_count  = 0;
    this->training_class_idx    = 0;
    this->training_item_idx     = 0;
    this->training_rotation_idx = 0;

    this->idx_testing_item_now = -1;
}

GoDatasetRuntime::GoDatasetRuntime(GoDatasetRuntime& other)
                 :DatasetInterface(other)
{
    copy_go_dataset_runtime(other);
}

GoDatasetRuntime::GoDatasetRuntime(const GoDatasetRuntime& other)
                 :DatasetInterface(other)
{
    copy_go_dataset_runtime(other);
}

GoDatasetRuntime::GoDatasetRuntime(std::string json_file_name)
                 :DatasetInterface()
{
    JsonConfig json_config(json_file_name);
    init(json_config.result);
}

GoDatasetRuntime::GoDatasetRuntime(Json::Value json_init)
                 :DatasetInterface()
{
    init(json_init);
}



GoDatasetRuntime::~GoDatasetRuntime()
{

}

GoDatasetRuntime& GoDatasetRuntime::operator= (GoDatasetRuntime& other)
{
    copy(other);
    copy_go_dataset_runtime(other);
    return *this;
}

GoDatasetRuntime& GoDatasetRuntime::operator= (const GoDatasetRuntime& other)
{
    copy(other);
    copy_go_dataset_runtime(other);
    return *this;
}


void GoDatasetRuntime::copy_go_dataset_runtime(GoDatasetRuntime& other)
{
    this->json_init         = other.json_init;
    this->board_size        = other.board_size;
    this->padding           = other.padding;
    this->noise_level       = other.noise_level;

    this->training_count = other.training_count;
    this->testing_count  = other.testing_count;
    this->training_class_idx    = other.training_class_idx;
    this->training_item_idx     = other.training_item_idx;
    this->training_rotation_idx = other.training_rotation_idx;

    this->training_boards   = other.training_boards;
    this->testing_boards    = other.testing_boards;

    this->idx_testing_item_now = other.idx_testing_item_now;
    this->testing_item = other.testing_item;
    this->testing_item = other.testing_item;
}

void GoDatasetRuntime::copy_go_dataset_runtime(const GoDatasetRuntime& other)
{
    this->json_init         = other.json_init;
    this->board_size        = other.board_size;
    this->padding           = other.padding;
    this->noise_level       = other.noise_level;

    this->training_count = other.training_count;
    this->testing_count  = other.testing_count;
    this->training_class_idx    = other.training_class_idx;
    this->training_item_idx     = other.training_item_idx;
    this->training_rotation_idx = other.training_rotation_idx;

    this->training_boards   = other.training_boards;
    this->testing_boards    = other.testing_boards;

    this->idx_testing_item_now = other.idx_testing_item_now;
    this->testing_item = other.testing_item;
    this->testing_item = other.testing_item;
}

unsigned int GoDatasetRuntime::get_training_count()
{
    return training_count;
}

unsigned int GoDatasetRuntime::get_testing_count()
{
    return testing_count;
}

unsigned int GoDatasetRuntime::get_classes_count()
{
    return output_shape.size();
}


void GoDatasetRuntime::set_random_training_idx()
{
    training_class_idx      = rand()%training_boards.size();
    training_item_idx       = rand()%training_boards[training_class_idx].size();
    training_rotation_idx   = rand()%training_boards[training_class_idx][training_item_idx].get_rotations_count();

    training_item = training_boards[training_class_idx][training_item_idx].get(padding, training_rotation_idx, noise_level);
}

std::vector<float>& GoDatasetRuntime::get_training_input()
{
    return training_item.input;
}

std::vector<float>& GoDatasetRuntime::get_training_output()
{
    return training_item.output;
}

std::vector<float>& GoDatasetRuntime::get_testing_input(unsigned int idx)
{
    if (idx_testing_item_now != (int)idx)
    {
        idx_testing_item_now = idx;
        testing_item = testing_boards[idx].get(padding, 0);
    }

    return testing_item.input;
}

std::vector<float>& GoDatasetRuntime::get_testing_output(unsigned int idx)
{
    if (idx_testing_item_now != (int)idx)
    {
        idx_testing_item_now = idx;
        testing_item = testing_boards[idx].get(padding, 0);
    }

    return testing_item.output;
}



void GoDatasetRuntime::create()
{
    training_boards.resize(board_size*board_size + 1);

    for (unsigned int i = 0; i < json_init["testing_sgf"].size(); i++)
    {
        unsigned int max_count = json_init["testing_positions_count"].asInt();
        play_path(json_init["testing_sgf"][i].asString(), max_count, true);
        update_testing_count();
        if (get_testing_count() > max_count)
            break;
    }

    for (unsigned int i = 0; i < json_init["training_sgf"].size(); i++)
    {
        unsigned int max_count = json_init["training_positions_count"].asInt();
        play_path(json_init["training_sgf"][i].asString(), max_count, false);
        update_training_count();
        if (get_training_count() > max_count)
            break;
    }


    testing_count = testing_boards.size();

    for (unsigned int i = 0; i < 10; i++)
        testing_boards[rand()%testing_count].print();

    print();
}



int GoDatasetRuntime::init(Json::Value json_init)
{
    this->json_init = json_init;

    this->board_size  = json_init["board_size"].asInt();
    this->padding     = json_init["padding"].asInt();
    this->noise_level = json_init["noise_level"].asFloat();

    this->training_count = 0;
    this->testing_count  = 0;
    this->training_class_idx    = 0;
    this->training_item_idx     = 0;
    this->training_rotation_idx = 0;

    this->idx_testing_item_now = -1;

    //input tensor, board_size x board_size x (black_stones + white_stones + empty_stones + player_on_move_id)
    input_shape.set(board_size + 2*padding, board_size + 2*padding, 4);

    //board_size x board_size moves + PASS move outputs
    output_shape.set(1, 1, board_size*board_size + 1);

    return 0;
}

void GoDatasetRuntime::play_path(std::string path, unsigned int max_count, bool testing)
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
                update_testing_count();
                std::cout << "game " << game_record_file_name << " " << get_testing_count() << "\n";

                if (get_testing_count() > max_count)
                    return;
            }
            else
            {
                update_training_count();
                std::cout << "game " << game_record_file_name << " " << get_training_count() << "\n";

                if (get_training_count() > max_count)
                    return;
            }
        }
    }
}

void GoDatasetRuntime::play_game(std::string sgf_file_name, bool testing)
{
    Goban       goban(board_size);
    GoSgfPlayer player(sgf_file_name);

    if (player.get_moves_count() < 4)
    {
        return;
    }

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
        GoBoardState board_state(boards[i], moves[i], moves[i+1]);
        if (testing)
        {
            testing_boards.push_back(board_state);
        }
        else
        {
            auto output = board_state.get(padding, 0).output;
            unsigned int class_id = argmax(output);
            training_boards[class_id].push_back(board_state);
        }
    }
}

void GoDatasetRuntime::update_training_count()
{
    training_count = 0;
    for (unsigned int i = 0; i < training_boards.size(); i++)
        training_count+= training_boards[i].size();
}

void GoDatasetRuntime::update_testing_count()
{
    testing_count = testing_boards.size();
}


unsigned int GoDatasetRuntime::argmax(std::vector<float> &v)
{
    unsigned int result = 0;
    for (unsigned int i = 0; i < v.size(); i++)
        if (v[i] > v[result])
            result = i;

    return result;
}

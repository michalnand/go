#include "go_supervised.h"

#include <iomanip> // setprecision
#include <sstream> // stringstream

GoSupervised::GoSupervised(DatasetInterface &dataset, std::string experiment_dir, std::string network_config_file)
             :ClassificationExperiment(dataset, experiment_dir, network_config_file)
{

}

GoSupervised::~GoSupervised()
{

}



void GoSupervised::process_best()
{
    {
        sGoSupervisedResult result = compute_matrix(compare_testing);

        save_svg_success(experiment_dir + "moves_success_rate_testing_top1.svg", result);
        save_svg_count(experiment_dir   + "moves_frequency_testing.svg", result);
    }

    {
        sGoSupervisedResult result = compute_matrix(compare_testing_top5);
        save_svg_success(experiment_dir + "moves_success_rate_testing_top5.svg", result);
    }

    {
        sGoSupervisedResult result = compute_matrix(compare_training);

        save_svg_success(experiment_dir + "moves_success_rate_training_top1.svg", result);
        save_svg_count(experiment_dir   + "moves_frequency_training.svg", result);
    }

    {
        sGoSupervisedResult result = compute_matrix(compare_training_top5);
        save_svg_success(experiment_dir + "moves_success_rate_training_top5.svg", result);
    }
}




void GoSupervised::save_svg_success(std::string file_name, sGoSupervisedResult &result)
{
    SVG svg;

    unsigned int square_size  = 50;
    unsigned int board_size = result.board_size;

    svg.add_header(square_size*board_size, square_size*(board_size + 1));

    int r = 0;
    int g = 0;
    int b = 0;

    for (unsigned int y = 0; y < board_size; y++)
    {
        for (unsigned int x = 0; x < board_size; x++)
        {
            float value = result.moves_success_rate[y][x];

            if (value > result.average_success_rate)
            {
                r = 200;
                g = 150;
                b = 150;
            }
            else
            {
                r = 150;
                g = 150;
                b = 200;
            }

            add_field(  svg,
                        x*square_size, y*square_size, square_size,
                        r, g, b, value);
        }
    }

    float value = result.pass_success_rate;

    if (value > result.average_success_rate)
    {
        r = 200;
        g = 150;
        b = 150;
    }
    else
    {
        r = 150;
        g = 150;
        b = 200;
    }

    add_field(  svg,
                (board_size - 1)*square_size, (board_size - 0)*square_size, square_size,
                r, g, b, value);

    svg.add_footer();

    svg.save(file_name);
}



void GoSupervised::save_svg_count(std::string file_name, sGoSupervisedResult &result)
{
    SVG svg;

    unsigned int square_size  = 50;
    unsigned int board_size = result.board_size;

    svg.add_header(square_size*board_size, square_size*(board_size + 1));

    int r = 150;
    int g = 150;
    int b = 150;

    for (unsigned int y = 0; y < board_size; y++)
    {
        for (unsigned int x = 0; x < board_size; x++)
        {
            unsigned int value = result.moves_count[y][x];

            add_field(  svg,
                        x*square_size, y*square_size, square_size,
                        r, g, b, value);
        }
    }

    float value = result.pass_count;

    add_field(  svg,
                (board_size - 1)*square_size, (board_size - 0)*square_size, square_size,
                r, g, b, value);

    svg.add_footer();
    svg.save(file_name);
}


sGoSupervisedResult GoSupervised::compute_matrix(ClassificationCompare &compare)
{
    unsigned int board_size;

    unsigned int classes_count = dataset->get_classes_count();

    switch (classes_count)
    {
        case 5*5+1  : board_size = 5;   break;
        case 9*9+1  : board_size = 9;   break;
        case 13*13+1: board_size = 13;  break;
        case 19*19+1: board_size = 19;  break;
        default     : board_size = 19;
    }

    sGoSupervisedResult result;

    result.board_size = board_size;
    result.moves_success_rate.resize(board_size);
    result.moves_count.resize(board_size);

    for (unsigned int j = 0; j < board_size; j++)
    {
        result.moves_success_rate[j].resize(board_size);
        result.moves_count[j].resize(board_size);
    }

    result.pass_success_rate      = 0.0;
    result.pass_count             = 0;
    result.average_success_rate   = 0.0;

    auto class_success_rate = compare.get_class_success_rate();

    unsigned int ptr = 0;
    for (unsigned int y = 0; y < board_size; y++)
        for (unsigned int x = 0; x < board_size; x++)
        {
            float success = class_success_rate[ptr];

            result.average_success_rate+= success;
            result.moves_success_rate[y][x] = success;
            ptr++;
        }

    result.pass_success_rate = class_success_rate[class_success_rate.size()-1];
    result.average_success_rate+= result.pass_success_rate;

    result.average_success_rate/= class_success_rate.size();

    auto confusion_matrix = compare.get_confusion_matrix();

    ptr = 0;

    for (unsigned int y = 0; y < board_size; y++)
        for (unsigned int x = 0; x < board_size; x++)
        {
            unsigned int count = 0;

            for (unsigned int i = 0; i < confusion_matrix.size(); i++)
                count+= confusion_matrix[i][ptr];

            result.moves_count[y][x] = count;
            ptr++;
        }

    for (unsigned int i = 0; i < confusion_matrix.size(); i++)
        result.pass_count+= confusion_matrix[i][ptr];

    return result;
}

void GoSupervised::add_field(   SVG &svg,
                                unsigned int x, unsigned int y, unsigned int square_size,
                                unsigned int r, unsigned int g, unsigned int b,
                                float value)
{
    int sr = r/2;
    int sg = g/2;
    int sb = b/2;

    svg.add_rectangle(
                        x,
                        y,
                        square_size,
                        square_size,
                        2,
                        r, g, b,
                        sr, sg, sb);

    std::stringstream stream;

    if (value >= 10000)
        stream << std::fixed << std::setprecision(0) << value/10000.0 << "k";
    else
        stream << std::fixed << std::setprecision(0) << value;

    std::string text;
    text = stream.str();

    svg.add_text(   x + square_size/4,
                    y + square_size/2,

                    text,
                    20,
                    false);
}

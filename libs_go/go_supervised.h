#ifndef _GO_SUPERVISED_H_
#define _GO_SUPERVISED_H_

#include <classification_experiment.h>
#include <classification_compare.h>
#include <svg.h>


struct sGoSupervisedResult
{
    unsigned int board_size;

    std::vector<std::vector<float>>         moves_success_rate;
    std::vector<std::vector<unsigned int>>  moves_count;

    float pass_success_rate;
    unsigned int pass_count;

    float average_success_rate;
};


class GoSupervised: public ClassificationExperiment
{
    public:
        GoSupervised(DatasetInterface &dataset, std::string experiment_dir, std::string network_config_file);
        virtual ~GoSupervised();

    protected:
        void process_best();

        void save_svg_success(std::string file_name, sGoSupervisedResult &result);
        void save_svg_count(std::string file_name, sGoSupervisedResult &result);

        sGoSupervisedResult compute_matrix(ClassificationCompare &compare);

        void add_field( SVG &svg,
                        unsigned int x, unsigned int y, unsigned int square_size,
                        unsigned int r, unsigned int g, unsigned int b,
                        float value);
};

#endif

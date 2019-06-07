#include <iostream>

#include <GoDatasetValueRuntime.h>

#include <classification_experiment.h>


int main()
{
	srand(time(NULL));

	std::string dataset_file_name = "networks/9_9_go/dataset_config.json";
	std::string experiment_config_dir = "networks/9_9_go/experiment_dummy/";

 	GoDatasetValueRuntime dataset(dataset_file_name);
	dataset.create();


	ClassificationExperiment experiment(dataset, experiment_config_dir);
	experiment.run();

	std::cout << "program done\n";

	return 0;
}

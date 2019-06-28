#include <iostream>
#include <GoDatasetRuntime.h>
#include <go_supervised.h>


int main()
{
	srand(time(NULL));

	std::string dataset_file_name     = "networks/9_9_go/dataset_config.json";

 	GoDatasetRuntime dataset(dataset_file_name);
	dataset.create();

	{
		std::string experiment_config_dir = "networks/9_9_go/net_0/";
		std::string network_config_file   = "network_config.json";
		GoSupervised experiment(dataset, experiment_config_dir, network_config_file);
		experiment.run();
	}

	{
		std::string experiment_config_dir = "networks/9_9_go/net_1/";
	    std::string network_config_file   = "network_config.json";
	    GoSupervised experiment(dataset, experiment_config_dir, network_config_file);
	    experiment.run();
	}

	std::cout << "program done\n";

 	return 0;
}

#include <iostream>

#include <GoDatasetRuntime.h>
#include <go_supervised.h>
#include <thread>

void experiment_main(DatasetInterface *dataset, std::string experiment_config_dir)
{
    std::cout << "starting experiment from " << experiment_config_dir << "\n";
    GoSupervised experiment(*dataset, experiment_config_dir);
    experiment.run();
}

int main()
{
	srand(time(NULL));

	std::string dataset_file_name = "networks/9_9_go/dataset_config.json";
	std::string experiment_0_config_dir = "networks/9_9_go/net_0/";
	std::string experiment_1_config_dir = "networks/9_9_go/net_1/";

 	GoDatasetRuntime dataset(dataset_file_name);
	dataset.create();

	std::thread t0(experiment_main, &dataset, experiment_0_config_dir);
	std::thread t1(experiment_main, &dataset, experiment_1_config_dir);

	t0.join();
	t1.join();

	std::cout << "program done\n";

 	return 0;
}

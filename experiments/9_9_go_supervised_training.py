import sys
sys.path.insert(0, "/home/michal/programming/go/libs_python")
import pygo


def experiment_main(dataset, experiment_config_dir):
    print("starting experiment from ", experiment_config_dir)
    experiment = pygo.GoSupervised(dataset, experiment_config_dir)
    experiment.run()


dataset_file_name = "networks/9_9_go/dataset_config.json"
experiment_0_config_dir = "networks/9_9_go/net_0/"
experiment_1_config_dir = "networks/9_9_go/net_1/"

dataset = pygo.GoDatasetRuntime(dataset_file_name)
dataset.create()


experiment_main(dataset, experiment_0_config_dir)
experiment_main(dataset, experiment_1_config_dir)


print("program done")

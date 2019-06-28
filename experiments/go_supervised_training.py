import sys
sys.path.insert(0, "/home/michal/programming/go/libs_python")
import pygo


dataset_file_name     = "networks/9_9_go/dataset_config.json"

experiment_config_dir = "networks/9_9_go/net_0/"
network_config_file   = "network_config.json"

dataset = pygo.GoDatasetRuntime(dataset_file_name)
dataset.create()

experiment = pygo.GoSupervised(dataset, experiment_config_dir, network_config_file)
experiment.run()

print("program done")

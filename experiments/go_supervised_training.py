import sys
sys.path.insert(0, "/home/michal/programming/go/libs_python")
import pygo
import threading


dataset_file_name = "networks/19_19_go/dataset_config.json"

experiment_config_dir = "networks/19_19_go/net_2/"


dataset = pygo.GoDatasetRuntime(dataset_file_name)
dataset.create()

experiment = pygo.GoSupervised(dataset, experiment_config_dir)
experiment.run()

print("program done")

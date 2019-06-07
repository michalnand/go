LIBS_RYSY_PATH=$(HOME)/libs/rysy
LIBS_DQN_PATH=$(HOME)/libs/deep_q_network


export LIBS_RYSY_PATH
export LIBS_DQN_PATH

all:
	cd libs_go && make -j4
	cd libs_python && make -j4

	cd experiments && make -j4


clean:
	cd libs_go && make clean
	cd libs_python && make clean

	cd experiments && make clean

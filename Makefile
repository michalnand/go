LIBS_RYSY_PATH=$(HOME)/libs/rysy/rysy2

export LIBS_RYSY_PATH

all:
	cd libs_go && make -j4
	cd libs_python && make -j4

	cd experiments && make


clean:
	cd libs_go && make clean
	cd libs_python && make clean

	cd experiments && make clean

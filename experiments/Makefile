
INC+= -I.
INC+= -I../libs_go
INC+= -I$(LIBS_RYSY_PATH)/libs_common
INC+= -I$(LIBS_RYSY_PATH)/libs_dataset
INC+= -I$(LIBS_RYSY_PATH)/libs_nn
INC+= -I$(LIBS_RYSY_PATH)/libs_experiment

LIBS+= ../libs_go/libs_go.a
LIBS+= $(LIBS_RYSY_PATH)/libs_experiment/libs_experiment.a
LIBS+= $(LIBS_RYSY_PATH)/libs_dataset/libs_dataset.a
LIBS+= $(LIBS_RYSY_PATH)/libs_nn/libs_nn.a
LIBS+= $(LIBS_RYSY_PATH)/libs_common/libs_common.a
 
CC = g++
LD = g++



CFLAGS = $(DEGUBFLAGS) -O2 -Os -s -Wall -Wextra -pedantic -std=c++17 $(INC)
LDFLAGS = $(DEGUBFLAGS) -lstdc++fs -lm -lpthread -pthread -std=c++17 -lglut -lGL -lGLU -lX11 -ljsoncpp -L/usr/local/cuda/lib64 -lcuda -lcudart

SRCS+= $(shell find -name '*.cpp')

OBJS = $(SRCS:.cpp=.o)

ELFS = $(SRCS:.cpp=.elf)

all: $(ELFS)

%.elf: %.o
	$(LD) -o $@ $< $(LIBS) $(LDFLAGS)

%.o: %.cpp
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	rm $(OBJS) -f
	rm $(ELFS) -f

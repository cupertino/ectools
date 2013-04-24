#=============================================================================
#  Author      : Leandro Fontoura Cupertino
#  Date        : 2012.06.01
#  Copyright   : Your copyright notice
#=============================================================================

# compiler
CC = g++

# C++ compiler flags (-g -O2 -Wall -static)
CCFLAGS = -g -O2 -Wall

default: build

all: build_demos

build: build_demos

clean: clean_demos

rebuild: clean build

# ---------------- Compiles the Demos ----------------

# include directories
INCLUDES = -I../include/ -I$(KERNEL_SRC)/tools/perf

# library paths
LIBS = -L../bin/ -lec -lpthread -lncurses

OUT = ../bin/demos

build_demos:
	mkdir -p $(OUT)
	$(CC) $(INCLUDES) $(CCFLAGS) sensor_simple.cpp -o $(OUT)/simple_sensor_demo $(LIBS)
	$(CC) $(INCLUDES) $(CCFLAGS) sensor_process.cpp -o $(OUT)/process_sensor_demo $(LIBS)
	$(CC) $(INCLUDES) $(CCFLAGS) ../src/ectop/model/MonitorEctop.cpp ectop_overhead.cpp -o $(OUT)/ectop_overhead_demo $(LIBS)
	$(CC) $(INCLUDES) $(CCFLAGS) ../src/ectop/model/MonitorEctop.cpp ectop_case.cpp -o $(OUT)/ectop_case_demo $(LIBS)
	$(CC) $(INCLUDES) $(CCFLAGS) dynamic_estimator_daemon.cpp -o $(OUT)/dynamic_estimator_demo_daemon $(LIBS)
	cp dynamic_estimator_view.py $(OUT)/dynamic_estimator_demo_view.py

clean_demos:
	rm -rf $(DEMO_OUT)
	

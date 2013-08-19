#=============================================================================
#  Author      : Leandro Fontoura Cupertino
#  Date        : 2012.06.01
#  Copyright   : Your copyright notice
#=============================================================================

# compiler
CC = g++

# C++ compiler flags (-g -O2 -Wall -static)
CCFLAGS = -g -O2 -Wall

# make the build silent. Set this to something else to make it noisy again.
V ?=		false

QUIET=@
ECHO=@echo
ifeq ($(strip $(V)),false)
	QUIET=@
	ECHO=@echo
else
	QUIET=
	ECHO=@\#
endif

default: build

all: build_demos

build: build_demos

clean: clean_demos

rebuild: clean build

# ---------------- Compiles the Demos ----------------

# include directories
INCLUDES = -I../include/ -I$(KERNEL_SRC)/tools/perf

# library paths
LIBS = -L../bin/ -lec -lpthread -lncurses -lrt

OUT = ../bin/demos

build_demos:
	$(QUIET) mkdir -p $(OUT)
	$(ECHO) "  CC      $(OUT)/simple_sensor_demo"
	$(QUIET) $(CC) $(INCLUDES) $(CCFLAGS) sensor_simple.cpp -o $(OUT)/simple_sensor_demo $(LIBS)
	$(ECHO) "  CC      $(OUT)/process_sensor_demo"
	$(QUIET) $(CC) $(INCLUDES) $(CCFLAGS) sensor_process.cpp -o $(OUT)/process_sensor_demo $(LIBS)
	$(ECHO) "  CC      $(OUT)/ectop_overhead_demo"
	$(QUIET) $(CC) $(INCLUDES) $(CCFLAGS) ../src/ectop/model/MonitorEctop.cpp ectop_overhead.cpp -o $(OUT)/ectop_overhead_demo $(LIBS)
	$(ECHO) "  CC      $(OUT)/ectop_case_demo"
	$(QUIET) $(CC) $(INCLUDES) $(CCFLAGS) ../src/ectop/model/MonitorEctop.cpp ectop_case.cpp -o $(OUT)/ectop_case_demo $(LIBS)
	$(ECHO) "  CC      $(OUT)/ecdaq_case_demo"
	$(QUIET) $(CC) $(INCLUDES) $(CCFLAGS) ../src/ectop/model/MonitorEctop.cpp ecdaq_case.cpp -o $(OUT)/ecdaq_case_demo $(LIBS)
	$(ECHO) "  CC      $(OUT)/dynamic_estimator_demo_daemon"
	$(QUIET) $(CC) $(INCLUDES) $(CCFLAGS) dynamic_estimator_daemon.cpp -o $(OUT)/dynamic_estimator_demo_daemon $(LIBS)
	$(ECHO) "  CP      $(OUT)/dynamic_estimator_demo_view.py"
	$(QUIET) cp dynamic_estimator_view.py $(OUT)/dynamic_estimator_demo_view.py
	$(ECHO) "  CC      $(OUT)/power_estimator_demo_daemon"
	$(QUIET) $(CC) $(INCLUDES) $(CCFLAGS) power_estimator_daemon.cpp -o $(OUT)/power_estimator_demo_daemon $(LIBS)
	$(ECHO) "  CP      $(OUT)/power_estimator_demo_view.py"
	$(QUIET) cp power_estimator_view.py $(OUT)/power_estimator_demo_view.py

clean_demos:
	rm -rf $(DEMO_OUT)
	

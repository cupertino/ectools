#=============================================================================
#  Author      : Leandro Fontoura Cupertino
#  Date        : 2012.06.01
#  Copyright   : Your copyright notice
#=============================================================================

# compiler
CC = g++

# C++ compiler flags (-g -O2 -Wall -static)
CCFLAGS = -g -O2 -Wall

# default path of the kernel source code
//KERNEL_SRC = /usr/src/linux-cea

# install path (/var/run/libec or /var/lib/libec)
INSTALL_PATH = /tmp

default: build

all: build build_testsuite #build_demos

build: build_lib build_ecd build_valgreen build_top build_daq #build_ps #build_ganglia 

clean: clean_lib clean_ecd clean_daq clean_ganglia clean_ps clean_top clean_testsuite clean_demos
	rm -f Makefile.bak

rebuild: clean build

install: install_daq install_top

# ---------------- Compiles the Static Library ----------------

# Source files.
# Compiles all .cpp files in the path src/lib and 2 of its subfirectories
LIB_SRC = $(wildcard src/libec/*.cpp src/libec/*/*.cpp src/libec/*/*/*.cpp src/libec/*/*/*/*.cpp src/libec/*/*/*/*/*.cpp)

LIB_OBJ = $(subst src,obj,$(LIB_SRC:.cpp=.o))

LIB_OUT = bin/libec.a

# include directories
LIB_INCLUDES = -Iinclude/ #-I$(KERNEL_SRC)/tools/perf

# library paths
LIB_LIBS = -L/usr/local/lib -L/usr/lib/ -lm -lsensors

.SUFFIXES: .cpp

obj/%.o:src/%.cpp
	mkdir -p $(dir $@)
	$(CC) $(LIB_INCLUDES) $(CCFLAGS) -c $< -o $@

build_lib: $(LIB_OBJ)
	mkdir -p $(dir $(LIB_OUT))
	ar rcs $(LIB_OUT) $(LIB_OBJ)

clean_lib:
	rm -f $(LIB_OBJ) $(LIB_OUT)

# ---------------- Compiles the Views ----------------

# include directories
VIEW_INCLUDES = $(LIB_INCLUDES)

# library paths
VIEW_LIBS = -L$(dir $(LIB_OUT)) -lec

# output execuable files
DAQ_OUT = bin/ecdaq
GANGLIA_OUT = bin/ecdaq-ganglia
PS_OUT = bin/ecps
TOP_OUT = bin/ectop
ECD_OUT = bin/ecd
VALGREEN_OUT = bin/valgreen

build_daq: build_lib
	mkdir -p $(dir $(DAQ_OUT))
	mkdir -p ~/.config/libec
	$(CC) $(VIEW_INCLUDES) $(CCFLAGS) -lpthread src/ecdaq/main.cpp -o $(DAQ_OUT) $(VIEW_LIBS)

install_daq:
	cp $(DAQ_OUT) $(INSTALL_PATH)/

clean_daq:
	rm -f $(DAQ_OUT)

build_valgreen: build_lib
	mkdir -p $(dir $(DAQ_OUT))
	mkdir -p ~/.config/valgreen
	$(CC) $(VIEW_INCLUDES) $(CCFLAGS) -lpthread src/valgreen/DPELRCpuProcs.cpp src/valgreen/main.cpp -o $(VALGREEN_OUT) $(VIEW_LIBS)

install_valgreen:
	cp $(VALGREEN_OUT) $(INSTALL_PATH)/

clean_valgreen:
	rm -f $(DAQ_OUT)

build_ganglia:
	mkdir -p $(dir $(GANGLIA_OUT))
	$(CC) $(VIEW_INCLUDES) $(CCFLAGS) src/ecdaq-ganglia/main.cpp -o $(GANGLIA_OUT) $(VIEW_LIBS)

install_ganglia:
	cp $(GANGLIA_OUT) $(INSTALL_PATH)/

clean_ganglia:
	rm -f $(GANGLIA_OUT)

build_ps: build_lib
	mkdir -p $(dir $(PS_OUT))
	$(CC) $(VIEW_INCLUDES) $(CCFLAGS) src/ecps/main.cpp -o $(PS_OUT) $(VIEW_LIBS)

clean_ps:
	rm -f $(PS_OUT)

build_top: build_lib
	mkdir -p $(dir $(TOP_OUT))
	mkdir -p ~/.config/ectop
	$(CC) $(VIEW_INCLUDES) $(CCFLAGS) -lncurses src/ectop/model/MonitorEctop.cpp src/ectop/main.cpp -o $(TOP_OUT) $(VIEW_LIBS)
	mkdir -p ~/.config/ectop/html
	#static files for frame table view (need the httpd service running)
	cp -rf src/ectop/view/html/frame_table/* ~/.config/ectop/html

install_top:
	cp $(TOP_OUT) $(INSTALL_PATH)/

clean_top:
	rm -f $(TOP_OUT)
	rm -rf ~/.config/ectop

build_ecd: build_lib
	$(CC) $(VIEW_INCLUDES) $(CCFLAGS) src/ecdaq-daemon/main.cpp -o $(ECD_OUT) $(VIEW_LIBS)

clean_ecd:
	rm -f $(ECD_OUT)
	

# ---------------- Compiles the Tests ----------------

# include directories
TEST_INCLUDES = $(LIB_INCLUDES)

# library paths
TEST_LIBS = -L$(dir $(LIB_OUT)) -lec -lpthread -lncurses

TEST_OUT = bin/test/

build_testsuite: build_lib
	mkdir -p $(TEST_OUT)
	# sensors
	$(CC) $(TEST_INCLUDES) $(CCFLAGS) testsuite/Sensor_test.cpp -o $(TEST_OUT)/sensor_test $(TEST_LIBS)
	$(CC) $(TEST_INCLUDES) $(CCFLAGS) testsuite/SensorCpuFreq_test.cpp -o $(TEST_OUT)/sensorCpuFreq_test $(TEST_LIBS)
	$(CC) $(TEST_INCLUDES) $(CCFLAGS) testsuite/SensorCpuTemp_test.cpp -o $(TEST_OUT)/sensorCpuTemp_test $(TEST_LIBS)
	$(CC) $(TEST_INCLUDES) $(CCFLAGS) testsuite/SensorNetwork_test.cpp -o $(TEST_OUT)/sensorNetwork_test $(TEST_LIBS)
	$(CC) $(TEST_INCLUDES) $(CCFLAGS) testsuite/SensorRunningProcs_test.cpp -o $(TEST_OUT)/sensorRunningProcs_test $(TEST_LIBS)
	$(CC) $(TEST_INCLUDES) $(CCFLAGS) testsuite/SensorPowerAcpi_test.cpp -o $(TEST_OUT)/sensorPowerAcpi_test $(TEST_LIBS)
	$(CC) $(TEST_INCLUDES) $(CCFLAGS) testsuite/SensorPowerPlogg_test.cpp -o $(TEST_OUT)/sensorPowerPlogg_test $(TEST_LIBS)
	$(CC) $(TEST_INCLUDES) $(CCFLAGS) testsuite/SensorPowerRecs_test.cpp -o $(TEST_OUT)/sensorPowerRecs_test $(TEST_LIBS)
	$(CC) $(TEST_INCLUDES) $(CCFLAGS) testsuite/SensorPowerG5k_test.cpp -o $(TEST_OUT)/sensorPowerG5k_test $(TEST_LIBS)
	# pid sensors
	$(CC) $(TEST_INCLUDES) $(CCFLAGS) testsuite/SensorPidCpuTime_test.cpp -o $(TEST_OUT)/sensorPidCpuTime_test $(TEST_LIBS)
	$(CC) $(TEST_INCLUDES) $(CCFLAGS) testsuite/SensorPidCpuElapsedTime_test.cpp -o $(TEST_OUT)/sensorPidCpuElapsedTime_test $(TEST_LIBS)
	$(CC) $(TEST_INCLUDES) $(CCFLAGS) testsuite/SensorPidCpuTimeUsage_test.cpp -o $(TEST_OUT)/sensorPidCpuUsage_test $(TEST_LIBS)
    # power estimators
	$(CC) $(TEST_INCLUDES) $(CCFLAGS) testsuite/PEInverseCpu_test.cpp -o $(TEST_OUT)/peInverseCpu_test $(TEST_LIBS)
	# others
	$(CC) $(TEST_INCLUDES) $(CCFLAGS) testsuite/SensorList_test.cpp -o $(TEST_OUT)/sensorList_test $(TEST_LIBS)
	$(CC) $(TEST_INCLUDES) $(CCFLAGS) testsuite/NCurses_test.cpp -o $(TEST_OUT)/nCurses_test $(TEST_LIBS)
	$(CC) $(TEST_INCLUDES) $(CCFLAGS) testsuite/MicroBenchmark_test.cpp -o $(TEST_OUT)/microBenchmark_test $(TEST_LIBS)
	$(CC) $(TEST_INCLUDES) $(CCFLAGS) testsuite/LinearRegression_test.cpp -o $(TEST_OUT)/linearRegression_test $(TEST_LIBS)
	$(CC) $(TEST_INCLUDES) $(CCFLAGS) testsuite/DPELinearRegression_test.cpp -o $(TEST_OUT)/dpeLinearRegression_test $(TEST_LIBS)
	
	
	$(CC) $(TEST_INCLUDES) $(CCFLAGS) testsuite/CpuInfo_test.cpp -o $(TEST_OUT)/cpuInfo_test $(TEST_LIBS)
	$(CC) $(TEST_INCLUDES) $(CCFLAGS) testsuite/SystemInfo_test.cpp -o $(TEST_OUT)/systemInfo_test $(TEST_LIBS)
	$(CC) $(TEST_INCLUDES) $(CCFLAGS) testsuite/SensorController_test.cpp -o $(TEST_OUT)/sensorController_test $(TEST_LIBS)
#	$(CC) $(TEST_INCLUDES) $(CCFLAGS) testsuite/SensorStructure_test.cpp -o $(TEST_OUT)/sensorStructure_test $(TEST_LIBS)
	$(CC) $(TEST_INCLUDES) $(CCFLAGS) testsuite/SensorPerfCount_test.cpp -o $(TEST_OUT)/sensorPerfCount_test $(TEST_LIBS)
#	$(CC) $(TEST_INCLUDES) $(CCFLAGS) testsuite/SensorPID_test.cpp -o $(TEST_OUT)/sensorPid_test $(TEST_LIBS)
#	$(CC) $(TEST_INCLUDES) $(CCFLAGS) testsuite/SensorPidCpuTimeCounter_test.cpp -o $(TEST_OUT)/sensorPidCpuTimeCounter_test $(TEST_LIBS)
#	$(CC) $(TEST_INCLUDES) $(CCFLAGS) testsuite/SensorPidStat_test.cpp -o $(TEST_OUT)/sensorPidStat_test $(TEST_LIBS)
#	$(CC) $(TEST_INCLUDES) $(CCFLAGS) testsuite/Log_test.cpp -o $(TEST_OUT)/log_test $(TEST_LIBS)
#	$(CC) $(TEST_INCLUDES) $(CCFLAGS) testsuite/Monitor_test.cpp -o $(TEST_OUT)/monitor_test $(TEST_LIBS)
	$(CC) $(TEST_INCLUDES) $(CCFLAGS) testsuite/MonitorLoader_test.cpp -o $(TEST_OUT)/monitor_load_xml_test $(TEST_LIBS)
	$(CC) $(TEST_INCLUDES) $(CCFLAGS) testsuite/ectop_test.cpp -o $(TEST_OUT)/ectop_test $(TEST_LIBS)
#	$(CC) $(TEST_INCLUDES) $(CCFLAGS) testsuite/MonitorWithHTML_test.cpp -o $(TEST_OUT)/monitorWithHTML_test $(TEST_LIBS)
#	$(CC) $(TEST_INCLUDES) $(CCFLAGS) testsuite/Tools_test.cpp -o $(TEST_OUT)/tools_test $(TEST_LIBS)
#	$(CC) $(TEST_INCLUDES) $(CCFLAGS) testsuite/ValidateCpuUsage_test.cpp -o $(TEST_OUT)/validateCpuUsage_test $(TEST_LIBS)
	cp -rf testsuite/monitor_test/html_view $(TEST_OUT)
	cp -rf testsuite/monitorLoader_test/monitorTop.xml /tmp

clean_testsuite:
	rm -rf $(TEST_OUT) /tmp/monitorB.xml

# ---------------- Compiles the Demos ----------------

# include directories
DEMO_INCLUDES = $(LIB_INCLUDES)

# library paths
DEMO_LIBS = -L$(dir $(LIB_OUT)) -lec -lpthread -lncurses

DEMO_OUT = bin/demos/

build_demos: build_lib
	mkdir -p $(DEMO_OUT)
	$(CC) $(DEMO_INCLUDES) $(CCFLAGS) demos/ectop/model/SensorFake.cpp demos/ectop/model/MonitorEctop.cpp demos/ectop/main.cpp -o $(DEMO_OUT)/ectop_demo $(DEMO_LIBS)

clean_demos:
	rm -rf $(DEMO_OUT)
	

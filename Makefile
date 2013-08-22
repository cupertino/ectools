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
#KERNEL_SRC = /usr/src/linux-cea

# install path (/var/run/libec or /var/lib/libec)
INSTALL_PATH = /tmp

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

all: build build_testsuite #build_demos

build: build_lib build_ecd build_valgreen build_top build_daq #build_ps #build_ganglia 

clean: clean_lib clean_ecd clean_daq clean_ganglia clean_ps clean_top clean_testsuite clean_demos
	rm -f Makefile.bak

rebuild: clean build

install: install_daq install_top

# ---------------- Compiles the Required Libraries ----------------
build_req:
	$(ECHO) "  CC      obj/sysfs.o" 
	$(QUIET) gcc -fPIC -o obj/sysfs.o -c include/libcpufreq/sysfs.c
	$(ECHO) "  CC      obj/cpufreq.o" 
	$(QUIET) gcc -fPIC -o obj/cpufreq.o -c include/libcpufreq/cpufreq.c

DEPS_OBJ = obj/sysfs.o obj/cpufreq.o

# ---------------- Compiles the Static Library ----------------

# Source files.
# Compiles all .cpp files in the path src/lib and 2 of its subfirectories
LIB_SRC = $(wildcard src/libec/*.cpp src/libec/*/*.cpp src/libec/*/*/*.cpp src/libec/*/*/*/*.cpp src/libec/*/*/*/*/*.cpp)

LIB_OBJ = $(subst src,obj,$(LIB_SRC:.cpp=.o))

LIB_OUT = bin/libec.a

# include directories
LIB_INCLUDES = -I/usr/include -I./include/ -I./include/libcpufreq #-I$(KERNEL_SRC)/tools/perf

# library paths
LIB_LIBS = -L/usr/local/lib -L/usr/lib -L/usr/lib64 -lm -lrt -lsensors 

.SUFFIXES: .cpp

obj/%.o:src/%.cpp
	$(QUIET) mkdir -p $(dir $@)
	$(ECHO) "  CC     " $@
	$(QUIET) $(CC) $(LIB_INCLUDES) $(CCFLAGS) -c $< -o $@

build_lib: build_req $(LIB_OBJ)
	$(QUIET) mkdir -p $(dir $(LIB_OUT))
	$(ECHO) "  LD     " $(LIB_OUT)
	$(QUIET) ar rcs $(LIB_OUT) $(DEPS_OBJ) $(LIB_OBJ)

clean_lib:
	rm -f $(LIB_OBJ) $(LIB_OUT)

# ---------------- Compiles the Views ----------------

# include directories
VIEW_INCLUDES = $(LIB_INCLUDES) -I/usr/include

# library paths
VIEW_LIBS = -L$(dir $(LIB_OUT)) -L/usr/lib -L/usr/lib64 -lec -lrt  

# output execuable files
DAQ_OUT = bin/ecdaq
GANGLIA_OUT = bin/ecdaq-ganglia
PS_OUT = bin/ecps
TOP_OUT = bin/ectop
ECD_OUT = bin/ecd
VALGREEN_OUT = bin/valgreen

build_daq: build_lib
	$(QUIET) mkdir -p $(dir $(DAQ_OUT))
	$(QUIET) mkdir -p ~/.config/libec
#	$(CC) $(VIEW_INCLUDES) $(CCFLAGS) -lpthread src/ecdaq-daemon-thomas/main.cpp -o $(DAQ_OUT) $(VIEW_LIBS)
	$(ECHO) "  CC     " $(DAQ_OUT)
	$(QUIET) $(CC) $(VIEW_INCLUDES) $(CCFLAGS) -lpthread src/ecdaq/main.cpp -o $(DAQ_OUT) $(VIEW_LIBS)

install_daq:
	cp $(DAQ_OUT) $(INSTALL_PATH)/

clean_daq:
	rm -f $(DAQ_OUT)

build_valgreen: build_lib
	$(QUIET) mkdir -p $(dir $(DAQ_OUT))
	$(QUIET) mkdir -p ~/.config/valgreen
	$(ECHO) "  CC     " $(VALGREEN_OUT)
	$(QUIET) $(CC) $(VIEW_INCLUDES) $(CCFLAGS) -lpthread src/valgreen/DPELRCpuProcs.cpp src/valgreen/main.cpp -o $(VALGREEN_OUT) $(VIEW_LIBS)

install_valgreen:
	cp $(VALGREEN_OUT) $(INSTALL_PATH)/

clean_valgreen:
	rm -f $(DAQ_OUT)

build_ganglia:
	$(QUIET) mkdir -p $(dir $(GANGLIA_OUT))
	$(ECHO) "  CC      " $(GANGLIA_OUT) 
	$(QUIET) $(CC) $(VIEW_INCLUDES) $(CCFLAGS) src/ecdaq-ganglia/main.cpp -o $(GANGLIA_OUT) $(VIEW_LIBS)

install_ganglia:
	cp $(GANGLIA_OUT) $(INSTALL_PATH)/

clean_ganglia:
	rm -f $(GANGLIA_OUT)

build_ps: build_lib
	$(QUIET) mkdir -p $(dir $(PS_OUT))
	$(ECHO) "  CC     " $(PS_OUT)
	$(QUIET) $(CC) $(VIEW_INCLUDES) $(CCFLAGS) src/ecps/main.cpp -o $(PS_OUT) $(VIEW_LIBS)

clean_ps:
	rm -f $(PS_OUT)

build_top: build_lib
	$(QUIET) mkdir -p $(dir $(TOP_OUT))
	$(QUIET) mkdir -p ~/.config/ectop
	$(ECHO) "  CC     " $(TOP_OUT)
	$(QUIET) $(CC) $(VIEW_INCLUDES) $(CCFLAGS) -lncurses src/ectop/model/MonitorEctop.cpp src/ectop/main.cpp -o $(TOP_OUT) $(VIEW_LIBS)
	$(QUIET) mkdir -p ~/.config/ectop/html
#	#static files for frame table view (need the httpd service running)
	$(QUIET) cp -rf src/ectop/view/html/frame_table/* ~/.config/ectop/html

install_top:
	cp $(TOP_OUT) $(INSTALL_PATH)/

clean_top:
	rm -f $(TOP_OUT)
	rm -rf ~/.config/ectop

build_ecd: build_lib
	$(ECHO) "  CC     " $(ECD_OUT)
	$(QUIET) $(CC) $(VIEW_INCLUDES) $(CCFLAGS) src/ecdaq-daemon/main.cpp -o $(ECD_OUT) $(VIEW_LIBS)

clean_ecd:
	rm -f $(ECD_OUT)
	

# ---------------- Compiles the Tests ----------------

# include directories
TEST_INCLUDES = $(LIB_INCLUDES) -I/usr/include

# library paths
TEST_LIBS = -L$(dir $(LIB_OUT)) -L/usr/lib64 -lec -lpthread -lncurses -lrt 

TEST_OUT = bin/test

build_testsuite: build_lib
	$(ECHO) "Building tests"
	$(QUIET) mkdir -p $(TEST_OUT)
# sensors
	$(ECHO) "  CC     " $(TEST_OUT)/sensor_test
	$(QUIET) $(CC) $(TEST_INCLUDES) $(CCFLAGS) testsuite/Sensor_test.cpp -o $(TEST_OUT)/sensor_test $(TEST_LIBS)
	$(ECHO) "  CC     " $(TEST_OUT)/sensorCpuFreq_test
	$(QUIET) $(CC) $(TEST_INCLUDES) $(CCFLAGS) testsuite/SensorCpuFreq_test.cpp -o $(TEST_OUT)/sensorCpuFreq_test $(TEST_LIBS)
	$(ECHO) "  CC     " $(TEST_OUT)/sensorCpuFreqMsr_test
	$(QUIET) $(CC) $(TEST_INCLUDES) $(CCFLAGS) testsuite/SensorCpuFreqMsr_test.cpp -o $(TEST_OUT)/sensorCpuFreqMsr_test $(TEST_LIBS)
	$(ECHO) "  CC     " $(TEST_OUT)/sensorCpuStateMsr_test
	$(QUIET) $(CC) $(TEST_INCLUDES) $(CCFLAGS) testsuite/SensorCpuStateMsr_test.cpp -o $(TEST_OUT)/sensorCpuStateMsr_test $(TEST_LIBS)
	$(ECHO) "  CC     " $(TEST_OUT)/sensorCpuStateTime_test
	$(QUIET) $(CC) $(TEST_INCLUDES) $(CCFLAGS) testsuite/SensorCpuStateTime_test.cpp -o $(TEST_OUT)/sensorCpuStateTime_test $(TEST_LIBS)
	$(ECHO) "  CC     " $(TEST_OUT)/sensorCpuTemp_test
	$(QUIET) $(CC) $(TEST_INCLUDES) $(CCFLAGS) testsuite/SensorCpuTemp_test.cpp -o $(TEST_OUT)/sensorCpuTemp_test $(TEST_LIBS)
	$(ECHO) "  CC     " $(TEST_OUT)/sensorNetwork_test
	$(QUIET) $(CC) $(TEST_INCLUDES) $(CCFLAGS) testsuite/SensorNetwork_test.cpp -o $(TEST_OUT)/sensorNetwork_test $(TEST_LIBS)
	$(ECHO) "  CC     " $(TEST_OUT)/sensorRunningProcs_test
	$(QUIET) $(CC) $(TEST_INCLUDES) $(CCFLAGS) testsuite/SensorRunningProcs_test.cpp -o $(TEST_OUT)/sensorRunningProcs_test $(TEST_LIBS)
	$(ECHO) "  CC     " $(TEST_OUT)/sensorPowerAcpi_test
	$(QUIET) $(CC) $(TEST_INCLUDES) $(CCFLAGS) testsuite/SensorPowerAcpi_test.cpp -o $(TEST_OUT)/sensorPowerAcpi_test $(TEST_LIBS)
	$(ECHO) "  CC     " $(TEST_OUT)/sensorPowerPlogg_test
	$(QUIET) $(CC) $(TEST_INCLUDES) $(CCFLAGS) testsuite/SensorPowerPlogg_test.cpp -o $(TEST_OUT)/sensorPowerPlogg_test $(TEST_LIBS)
	$(ECHO) "  CC     " $(TEST_OUT)/sensorPowerRecs_test
	$(QUIET) $(CC) $(TEST_INCLUDES) $(CCFLAGS) testsuite/SensorPowerRecs_test.cpp -o $(TEST_OUT)/sensorPowerRecs_test $(TEST_LIBS)
	$(ECHO) "  CC     " $(TEST_OUT)/sensorPowerG5k_test
	$(QUIET) $(CC) $(TEST_INCLUDES) $(CCFLAGS) testsuite/SensorPowerG5k_test.cpp -o $(TEST_OUT)/sensorPowerG5k_test $(TEST_LIBS)
# pid sensors
	$(ECHO) "  CC     " $(TEST_OUT)/sensorPidCpuTime_test
	$(QUIET) $(CC) $(TEST_INCLUDES) $(CCFLAGS) testsuite/SensorPidCpuTime_test.cpp -o $(TEST_OUT)/sensorPidCpuTime_test $(TEST_LIBS)
	$(ECHO) "  CC     " $(TEST_OUT)/sensorPidCpuElapsedTime_test
	$(QUIET) $(CC) $(TEST_INCLUDES) $(CCFLAGS) testsuite/SensorPidCpuElapsedTime_test.cpp -o $(TEST_OUT)/sensorPidCpuElapsedTime_test $(TEST_LIBS)
	$(ECHO) "  CC     " $(TEST_OUT)/sensorPidCpuUsage_test
	$(QUIET) $(CC) $(TEST_INCLUDES) $(CCFLAGS) testsuite/SensorPidCpuTimeUsage_test.cpp -o $(TEST_OUT)/sensorPidCpuUsage_test $(TEST_LIBS)
# power estimators
	$(ECHO) "  CC     " $(TEST_OUT)/peInverseCpu_test
	$(QUIET) $(CC) $(TEST_INCLUDES) $(CCFLAGS) testsuite/PEInverseCpu_test.cpp -o $(TEST_OUT)/peInverseCpu_test $(TEST_LIBS)
# others
	$(ECHO) "  CC     " $(TEST_OUT)/sensorList_test
	$(QUIET) $(CC) $(TEST_INCLUDES) $(CCFLAGS) testsuite/SensorList_test.cpp -o $(TEST_OUT)/sensorList_test $(TEST_LIBS)
	$(ECHO) "  CC     " $(TEST_OUT)/nCurses_test
	$(QUIET) $(CC) $(TEST_INCLUDES) $(CCFLAGS) testsuite/NCurses_test.cpp -o $(TEST_OUT)/nCurses_test $(TEST_LIBS)
	$(ECHO) "  CC     " $(TEST_OUT)/microBenchmark_test
	$(QUIET) $(CC) $(TEST_INCLUDES) $(CCFLAGS) testsuite/MicroBenchmark_test.cpp -o $(TEST_OUT)/microBenchmark_test $(TEST_LIBS)
	$(ECHO) "  CC     " $(TEST_OUT)/linearRegression_test
	$(QUIET) $(CC) $(TEST_INCLUDES) $(CCFLAGS) testsuite/LinearRegression_test.cpp -o $(TEST_OUT)/linearRegression_test $(TEST_LIBS)
	$(ECHO) "  CC     " $(TEST_OUT)/dpeLinearRegression_test
	$(QUIET) $(CC) $(TEST_INCLUDES) $(CCFLAGS) testsuite/DPELinearRegression_test.cpp -o $(TEST_OUT)/dpeLinearRegression_test $(TEST_LIBS)	
	$(ECHO) "  CC     " $(TEST_OUT)/cpuInfo_test
	$(QUIET) $(CC) $(TEST_INCLUDES) $(CCFLAGS) testsuite/CpuInfo_test.cpp -o $(TEST_OUT)/cpuInfo_test $(TEST_LIBS)
	$(ECHO) "  CC     " $(TEST_OUT)/systemInfo_test
	$(QUIET) $(CC) $(TEST_INCLUDES) $(CCFLAGS) testsuite/SystemInfo_test.cpp -o $(TEST_OUT)/systemInfo_test $(TEST_LIBS)
	$(ECHO) "  CC     " $(TEST_OUT)/sensorController_test
	$(QUIET) $(CC) $(TEST_INCLUDES) $(CCFLAGS) testsuite/SensorController_test.cpp -o $(TEST_OUT)/sensorController_test $(TEST_LIBS)
#	$(CC) $(TEST_INCLUDES) $(CCFLAGS) testsuite/SensorStructure_test.cpp -o $(TEST_OUT)/sensorStructure_test $(TEST_LIBS)
	$(ECHO) "  CC     " $(TEST_OUT)/sensorPerfCount_test
	$(QUIET) $(CC) $(TEST_INCLUDES) $(CCFLAGS) testsuite/SensorPerfCount_test.cpp -o $(TEST_OUT)/sensorPerfCount_test $(TEST_LIBS)
#	$(CC) $(TEST_INCLUDES) $(CCFLAGS) testsuite/SensorPID_test.cpp -o $(TEST_OUT)/sensorPid_test $(TEST_LIBS)
#	$(CC) $(TEST_INCLUDES) $(CCFLAGS) testsuite/SensorPidCpuTimeCounter_test.cpp -o $(TEST_OUT)/sensorPidCpuTimeCounter_test $(TEST_LIBS)
#	$(CC) $(TEST_INCLUDES) $(CCFLAGS) testsuite/SensorPidStat_test.cpp -o $(TEST_OUT)/sensorPidStat_test $(TEST_LIBS)
#	$(CC) $(TEST_INCLUDES) $(CCFLAGS) testsuite/Log_test.cpp -o $(TEST_OUT)/log_test $(TEST_LIBS)
#	$(CC) $(TEST_INCLUDES) $(CCFLAGS) testsuite/Monitor_test.cpp -o $(TEST_OUT)/monitor_test $(TEST_LIBS)
	$(ECHO) "  CC     " $(TEST_OUT)/monitor_load_xml_test
	$(QUIET) $(CC) $(TEST_INCLUDES) $(CCFLAGS) testsuite/MonitorLoader_test.cpp -o $(TEST_OUT)/monitor_load_xml_test $(TEST_LIBS)
	$(ECHO) "  CC     " $(TEST_OUT)/ectop_test
	$(QUIET) $(CC) $(TEST_INCLUDES) $(CCFLAGS) testsuite/ectop_test.cpp -o $(TEST_OUT)/ectop_test $(TEST_LIBS)
#	$(CC) $(TEST_INCLUDES) $(CCFLAGS) testsuite/MonitorWithHTML_test.cpp -o $(TEST_OUT)/monitorWithHTML_test $(TEST_LIBS)
#	$(CC) $(TEST_INCLUDES) $(CCFLAGS) testsuite/Tools_test.cpp -o $(TEST_OUT)/tools_test $(TEST_LIBS)
#	$(CC) $(TEST_INCLUDES) $(CCFLAGS) testsuite/ValidateCpuUsage_test.cpp -o $(TEST_OUT)/validateCpuUsage_test $(TEST_LIBS)
	$(QUIET) cp -rf testsuite/monitor_test/html_view $(TEST_OUT)
	$(QUIET) cp -rf testsuite/monitorLoader_test/monitorTop.xml /tmp

clean_testsuite:
	rm -rf $(TEST_OUT) /tmp/monitorB.xml

# ---------------- Compiles the Demos ----------------

# include directories
DEMO_INCLUDES = $(LIB_INCLUDES)

# library paths
DEMO_LIBS = -L$(dir $(LIB_OUT)) -lec -lpthread -lncurses

DEMO_OUT = bin/demos

build_demos: build_lib
	$(QUIET) mkdir -p $(DEMO_OUT)
	$(ECHO) "  CC     " $(DEMO_OUT)/ectop_demo
	$(QUIET) $(CC) $(DEMO_INCLUDES) $(CCFLAGS) demos/ectop/model/SensorFake.cpp demos/ectop/model/MonitorEctop.cpp demos/ectop/main.cpp -o $(DEMO_OUT)/ectop_demo $(DEMO_LIBS)

clean_demos:
	rm -rf $(DEMO_OUT)
	

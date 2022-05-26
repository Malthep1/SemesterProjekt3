
SOURCES=$(wildcard *.cpp)
OBJECTS=$(addprefix $(BUILD_DIR)/,$(SOURCES:.cpp=.o)) 
DEPS=$(addprefix $(BUILD_DIR)/,$(SOURCES:.cpp=.d))    
EXE=prog
CXXFLAGS=-DOS_LINUX -I$(BASEPATH)inc
ARCH?=arm
BASEPATH=../inc/OSApiStudent/
LIBPATH=$(BASEPATH)lib/target/release/
LNKFLAGS=-L$(LIBPATH) -lOSApi -lrt -lpthread
# Making for x86 -64 e.g. x86 -64 (the architecture employed)
# > make ARCH=x86 -64
ifeq (${ARCH},x86-64)
CXX=g++
BUILD_DIR=build/x86-64
endif
# Making for architecture
# > make ARCH=arm
ifeq (${ARCH},arm)
CXX=arm-rpizw-g++
BUILD_DIR=build/arm
endif
$(BUILD_DIR)/$(EXE): $(DEPS) $(OBJECTS) # << Check the $(DEPS) new dependency
	$(CXX) $(CXXFLAGS) -o $@ $(OBJECTS) $(LNKFLAGS)
	echo $(BUILD_DIR)/$(EXE)
# %.cpp -> %.o needs to be added! Target is NOT just %.o...
$(BUILD_DIR)/%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@ $(LNKFLAGS)
# Rule that describes how a .d (dependency) file is created from a .cpp file
# Similar to the assigment that you just completed %.cpp -> %.o
$(BUILD_DIR)/%.d: %.cpp
	$(CXX) -MT$@ -MM $(CXXFLAGS) $< > $@
	$(CXX) -MT$(@:.d=.o) -MM $(CXXFLAGS) $< >> $@
ifneq ($(MAKECMDGOALS),clean)
-include $(DEPS)
endif

clean:
	rm -d build/arm/ -rf *.o *.d prog
	rm -d build/x86-64/ -rf *.o *.d prog
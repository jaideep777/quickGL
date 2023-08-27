# makefile for libgsm

TARGET = libquickgl
VERSION = 
LIBPATH = #-L/usr/local/netcdf-cxx-legacy/lib -L/usr/local/cuda/lib64	# Netcdf-c++ libaray path
INCPATH = -I./include #-I/usr/local/netcdf-cxx-legacy/include -I/usr/local/netcdf-c/include -I/usr/local/cuda/include  # need paths to netcdf-c as well as c++ includes
LDFLAGS =  
CPPFLAGS = -O3 -std=c++11 -fPIC -Wall -Wno-unused-variable
CUDAFLAGS = #-std=c++11 -Xcompiler -fPIC -arch=sm_35 -Wno-deprecated-gpu-targets

LIBS = 
#LIBS += -lgsl -lgslcblas 
LIBS += -lglfw -ldl #-lGL -lglut -lGLU -lGLEW

CUDA_LIBS = #-lcudart -lcurand -lcufft

SOURCEDIR = src
CUDA_SOURCEDIR = src_cuda
BUILDDIR = build
OUTLIBPATH = lib
INSTALLDIR = 

SOURCES = $(wildcard $(SOURCEDIR)/*.cpp)
OBJECTS = $(patsubst $(SOURCEDIR)/%.cpp, $(BUILDDIR)/%.o, $(SOURCES))
CUDA_SOURCES = $(wildcard $(CUDA_SOURCEDIR)/*.cu)
CUDA_OBJECTS = $(patsubst $(CUDA_SOURCEDIR)/%.cu, $(BUILDDIR)/%.cu_o, $(CUDA_SOURCES))

all: dir $(TARGET)

dir:
	mkdir -p $(BUILDDIR) lib

gl3w:
	python3 gl3w_gen.py
	mv src/gl3w.c src/gl3w.cpp

$(TARGET): $(OBJECTS) $(CUDA_OBJECTS)
	g++ -shared $(LIBPATH) $(LDFLAGS) -o $(OUTLIBPATH)/$(TARGET).so $(OBJECTS) $(CUDA_OBJECTS) $(LIBS) $(CUDA_LIBS)

$(OBJECTS): $(BUILDDIR)/%.o : $(SOURCEDIR)/%.cpp
	g++ -c $(CPPFLAGS) $(INCPATH) $< -o $@ 

$(CUDA_OBJECTS): $(BUILDDIR)/%.cu_o : $(CUDA_SOURCEDIR)/%.cu
	nvcc -c $(CUDAFLAGS) $(INCPATH) $< -o $@ 

install:
	cp $(OUTLIBPATH)/$(TARGET).so.$(VERSION) $(INSTALLDIR)
	
clean:
	rm -f $(BUILDDIR)/*.o $(BUILDDIR)/*.cu_o $(OUTLIBPATH)/$(TARGET)*


## TESTING SUITE ##

TEST_FILES = $(wildcard tests/*.cpp) 
TEST_OBJECTS = $(patsubst tests/%.cpp, tests/%.o, $(TEST_FILES))
TEST_TARGETS = $(patsubst tests/%.cpp, tests/%.test, $(TEST_FILES))
TEST_RUNS = $(patsubst tests/%.cpp, tests/%.run, $(TEST_FILES))
ADD_OBJECTS = 

check: dir $(TARGET) compile_tests clean_log run_tests # plant_demo_test

compile_tests: $(TEST_TARGETS)
	
clean_log:
	@rm -f log.txt

run_tests: $(TEST_RUNS) 
	
$(TEST_RUNS): tests/%.run : tests/%.test	
	@echo "~~~~~~~~~~~~~~~ $< ~~~~~~~~~~~~~~~~" >> log.txt
	@./$< >> log.txt && \
		printf "%b" "\033[0;32m[PASS]\033[m" ": $* \n"  || \
		printf "%b" "\033[1;31m[FAIL]\033[m" ": $* \n"

$(TEST_OBJECTS): tests/%.o : tests/%.cpp $(HEADERS) 
	g++ -c $(CPPFLAGS) $(INC_PATH) $< -o $@

$(TEST_TARGETS): tests/%.test : tests/%.o
	g++ $(LDFLAGS) -o $@ $(LIB_PATH) $(OBJECTS) $(ADD_OBJECTS) $< $(LIBS) 

testclean: 
	rm -f tests/*.o tests/*.test lib/*.a lib/*.so

recheck: testclean check

.PHONY: $(TEST_RUNS) run_tests clean testclean

## MAKEFILE FOR CUDA

##-------------------------------------------------------------------------------
## executable name
#TARGET := 1

## files
#CCFILES  :=  $(wildcard src/*.cpp) 

## ------------------------------------------------------------------------------

## paths
##CUDA_INSTALL_PATH ?= /usr/local/cuda#-5.0

## compilers

## include and lib dirs (esp for cuda)
#INC_PATH := 
#LIB_PATH := 
#GLLIB_PATH := 

## flags
#COMMONFLAGS = -m64 
#CPPFLAGS = -O3 -std=c++11 -g -Wall -Wno-unused-variable
#LINKFLAGS += $(COMMONFLAGS) 

## libs
##LIBS = -lcudart 					# cuda libs 		-lcutil_x86_64 -lshrutil_x86_64
#GLLIBS = -lGL -lglut -lGLU -lGLEW 				# openGL libs       -lGL -lGLEW  #-lX11 -lXi -lXmu 		
#LIBS = 	  	# additional libs

## files
#OBJECTS = $(patsubst src/%.cpp, build/%.o, $(CCFILES))

## common dependencies	
#COM_DEP = 

#all: dir $(TARGET)	

#dir: 
#	mkdir -p lib build

#$(TARGET): $(OBJECTS) 
#	g++ -o $(TARGET) $(LIB_PATH) $(GLLIB_PATH) $(OBJECTS) $(LIBS) $(GLLIBS)


#$(OBJECTS): build/%.o : src/%.cpp
#	g++ -c $(CPPFLAGS) $(INC_PATH) $< -o $@ 

#clean:
#	rm -f $(TARGET) build/*.o 
#	
#re: clean all

## ------------------------------------------------------------------------------





#-gencode=arch=compute_10,code=\"sm_10,compute_10\"  -gencode=arch=compute_20,code=\"sm_20,compute_20\"  -gencode=arch=compute_30,code=\"sm_30,compute_30\" 

#-W -Wall -Wimplicit -Wswitch -Wformat -Wchar-subscripts -Wparentheses -Wmultichar -Wtrigraphs -Wpointer-arith -Wcast-align -Wreturn-type -Wno-unused-function 
#-m64 -fno-strict-aliasing 
#-I. -I/usr/local/cuda/include -I../../common/inc -I../../../shared//inc 
#-DUNIX -O2


#g++ -fPIC   -m64 -o ../../bin/linux/release/swarming_chasing_predator obj/x86_64/release/genmtrand.cpp.o  obj/x86_64/release/simpleGL.cu.o  -L/usr/local/cuda/lib64 -L../../lib -L../../common/lib/linux -L../../../shared//lib -lcudart   
#-lGL -lGLU -lX11 -lXi -lXmu -lGLEW_x86_64 -L/usr/X11R6/lib64 -lGLEW_x86_64 -L/usr/X11R6/lib64 -lglut 
#-L/usr/local/cuda/lib64 -L../../lib -L../../common/lib/linux -L../../../shared//lib -lcudart 
#-L/usr/lib -lgsl -lgslcblas 
#-lcutil_x86_64  -lshrutil_x86_64 




#CXXWARN_FLAGS := \
#	-W -Wall \
#	-Wimplicit \
#	-Wswitch \
#	-Wformat \
#	-Wchar-subscripts \
#	-Wparentheses \
#	-Wmultichar \
#	-Wtrigraphs \
#	-Wpointer-arith \
#	-Wcast-align \
#	-Wreturn-type \
#	-Wno-unused-function \
#	$(SPACE)

#CWARN_FLAGS := $(CXXWARN_FLAGS) \
#	-Wstrict-prototypes \
#	-Wmissing-prototypes \
#	-Wmissing-declarations \
#	-Wnested-externs \
#	-Wmain \
#	
	
#HEADERS  := $(wildcard *.h)
	

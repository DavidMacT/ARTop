TARGET_EXEC2 = source/cpp/getVelocity
TARGET_EXEC1 = source/cpp/observationalWindingPotentialFast

SRCS1 = source/cpp/pointCheap.cpp source/cpp/interpolation2Large.cpp  source/cpp/biotSavartGauge.cpp source/cpp/mainObservationalWindingPotentialFast.cpp
SRCS2 = source/cpp/pointCheap.cpp  source/cpp/dave4VM.cpp  source/cpp/mainDave.cpp

OBJS1 = $(SRCS1:.cpp=.o)
OBJS2 = $(SRCS2:.cpp=.o)

CXX = g++
CXXFLAGS = -O3 -std=gnu++14 -fopenmp
RM = rm -f
LDFLAGS1 =  -L/usr/local/lib -llapack -lblas -lgfortran


all: $(TARGET_EXEC1) $(TARGET_EXEC2)

$(TARGET_EXEC1): $(OBJS1)
	$(CXX) $(CXXFLAGS) -o $(TARGET_EXEC1) $(OBJS1) -fopenmp

$(TARGET_EXEC2): $(OBJS2)
	$(CXX) $(CXXFLAGS) -o $(TARGET_EXEC2) $(OBJS2) $(LDFLAGS1)

source/cpp/.cpp.o:
	$(CXX) $(CXXFLAGS) $< -o $@ 

clean:
	$(RM) $(OBJS2) $(OBJS1) $(TARGET_EXEC1) $(TARGET_EXEC2)


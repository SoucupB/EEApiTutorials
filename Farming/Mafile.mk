TARGET_DLL = Bots.dll
TARGET_ENGINE = Engine.a
CXX = g++
CXXFLAGS = -std=gnu++11 -IHeaders/ -IEngineHeaders/ -O2 -lPsapi -lgdi32 -luser32
DLL_BUILDER = -Wl,--whole-archive lib/$(TARGET_ENGINE) -Wl,--no-whole-archive -shared

SRC_DIR = Sources
OBJ_DIR = lib

SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SOURCES))

all: $(TARGET_DLL)

$(TARGET_DLL): $(OBJECTS)
	$(CXX) $^ $(DLL_BUILDER) -o $(OBJ_DIR)/$(TARGET_DLL) $(CXXFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@
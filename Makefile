# Compilateur C++
CXX = g++

# Drapeaux de compilation
CXXFLAGS = -Wall -Wextra -std=c++17 -floop-interchange -floop-strip-mine -floop-block -floop-parallelize-all -ftree-parallelize-loops=4 -Ofast -march=native -funroll-loops -flto -fprefetch-loop-arrays 
# Nom de l'exécutable
TARGET = Heuristic_PFSP_SLS

# Dossier des fichiers sources
SRC_DIR = src

# Dossier des fichiers objets
BUILD_DIR = build

# Fichiers sources C++
SRCS = $(wildcard $(SRC_DIR)/*.cpp)

# Fichiers objets correspondants
OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SRCS))

# Règle par défaut
all: $(TARGET)

# règle pour générer l'executable avec profiler
profile: CXXFLAGS += -pg
profile: $(TARGET)

# Règle pour générer l'exécutable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Règle pour générer les fichiers objets
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp $(SRC_DIR)/constant.hpp
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Règle pour nettoyer les fichiers générés
clean:
	rm -rf $(BUILD_DIR) $(TARGET)

# Règle pour supprimer tous les fichiers générés
.PHONY: all clean

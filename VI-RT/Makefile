# CXX      := g++ 
# CXXFLAGS := -std=c++11 -O3
# LDFLAGS  := 
# BUILD    := ./build
# OBJ_DIR  := $(BUILD)/objects
# APP_DIR  := $(BUILD)/apps
# SHELL	 := /bin/bash

# TARGET   := VI-RT

# INCLUDE  := -IVI-RT/Camera/ -IVI-RT/Image -IVI-RT/Light -IVI-RT/Primitive -IVI-RT/Primitive/BRDF -IVI-RT/Primitive/Geometry -IVI-RT/Rays -IVI-RT/Renderer -IVI-RT/Scene -IVI-RT/Shader -IVI-RT/utils -IVI-RT/Scene/tinyobjloader/

# SRC      :=                      \
#    $(wildcard VI-RT/*.cpp) \
#    $(wildcard VI-RT/Camera/*.cpp)         \
#    $(wildcard VI-RT/Image/*.cpp)         \
#    $(wildcard VI-RT/Primitive/BRDF/*.cpp)         \
#    $(wildcard VI-RT/Primitive/Geometry/*.cpp)         \
#    $(wildcard VI-RT/Renderer/*.cpp)         \
#    $(wildcard VI-RT/Scene/*.cpp)         \
#    $(wildcard VI-RT/Shader/*.cpp)         \

# OBJECTS  := $(SRC:%.cpp=$(OBJ_DIR)/%.o)
# DEPENDENCIES \
#          := $(OBJECTS:.o=.d)

# all:	build $(APP_DIR)/$(TARGET)

# $(OBJ_DIR)/%.o: %.cpp
# 	@mkdir -p $(@D) 2>/dev/null || true
# 	$(CXX) $(CXXFLAGS) $(INCLUDE) -c $< -MMD -o $@

# $(APP_DIR)/$(TARGET): $(OBJECTS)
# 	@mkdir -p $(@D) 2>/dev/null || true
# 	$(CXX) $(CXXFLAGS) -o $(APP_DIR)/$(TARGET) $^ $(LDFLAGS)

# -include $(DEPENDENCIES)

# .PHONY: all build clean 

# build:
# 	@mkdir -p $(APP_DIR) 2>/dev/null || true
# 	@mkdir -p $(OBJ_DIR) 2>/dev/null || true

# clean:
# 	-@rm -rvf $(OBJ_DIR)/*
# 	-@rm -rvf $(APP_DIR)/*

# ##########################################N NÃO APAGAR, É PARA O FERNAS CORRER ###########################################
CXX      := g++ 
CXXFLAGS := -std=c++11 -O3
LDFLAGS  := 
BUILD    := ./build
OBJ_DIR  := $(BUILD)/objects
APP_DIR  := $(BUILD)/apps

TARGET   := VI-RT

INCLUDE  := -I"VI-RT/Camera/" -I"VI-RT/Image" -I"VI-RT/Light" -I"VI-RT/Primitive" -I"VI-RT/Primitive/BRDF" -I"VI-RT/Primitive/Geometry" -I"VI-RT/Rays" -I"VI-RT/Renderer" -I"VI-RT/Scene" -I"VI-RT/Shader" -I"VI-RT/utils" -I"VI-RT/Scene/tinyobjloader/"

SRC      :=                      \
   $(wildcard VI-RT/*.cpp) \
   $(wildcard VI-RT/Camera/*.cpp)         \
   $(wildcard VI-RT/Image/*.cpp)         \
   $(wildcard VI-RT/Primitive/BRDF/*.cpp)         \
   $(wildcard VI-RT/Primitive/Geometry/*.cpp)         \
   $(wildcard VI-RT/Renderer/*.cpp)         \
   $(wildcard VI-RT/Scene/*.cpp)         \
   $(wildcard VI-RT/Shader/*.cpp)         

OBJECTS  := $(SRC:%.cpp=$(OBJ_DIR)/%.o)
DEPENDENCIES := $(OBJECTS:.o=.d)

all:	build $(APP_DIR)/$(TARGET)

$(OBJ_DIR)/%.o: %.cpp
	@if not exist "$(OBJ_DIR)" mkdir "$(OBJ_DIR)"
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c $< -MMD -o $@

$(APP_DIR)/$(TARGET): $(OBJECTS)
	@if not exist "$(APP_DIR)" mkdir "$(APP_DIR)"
	$(CXX) $(CXXFLAGS) -o $(APP_DIR)/$(TARGET) $^ $(LDFLAGS)

-include $(DEPENDENCIES)

.PHONY: all build clean 

build:
	@if not exist "$(APP_DIR)" mkdir "$(APP_DIR)"
	@if not exist "$(OBJ_DIR)" mkdir "$(OBJ_DIR)"

clean:
	-@del /S /Q "$(OBJ_DIR)\*"
	-@del /S /Q "$(APP_DIR)\*"

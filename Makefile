####################################################################################################
# 当前路径
CURRENT_DIR := $(dir $(abspath $(firstword $(MAKEFILE_LIST))))

# 指定编译器及编译选项
CXX = g++
LANG_STD = -std=c++20
COMPILER_FLAGS = -Wall -Wfatal-errors

# 定义构建目录
BUILD_DIR = ./build

# 定义资源目录
ASSETS_DIR = ./assets

# 定义一个递归查找函数 rwildcard
rwildcard = $(wildcard $1$2) $(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2))

# 源代码文件：查找 ./src 及其所有子目录下的 .cpp 文件
SRC_FILES := $(call rwildcard,./src/,*.cpp)

# 为每个源文件生成对应的对象文件路径，所有对象文件直接放在 ./build 目录下
OBJ_FILES := $(patsubst ./src/%.cpp,$(BUILD_DIR)/%.o,$(SRC_FILES))

# 包含目录（用于查找头文件）
INCLUDE_PATH = -I"./include/"

# 静态库目录
LIBS_PATH = -L"./lib/Lua/" -L"./lib/SDL2/x64/" -L"./lib/SDL2_image/x64/" -L"./lib/SDL2_ttf/x64/" -L"./lib/SDL2_mixer/x64/"

# 静态库链接
LIBS =  -llua54 -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer

# 动态库目录
DLLS_PATH = ./dll

# 动态库链接（如果需要在运行时复制到可执行文件目录）
DLLS = $(call rwildcard,$(DLLS_PATH),*.dll)

# 根据操作系统设置最终目标名称及删除命令
ifeq ($(OS),Windows_NT)
TARGET = $(BUILD_DIR)/GameEngine.exe
RM = rmdir /S /Q "$(BUILD_DIR)"  # 使用 rmdir 删除文件夹
else
TARGET = $(BUILD_DIR)/GameEngine
RM = rm -rf "$(BUILD_DIR)"
endif

####################################################################################################
# 默认目标：先创建构建目录，再构建可执行文件
all: $(BUILD_DIR) $(TARGET)

# 确保构建目录存在（平台区分命令）
$(BUILD_DIR):
ifeq ($(OS),Windows_NT)
	@if not exist "$(BUILD_DIR)" mkdir "$(BUILD_DIR)"
else
	@mkdir -p $(BUILD_DIR)
endif

# 链接目标文件生成可执行文件，并链接所需的库
$(TARGET): $(OBJ_FILES)
	@echo "------------------------------------------"
	@echo "== 链接目标文件生成可执行文件 =="
	@echo "BUILD_DIR = $(BUILD_DIR)"
	@echo "OBJ_FILES = $(OBJ_FILES)"
	@echo "TARGET = $(TARGET)"
	@echo "LIBS = $(LIBS)"
	@echo "------------------------------------------"
	$(CXX) $(OBJ_FILES) -o $(TARGET) $(LIBS_PATH) $(LIBS)

# 生成对象文件，所有对象文件放在 ./build 目录下
$(BUILD_DIR)/%.o: ./src/%.cpp
	@echo "------------------------------------------"
	@echo "== 编译源文件 =="
	@echo "源文件: $<"
	@echo "目标对象文件: $@"
	@echo "目标目录: $(dir $@)"
	@echo "------------------------------------------"
ifeq ($(OS),Windows_NT)
	@if not exist "$(subst /,\,$(patsubst %/,%,$(dir $@)))" mkdir "$(subst /,\,$(patsubst %/,%,$(dir $@)))"
else
	@mkdir -p $(dir $@)
endif
	$(CXX) $(COMPILER_FLAGS) $(LANG_STD) $(INCLUDE_PATH) -c $< -o $@

####################################################################################################
# 复制动态库和资源到构建目录（如果有动态库需要在运行时复制）
copy_dll:
	@echo "------------------------------------------"
	@echo "== 复制动态链接库和资源到目标目录 ==";
	@echo "复制的库文件: $(DLLS)";
	@echo "目标目录: $(BUILD_DIR)";
	@echo "------------------------------------------"
ifeq ($(OS),Windows_NT)
	xcopy "$(DLLS_PATH)/SDL2/*" "$(BUILD_DIR)/" /D/I/H
	xcopy "$(DLLS_PATH)/Lua/*" "$(BUILD_DIR)/" /D/I/H
	@if not exist "$(BUILD_DIR)/$(ASSETS_DIR)" mkdir "$(BUILD_DIR)/$(ASSETS_DIR)"
	xcopy "$(ASSETS_DIR)/*" "$(BUILD_DIR)/$(ASSETS_DIR)/" /D/I/H
else
	cp $(DLLS_PATH)/SDL2/* $(BUILD_DIR)/
	cp $(DLLS_PATH)/Lua/* $(BUILD_DIR)/
	cp -r $(ASSETS_DIR)/ $(BUILD_DIR)/
endif

####################################################################################################
# 运行目标程序：先复制动态库，再运行程序
run: $(TARGET) copy_dll
	@echo "------------------------------------------"
	@echo "== 运行可执行文件 =="
	@echo "执行文件: $(TARGET)"
	@echo "------------------------------------------"
	./$(TARGET)

####################################################################################################
# 清理构建目录
clean:
	@echo "------------------------------------------"
	@echo "== 清理构建目录 =="
	@echo "删除目录: $(BUILD_DIR)"
	@echo "------------------------------------------"
	$(RM)

.PHONY: all run clean copy_dll

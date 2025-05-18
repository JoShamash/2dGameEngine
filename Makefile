####################################################################################################
# ��ǰ·��
CURRENT_DIR := $(dir $(abspath $(firstword $(MAKEFILE_LIST))))

# ָ��������������ѡ��
CXX = g++
LANG_STD = -std=c++20
COMPILER_FLAGS = -Wall -Wfatal-errors

# ���幹��Ŀ¼
BUILD_DIR = ./build

# ������ԴĿ¼
ASSETS_DIR = ./assets

# ����һ���ݹ���Һ��� rwildcard
rwildcard = $(wildcard $1$2) $(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2))

# Դ�����ļ������� ./src ����������Ŀ¼�µ� .cpp �ļ�
SRC_FILES := $(call rwildcard,./src/,*.cpp)

# Ϊÿ��Դ�ļ����ɶ�Ӧ�Ķ����ļ�·�������ж����ļ�ֱ�ӷ��� ./build Ŀ¼��
OBJ_FILES := $(patsubst ./src/%.cpp,$(BUILD_DIR)/%.o,$(SRC_FILES))

# ����Ŀ¼�����ڲ���ͷ�ļ���
INCLUDE_PATH = -I"./include/"

# ��̬��Ŀ¼
LIBS_PATH = -L"./lib/Lua/" -L"./lib/SDL2/x64/" -L"./lib/SDL2_image/x64/" -L"./lib/SDL2_ttf/x64/" -L"./lib/SDL2_mixer/x64/"

# ��̬������
LIBS =  -llua54 -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer

# ��̬��Ŀ¼
DLLS_PATH = ./dll

# ��̬�����ӣ������Ҫ������ʱ���Ƶ���ִ���ļ�Ŀ¼��
DLLS = $(call rwildcard,$(DLLS_PATH),*.dll)

# ���ݲ���ϵͳ��������Ŀ�����Ƽ�ɾ������
ifeq ($(OS),Windows_NT)
TARGET = $(BUILD_DIR)/GameEngine.exe
RM = rmdir /S /Q "$(BUILD_DIR)"  # ʹ�� rmdir ɾ���ļ���
else
TARGET = $(BUILD_DIR)/GameEngine
RM = rm -rf "$(BUILD_DIR)"
endif

####################################################################################################
# Ĭ��Ŀ�꣺�ȴ�������Ŀ¼���ٹ�����ִ���ļ�
all: $(BUILD_DIR) $(TARGET)

# ȷ������Ŀ¼���ڣ�ƽ̨�������
$(BUILD_DIR):
ifeq ($(OS),Windows_NT)
	@if not exist "$(BUILD_DIR)" mkdir "$(BUILD_DIR)"
else
	@mkdir -p $(BUILD_DIR)
endif

# ����Ŀ���ļ����ɿ�ִ���ļ�������������Ŀ�
$(TARGET): $(OBJ_FILES)
	@echo "------------------------------------------"
	@echo "== ����Ŀ���ļ����ɿ�ִ���ļ� =="
	@echo "BUILD_DIR = $(BUILD_DIR)"
	@echo "OBJ_FILES = $(OBJ_FILES)"
	@echo "TARGET = $(TARGET)"
	@echo "LIBS = $(LIBS)"
	@echo "------------------------------------------"
	$(CXX) $(OBJ_FILES) -o $(TARGET) $(LIBS_PATH) $(LIBS)

# ���ɶ����ļ������ж����ļ����� ./build Ŀ¼��
$(BUILD_DIR)/%.o: ./src/%.cpp
	@echo "------------------------------------------"
	@echo "== ����Դ�ļ� =="
	@echo "Դ�ļ�: $<"
	@echo "Ŀ������ļ�: $@"
	@echo "Ŀ��Ŀ¼: $(dir $@)"
	@echo "------------------------------------------"
ifeq ($(OS),Windows_NT)
	@if not exist "$(subst /,\,$(patsubst %/,%,$(dir $@)))" mkdir "$(subst /,\,$(patsubst %/,%,$(dir $@)))"
else
	@mkdir -p $(dir $@)
endif
	$(CXX) $(COMPILER_FLAGS) $(LANG_STD) $(INCLUDE_PATH) -c $< -o $@

####################################################################################################
# ���ƶ�̬�����Դ������Ŀ¼������ж�̬����Ҫ������ʱ���ƣ�
copy_dll:
	@echo "------------------------------------------"
	@echo "== ���ƶ�̬���ӿ����Դ��Ŀ��Ŀ¼ ==";
	@echo "���ƵĿ��ļ�: $(DLLS)";
	@echo "Ŀ��Ŀ¼: $(BUILD_DIR)";
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
# ����Ŀ������ȸ��ƶ�̬�⣬�����г���
run: $(TARGET) copy_dll
	@echo "------------------------------------------"
	@echo "== ���п�ִ���ļ� =="
	@echo "ִ���ļ�: $(TARGET)"
	@echo "------------------------------------------"
	./$(TARGET)

####################################################################################################
# ������Ŀ¼
clean:
	@echo "------------------------------------------"
	@echo "== ������Ŀ¼ =="
	@echo "ɾ��Ŀ¼: $(BUILD_DIR)"
	@echo "------------------------------------------"
	$(RM)

.PHONY: all run clean copy_dll

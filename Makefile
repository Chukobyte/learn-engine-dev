MAKE = make --no-print-directory

CHAPTER_1_DIR = src/1.foundation/1.embedding_python
CHAPTER_2_DIR = src/1.foundation/2.game_loop_and_sdl
CHAPTER_3_DIR = src/1.foundation/3.opengl_rendering
CHAPTER_4_DIR = src/1.foundation/4.implementing_audio
CHAPTER_5_DIR = src/1.foundation/5.input_management
CHAPTER_6_DIR = src/1.foundation/6.entity_component_system
CHAPTER_7_DIR = src/1.foundation/7.serializing_with_json

# Maps to proper section directories
# CHAPTER 1
ifeq ($(SECTION),1.1.0)
	SECTION_DIR = $(CHAPTER_1_DIR)/1.0.hello_python
else ifeq ($(SECTION),1.1.1)
    SECTION_DIR = $(CHAPTER_1_DIR)/1.1.calling_a_function
else ifeq ($(SECTION),1.1.2)
    SECTION_DIR = $(CHAPTER_1_DIR)/1.2.using_pyhelper
else ifeq ($(SECTION),1.1.3)
    SECTION_DIR = $(CHAPTER_1_DIR)/1.3.create_python_instance
else ifeq ($(SECTION),1.1.4)
    SECTION_DIR = $(CHAPTER_1_DIR)/1.4.create_custom_module
# CHAPTER 2
else ifeq ($(SECTION),1.2.0)
    SECTION_DIR = $(CHAPTER_2_DIR)/2.0.game_loop_and_sdl
# CHAPTER 3
else ifeq ($(SECTION),1.3.0)
    SECTION_DIR = $(CHAPTER_3_DIR)/3.0.rendering_sprites
else ifeq ($(SECTION),1.3.1)
    SECTION_DIR = $(CHAPTER_3_DIR)/3.1.rendering_text
# CHAPTER 4
else ifeq ($(SECTION),1.4.0)
    SECTION_DIR = $(CHAPTER_4_DIR)/4.0.sdl_mixer
# CHAPTER 5
else ifeq ($(SECTION),1.5.0)
    SECTION_DIR = $(CHAPTER_5_DIR)/5.0.input_management
# CHAPTER 6
else ifeq ($(SECTION),1.6.0)
    SECTION_DIR = $(CHAPTER_6_DIR)/6.0.entity_component_system
# CHAPTER 7
else ifeq ($(SECTION),1.7.0)
    SECTION_DIR = $(CHAPTER_7_DIR)/7.0.parsing_json_properties_file
else ifeq ($(SECTION),1.7.1)
    SECTION_DIR = $(CHAPTER_7_DIR)/7.1.creating_scene_json_files
else
	SECTION_DIR = ''
endif

.PHONY: all build run validate-section-key

all: build run

build: validate-section-key
	@$(MAKE) -C $(SECTION_DIR) build

clean: validate-section-key
	@$(MAKE) -C $(SECTION_DIR) clean

run: validate-section-key
	@$(MAKE) -C $(SECTION_DIR) run

format: validate-section-key
	@$(MAKE) -C $(SECTION_DIR) format

debug-build: validate-section-key
	@$(MAKE) -C $(SECTION_DIR) debug-build

validate-section-key:
ifeq ($(SECTION_DIR),'')
	@echo Not a valid section key! Sections keys have the following syntax [PART]-[CHAPTER].[SECTION] e.g. make build SECTION=1.1.0
	@exit 1
endif

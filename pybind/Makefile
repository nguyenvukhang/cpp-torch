BUILD_DIR := .build
MICROMAMBA_DIR := /home/appliedai/v/.local/micromamba/envs/dfp
MICROMAMBA_DIR := /Users/khang/.local/micromamba/envs/ml
LIBARROW_PATH := $(MICROMAMBA_DIR)/lib/libarrow.so
PYTHON3 := $(MICROMAMBA_DIR)/bin/python3

current: clean build run

clean:
	rm -rf $(BUILD_DIR) .cache

build:
	mkdir -p $(BUILD_DIR)
	cd $(BUILD_DIR) && cmake ..
	cd $(BUILD_DIR) && cmake --build .
	ln -sf $(BUILD_DIR)/compile_commands.json .

run:
	@echo '────────────────────────────────────────────────────────────'
	LD_PRELOAD=$(LIBARROW_PATH) $(PYTHON3) main.py

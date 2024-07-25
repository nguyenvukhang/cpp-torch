BUILD_DIR := .build

current: build run

clean:
	rm -rf $(BUILD_DIR) .cache

build:
	mkdir -p $(BUILD_DIR)
	cd $(BUILD_DIR) && cmake ..
	cd $(BUILD_DIR) && cmake --build .
	ln -sf $(BUILD_DIR)/compile_commands.json .

run:
	@echo '────────────────────────────────────────────────────────────'
	time python3 main.py

BUILD_DIR := .build

# SETUP
#
# On macos (Sonoma) 14.5, I had to install `libomp` through homebrew and then
# run the command
# ```sh
# install_name_tool -add_rpath /opt/homebrew/opt/libomp/lib ~/.local/libtorch/lib/libtorch_cpu.dylib
# ```
#
# Before the build started working.

run:
	rm -rf $(BUILD_DIR)
	mkdir $(BUILD_DIR)
	cd $(BUILD_DIR) && cmake ..
	cd $(BUILD_DIR) && cmake --build . --config Release
	$(BUILD_DIR)/tt

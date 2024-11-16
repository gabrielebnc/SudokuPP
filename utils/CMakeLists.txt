# utils/CMakeLists.txt

# Step 1: Create the 'sudoku_utils' library
add_library(sudoku_utils STATIC
        src/math.cpp
)

# Step 2: Specify the include directories for the library
# Use relative paths (not absolute paths) to specify include directories
target_include_directories(sudoku_utils
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>   # This ensures paths are relative to the build directory
        $<INSTALL_INTERFACE:include>  # This ensures correct paths when installed
)

# Step 3: Optionally create an alias for easier referencing
add_library(SudokuPP::utils ALIAS sudoku_utils)

# Step 4: Install the utils library and its headers (optional)
install(TARGETS sudoku_utils
        EXPORT utils-config
        DESTINATION lib
)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/utils
        DESTINATION include
)

# Optional: Export the CMake config to allow other projects to find this library
install(EXPORT utils-config
        FILE utils-config.cmake
        DESTINATION lib/cmake/utils
)

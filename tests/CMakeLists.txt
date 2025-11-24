# CloneMine Test Suite
cmake_minimum_required(VERSION 3.20)

# Enable testing
enable_testing()

# Download and setup Google Test
include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        v1.14.0
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${CMAKE_SOURCE_DIR}/external/lua/include)

# Test executable for RPG data loading
add_executable(rpg_data_tests
    rpg/test_character_class.cpp
    rpg/test_lua_data_loader.cpp
    rpg/test_rpg_data_manager.cpp
    rpg/test_damage_scaling.cpp
    rpg/test_spell_loading.cpp
)

target_link_libraries(rpg_data_tests
    gtest_main
    gmock
    rpg
    lua
)

# Test executable for data validation
add_executable(data_validation_tests
    data/test_class_data_integrity.cpp
    data/test_spell_data_integrity.cpp
    data/test_monster_data_integrity.cpp
)

target_link_libraries(data_validation_tests
    gtest_main
    rpg
    lua
)

# Integration tests
add_executable(integration_tests
    integration/test_class_spell_integration.cpp
    integration/test_pet_system_integration.cpp
    integration/test_damage_calculation.cpp
)

target_link_libraries(integration_tests
    gtest_main
    rpg
    lua
)

# Register tests with CTest
include(GoogleTest)
gtest_discover_tests(rpg_data_tests)
gtest_discover_tests(data_validation_tests)
gtest_discover_tests(integration_tests)

# Add custom test target for running all tests
add_custom_target(run_all_tests
    COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
    DEPENDS rpg_data_tests data_validation_tests integration_tests
)

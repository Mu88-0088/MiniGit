cmake_minimum_required(VERSION 3.10)
project(MiniGit VERSION 1.0)

# [CONFIGURATION] ================================================
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
option(WITH_DIFF "Build with diff functionality" ON)  # ← Toggle diff here

# [DEPENDENCIES] ================================================
find_package(OpenSSL REQUIRED)  # For SHA-1 hashing

# [CORE SOURCES (PDF-required)] =================================
set(CORE_SOURCES
    src/init.cpp
    src/add.cpp
    src/commit.cpp
    src/log.cpp
    src/branch.cpp
    src/checkout.cpp
    src/merge.cpp
)

# [OPTIONAL DIFF] ===============================================
if(WITH_DIFF)
    message(STATUS "Building with diff support")
    list(APPEND CORE_SOURCES src/diff.cpp)  # ← Adds diff.cpp if enabled
else()
    message(STATUS "Building without diff support")
endif()

# [EXECUTABLE] ==================================================
add_executable(minigit ${CORE_SOURCES})
target_include_directories(minigit PRIVATE src)
target_link_libraries(minigit PRIVATE OpenSSL::SSL)

# [INSTALLATION] ================================================
install(TARGETS minigit DESTINATION bin)
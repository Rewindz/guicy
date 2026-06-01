cmake_minimum_required(VERSION 3.28)

project(guicy LANGUAGES C CXX)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)
set(FETCHCONTENT_QUIET OFF)

FetchContent_Declare(rzutils
  GIT_REPOSITORY https://github.com/Rewindz/rzutils.git
  GIT_TAG main
  GIT_SHALLOW ON
)

FetchContent_MakeAvailable(rzutils)

find_package(wxWidgets REQUIRED COMPONENTS core base)
include(${wxWidgets_USE_FILE})

add_executable(guicy WIN32)
target_compile_features(guicy PRIVATE cxx_std_23)
target_link_libraries(guicy PRIVATE rzutils ${wxWidgets_LIBRARIES})
target_sources(guicy PRIVATE
  src/main.cpp
  src/GuicyFrame.cpp
  src/Calculation.cpp
)

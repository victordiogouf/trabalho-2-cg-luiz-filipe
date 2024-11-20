cmake_minimum_required(VERSION 3.20)
project("Trab2CG")

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(program_executable_name ${CMAKE_PROJECT_NAME})

file(GLOB_RECURSE src_files CONFIGURE_DEPENDS src/*.cpp)

set(gcc_like_cxx "$<COMPILE_LANG_AND_ID:CXX,ARMClang,AppleClang,Clang,GNU,LCC>")
set(msvc_cxx "$<COMPILE_LANG_AND_ID:CXX,MSVC>")
set(compile_options
  "$<${gcc_like_cxx}:-pedantic-errors;-Wall;-Weffc++;-Wextra>"
  "$<${msvc_cxx}:/permissive-;/W4;/WX>"
)

include(FetchContent)

FetchContent_Declare(
  SFML
  GIT_REPOSITORY https://github.com/SFML/SFML.git
  GIT_TAG 0e06af987deadc0240ae1286e1742a0ee5e758a5
)
FetchContent_MakeAvailable(SFML)

FetchContent_Declare(
	glm
	GIT_REPOSITORY	https://github.com/g-truc/glm.git
	GIT_TAG 	33b4a621a697a305bc3a7610d290677b96beb181 
)
FetchContent_MakeAvailable(glm)

find_package(OpenGL REQUIRED)

add_executable(${program_executable_name} ${src_files})
target_include_directories(${program_executable_name} PRIVATE include)
target_compile_options(${program_executable_name} PRIVATE ${compile_options})
target_link_libraries(
  ${program_executable_name}
  glm::glm
  sfml-graphics
  sfml-window
  sfml-system
  OpenGL::GL
)
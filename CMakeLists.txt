cmake_minimum_required(VERSION 3.14)

project(Direct3DExample)
include(CMakePrintHelpers)

# find_package(Git REQUIRED)
set(_path "${CMAKE_SOURCE_DIR}")
set(_main "${_path}/BoxApp.cpp")
file(GLOB _files "${_path}/Common/*.cpp")
include_directories("${_path}/Common")

get_filename_component(_name "DX12_SKELETON" NAME)
add_executable(${_name} WIN32 ${_files} ${_main})

target_compile_definitions(${_name} PRIVATE "UNICODE" "_UNICODE")
target_link_libraries(${_name} PRIVATE "d3d12.lib" "dxgi.lib" "d3dcompiler.lib")

# May not be necessary since I changed the compile path for the shader
# add_custom_command(
# TARGET ${_name}
# POST_BUILD
# COMMAND "${CMAKE_COMMAND}" -E copy ${_shader} $<TARGET_FILE_DIR:${_name}>
# )
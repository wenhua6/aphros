set(CSD "${CMAKE_CURRENT_SOURCE_DIR}")
get_filename_component(p ${CSD} NAME)

set(EXE t.${p})

add_executable(${EXE} main.cpp)

target_link_libraries(${EXE} sysinfo)
     
add_test(NAME ${p} COMMAND ${EXE})

set_tests_properties(${p} PROPERTIES RUN_SERIAL true)

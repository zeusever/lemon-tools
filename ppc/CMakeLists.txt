lemon_c_cxx_files(SRC)

lemon_exe_project(tools-lemon-ppc RENAME lemon-ppc ${SRC})

target_link_libraries(tools-lemon-ppc lemon-ppc)
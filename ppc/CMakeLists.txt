lemon_c_cxx_files(SRC)

lemon_library_project(tools-lemon-ppc RENAME lemon-ppc ${SRC})

target_link_libraries(tools-lemon-ppc lemonxx)
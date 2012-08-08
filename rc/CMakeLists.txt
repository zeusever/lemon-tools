lemon_c_cxx_files(SRC)

lemon_exe_project(tools-lemon-rc RENAME lemon-rc ${SRC}) 

target_link_libraries(tools-lemon-rc lemon-sys lemon-io lemon-lua lemonxx)
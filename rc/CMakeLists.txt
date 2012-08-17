lemon_c_cxx_files(SRC)

lemon_exe_project(tools-lemon-rc RENAME lemon-rc BOOTSTRAP1 ${SRC}) 

target_link_libraries(tools-lemon-rc lemon-sys lemon-lua lemon-resource lemonxx)
include(${PROJECT_SOURCE_DIR}/sources/lemonxx/cmake/lemonxx.cmake)

include_directories(BEFORE ${PROJECT_BINARY_DIR}/bootstrap/ ${PROJECT_BINARY_DIR}/bootstrap/sources)

lemon_c_cxx_files(SRC PATH ${PROJECT_SOURCE_DIR}/tools/lemon/rc)

lemonxx_ctg_files(CTGS BOOTSTRAP DIR lemonxx)

lemon_c_cxx_files(SYS_FILES PATH ${PROJECT_SOURCE_DIR}/sources/lemon/sys DIR sys)

lemon_c_cxx_files(LUA_FILES PATH ${PROJECT_SOURCE_DIR}/sources/lemon/lua DIR lua)

lemon_c_cxx_files(MEMORY_FILES PATH ${PROJECT_SOURCE_DIR}/sources/lemon/memory DIR memory)

lemon_c_cxx_files(RESOURCE_FILES PATH ${PROJECT_SOURCE_DIR}/sources/lemon/resource DIR resource)

lemon_c_cxx_files(LEMONXX_FILES PATH ${PROJECT_SOURCE_DIR}/sources/lemonxx DIR lemonxx)

lemon_project_config(RC_CONFIG_FILES tools-lemon-rc BOOTSTRAP PATH ${PROJECT_SOURCE_DIR}/tools/lemon/rc)

lemon_project_info(RC_ASSEMBLY_FILES tools-lemon-rc "1.0.*" PATH ${PROJECT_SOURCE_DIR}/tools/lemon/rc)

lemon_project_config(SYS_CONFIG_FILES lemon-sys BOOTSTRAP PATH ${PROJECT_SOURCE_DIR}/sources/lemon/sys DIR sys )

lemon_project_info(SYS_ASSEMBLY_FILES lemon-sys "1.0.*" PATH ${PROJECT_SOURCE_DIR}/sources/lemon/sys DIR sys)

lemon_project_config(LEMONXX_CONFIG_FILES lemonxx BOOTSTRAP PATH ${PROJECT_SOURCE_DIR}/sources/lemonxx DIR lemonxx )

lemon_project_info(LEMONXX_ASSEMBLY_FILES lemonxx "1.0.*" PATH ${PROJECT_SOURCE_DIR}/sources/lemonxx DIR lemonxx)

lemon_project_config(MEMORY_CONFIG_FILES lemon-memory BOOTSTRAP PATH ${PROJECT_SOURCE_DIR}/sources/lemon/memory DIR memory )

lemon_project_info(MEMORY_ASSEMBLY_FILES lemon-memory "1.0.*" PATH ${PROJECT_SOURCE_DIR}/sources/lemon/memory DIR memory)

lemon_project_config(RESOURCE_CONFIG_FILES lemon-resource BOOTSTRAP PATH ${PROJECT_SOURCE_DIR}/sources/lemon/resource DIR resource )

lemon_project_info(RESOURCE_ASSEMBLY_FILES lemon-resource "1.0.*" PATH ${PROJECT_SOURCE_DIR}/sources/lemon/resource DIR resource)

lemon_project_config(LUA_CONFIG_FILES lemon-lua BOOTSTRAP PATH ${PROJECT_SOURCE_DIR}/sources/lemon/lua DIR lua )

if(WIN32)
	add_definitions(/DLEMON_SYS_BUILD /DLEMON_IO_BUILD)

	add_definitions(/D_CRT_SECURE_NO_WARNINGS /wd"4244" /wd"4127" /wd"4702" /wd"4324")

else()
	add_definitions(-DLEMON_SYS_BUILD -DLEMON_IO_BUILD)
endif()

lemon_exe_project(tools-lemon-boostrap-rc BOOTSTRAP RENAME lemon-boostrap-rc ${RESOURCE_ASSEMBLY_FILES} ${RESOURCE_CONFIG_FILES} ${RESOURCE_FILES} ${RC_CONFIG_FILES} ${RC_ASSEMBLY_FILES} ${SRC} ${LUA_FILES} ${CTGS} ${MEMORY_FILES} ${MEMORY_ASSEMBLY_FILES} ${LEMONXX_ASSEMBLY_FILES} ${LEMONXX_FILES} ${SYS_FILES} ${IO_FILES} ${SYS_ASSEMBLY_FILES} ${IO_ASSEMBLY_FILES}) 

if(NOT WIN32)
	target_link_libraries(tools-lemon-boostrap-rc dl)
endif()
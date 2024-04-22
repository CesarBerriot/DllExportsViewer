set(CXX_COMPILER_PATH ${CMAKE_CXX_COMPILER})
cmake_path(REMOVE_FILENAME CXX_COMPILER_PATH)

macro(mingw_dll_copy_command FILENAME)
	add_custom_command(TARGET DllExportsViewer
			COMMAND ${CMAKE_COMMAND} -E copy ${CXX_COMPILER_PATH}/${FILENAME} ${CMAKE_CURRENT_BINARY_DIR})
endmacro()


mingw_dll_copy_command(libwinpthread-1.dll)
mingw_dll_copy_command(libgcc_s_seh-1.dll)
mingw_dll_copy_command(libstdc++-6.dll)

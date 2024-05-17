# toolchain-mingw.cmake
set(CMAKE_SYSTEM_NAME Windows)

# Specify the cross compiler
set(CMAKE_C_COMPILER /usr/bin/x86_64-w64-mingw32-gcc)
set(CMAKE_CXX_COMPILER /usr/bin/x86_64-w64-mingw32-g++)

# Where to look for the target environment
set(CMAKE_FIND_ROOT_PATH /usr/x86_64-w64-mingw32)

# Adjust the default behavior of the FIND_XXX() commands:
# search programs in the host environment
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
# search headers and libraries in the target environment
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

# Define path for the raylib library and include files
set(RAYLIB_ROOT "/home/ginko/.local/src/raylib-5.0_win64_mingw-w64/")  # Adjust this path to where raylib is located
set(raylib_LIBRARY "/home/ginko/.local/src/raylib-5.0_win64_mingw-w64/lib/raylib.dll")
set(raylib_INCLUDE_DIR "/home/ginko/.local/src/raylib-5.0_win64_mingw-w64/include/")
set(CMAKE_LIBRARY_PATH ${RAYLIB_ROOT}/lib)
set(CMAKE_INCLUDE_PATH ${RAYLIB_ROOT}/include)

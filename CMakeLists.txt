# Specify the minimum required version of CMake to build the project.
cmake_minimum_required( VERSION 3.6 )
project( CC_Fractal_Suite )
# Locate Houdini's libraries and header files.
# Registers an imported library target named 'Houdini'.
find_package( Houdini REQUIRED )
# Add a library with two source files.
set( library_name CC_Fractal_Suite )
add_library( ${library_name} SHARED
    src/COP2_FractalMatte.cpp
	src/COP2_FractalMatte.h
	src/COP2_Mandelbrot.cpp
    src/COP2_Mandelbrot.h
	src/COP2_Buddhabrot.cpp
    src/COP2_Buddhabrot.h
    src/Fractal.cpp
    src/Fractal.h
    src/FractalSpace.cpp
    src/FractalSpace.h
    src/Mandelbrot.cpp
    src/Mandelbrot.h
    src/register.cpp
    src/register.h
)
# Link against the Houdini libraries, and add required include directories and compile definitions.
target_link_libraries( ${library_name} Houdini )
# Configure several common target properties, such as its output directory.
houdini_configure_target( ${library_name} )

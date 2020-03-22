cmake_minimum_required( VERSION 3.6 )
project( CC_Fractal_Suite )

find_package( Houdini REQUIRED )
# Add a library with source files
set( library_name CC_Fractal_Suite )
add_library( ${library_name} SHARED
	src/COP2_Buddhabrot.cpp
	include/COP2_Buddhabrot.h
	src/COP2_FractalMatte.cpp
	include/COP2_FractalMatte.h
	src/COP2_Lyapunov.cpp
	include/COP2_Lyapunov.h
	src/COP2_Mandelbrot.cpp
	include/COP2_Mandelbrot.h
	src/COP2_Pickover.cpp
	include/COP2_Pickover.h
	include/Fractal.h
	include/FractalNode.h
	src/FractalSpace.cpp
	include/FractalSpace.h
	src/Lyapunov.cpp
	include/Lyapunov.h
	src/Mandelbrot.cpp
	include/Mandelbrot.h
	src/register.cpp
	include/register.h
	src/StashData.cpp
	include/StashData.h
	include/typedefs.h
)

# Specify repo's include dir as a cmake include directory.
# Without this, the cpp files won't find our headers'
target_include_directories(${library_name} PUBLIC include)

# Link against the Houdini libraries, and add required include directories and compile definitions.
target_link_libraries( ${library_name} Houdini )
# Configure several common target properties, such as its output directory.

# Configure some variables
set(release_path ${CC_Fractal_Suite_BINARY_DIR}/release)
set(houdini_ver houdini18.0)
set(ccfs_hfs_version hfs18.0.391.1)  # hfs{HMAJOR}.{HMINOR}.{HBUILD}.{CCFSBUILDVERSION}

file(COPY ${CC_Fractal_Suite_SOURCE_DIR}/media DESTINATION ${release_path})
file(COPY ${CC_Fractal_Suite_SOURCE_DIR}/${houdini_ver} DESTINATION ${release_path})

# Configure the Houdini target
houdini_configure_target( ${library_name} INSTDIR ${release_path}/${houdini_ver}/dso )

# Set 'Release' as the default build type, which will build Doxygen.
if (NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE "Release")
endif()

# Build Docs, only if in release mode and if Doxygen found
option(BUILD_DOC "Build documentation" ON)

if (BUILD_DOC)
	find_package(Doxygen)
	if (Doxygen_FOUND)
		if (CMAKE_BUILD_TYPE MATCHES "[Rr]elease")
			# Set the input and output Doxyfiles
			set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in)
			set(DOXYGEN_OUT ${release_path}/Doxyfile)

			configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)
			message("Doxygen build started")
			# note the option ALL which allows to build the docs together with the application
			add_custom_target( doc_doxygen ALL
				COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
				WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
				COMMENT "Generating API documentation with Doxygen"
				VERBATIM )
		else()
			message(STATUS "CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE} does not match '[Rr]elease. Doxygen not run.'")
			endif()
	else()
		message(STATUS "Doxygen not found.")
	endif()
else()
	message(STATUS "Doxygen not run because BUILD_DOC option not enabled.")
endif()

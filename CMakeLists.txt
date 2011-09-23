# Olivier Stasse, Francois Keith, Nicolas Perrin
# Creation: 13/07/2011
#
# Copyright CNRS/AIST, LAAS-CNRS

cmake_minimum_required(VERSION 2.6)

enable_language(CXX)

# Debug mode for cmake.
SET(CMAKE_VERBOSE_MAKEFILE ON)

# Set version
SET(PROJECT_VERSION 0.9.99)

IF(WIN32)
  SET(LIBDIR_KW "/LIBPATH:")
  SET(LIBINCLUSION_KW "")
  SET(LIBINCLUSION_EXT ".lib")
ENDIF(WIN32)

IF(UNIX)
  SET(LIBDIR_KW "-L")
  SET(LIBINCLUSION_KW "-l")
  SET(LIBINCLUSION_EXT "")
ENDIF(UNIX)

# name of the project
SET(PROJECT_NAME assimpBasedViewer)
SET(PROJECT_CONTACT )
SET(PROJECT_DESCRIPTION "")
project(${PROJECT_NAME})

# SET(CMAKE_INSTALL_PREFIX "./build/")

# Impose shared libraries
#------------------------
OPTION(BUILD_SHARED_LIBS "Build it with shared libraries." ON)

# Check for required packages
# -------------------------
INCLUDE(FindPkgConfig)

pkg_check_modules(ASSIMP REQUIRED assimp>=2.0)
pkg_check_modules(GLU REQUIRED glu>=7.9)
pkg_check_modules(GLUT REQUIRED glut>=3.9)


# -----

SET(PROJECT_REQUIREMENTS "assimp >= 2.0 glu >= 7.9 glut >= 3.9")

# -----

MESSAGE(STATUS "${ASSIMP_CFLAGS}")
MESSAGE(STATUS "${GLU_CFLAGS}")
MESSAGE(STATUS "${GLUT_CFLAGS}")

# -----

SET(${PROJECT_NAME}_CXXFLAGS "-Wall ${${PROJECT_NAME}_CXXFLAGS}")

# -----

FOREACH(option_cxxflags ${ASSIMP_CFLAGS})
  SET(${PROJECT_NAME}_CXXFLAGS "${option_cxxflags} ${${PROJECT_NAME}_CXXFLAGS}")
ENDFOREACH(option_cxxflags)

FOREACH(option_cxxflags ${GLU_CFLAGS})
  SET(${PROJECT_NAME}_CXXFLAGS "${option_cxxflags} ${${PROJECT_NAME}_CXXFLAGS}")
ENDFOREACH(option_cxxflags)

FOREACH(option_cxxflags ${GLUT_CFLAGS})
  SET(${PROJECT_NAME}_CXXFLAGS "${option_cxxflags} ${${PROJECT_NAME}_CXXFLAGS}")
ENDFOREACH(option_cxxflags)

# -----

FOREACH(option_ldflags ${ASSIMP_LDFLAGS})
  SET(${PROJECT_NAME}_LDFLAGS "${option_ldflags} ${${PROJECT_NAME}_LDFLAGS}")
ENDFOREACH(option_ldflags)

FOREACH(option_ldflags ${GLU_LDFLAGS})
  SET(${PROJECT_NAME}_LDFLAGS "${option_ldflags} ${${PROJECT_NAME}_LDFLAGS}")
ENDFOREACH(option_ldflags)

FOREACH(option_ldflags ${GLUT_LDFLAGS})
  SET(${PROJECT_NAME}_LDFLAGS "${option_ldflags} ${${PROJECT_NAME}_LDFLAGS}")
ENDFOREACH(option_ldflags)

# -----

SET(${PROJECT_NAME}_src_LDFLAGS ${${PROJECT_NAME}_LDFLAGS})
SET(${PROJECT_NAME}_LDFLAGS "${${PROJECT_NAME}_LDFLAGS}")

# -------------------------

add_subdirectory(src)
add_subdirectory(unitTesting)

# Prepare the documentation
# -------------------------
#INCLUDE(FindDoxygen)
#IF(DOXYGEN_FOUND)
#  MESSAGE(STATUS "Doxygen found")
#  add_subdirectory(doc)
#ENDIF(DOXYGEN_FOUND)

# Prepare description file for pkg-config.
#-----------------------------------------

SET(install_pkg_prefix "\${prefix}")
SET(install_pkg_exec_prefix "\${exec_prefix}")
SET(install_pkg_libdir "\${libdir}")
SET(install_pkg_include_dir "\${includedir}")

# Install the file in the appropriate place.
SET(install_libdir_pkgconfig ${CMAKE_INSTALL_PREFIX}/lib/pkgconfig)

CONFIGURE_FILE(${${PROJECT_NAME}_SOURCE_DIR}/${PROJECT_NAME}.pc.cmake
		 ${${PROJECT_NAME}_BINARY_DIR}/${PROJECT_NAME}.pc )

INSTALL(FILES ${${PROJECT_NAME}_BINARY_DIR}/${PROJECT_NAME}.pc
	DESTINATION ${install_libdir_pkgconfig}	
        PERMISSIONS OWNER_READ GROUP_READ WORLD_READ OWNER_WRITE )

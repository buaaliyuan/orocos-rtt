###########################################################
#                                                         #
# Look for dependencies required by individual components #
#                                                         #
###########################################################

# For examples of dependency checks see orca-components/config/check_depend.cmake

# Look for Player (ctually looks for v.>=1.6)
# INCLUDE (${PROJ_SOURCE_DIR}/config/FindPlayer2.cmake)

SET(CURSES_INCLUDE_DIR 0 INTERNAL)
INCLUDE( ${CMAKE_ROOT}/Modules/FindCurses.cmake )
IF ( CURSES_INCLUDE_DIR )
    MESSAGE("-- Looking for libncurses - found")
ELSE ( CURSES_INCLUDE_DIR )
    MESSAGE("-- Looking for libncurses - not found")
ENDIF ( CURSES_INCLUDE_DIR )

# Look for boost
IF ( EXISTS boost/shared_ptr.hpp AND EXISTS boost/spirit.hpp AND EXISTS boost/graph/adjacency_list.hpp)
  MESSAGE("-- Looking for Boost headers - found")
  SET(ORO_SUPPORT_BOOST TRUE CACHE INTERNAL "" FORCE)
ELSE ( EXISTS boost/shared_ptr.hpp AND EXISTS boost/spirit.hpp AND EXISTS boost/graph/adjacency_list.hpp )
  MESSAGE("-- Looking for Boost headers- not found")
  SET(ORO_SUPPORT_BOOST FALSE CACHE INTERNAL "" FORCE)
ENDIF ( EXISTS boost/shared_ptr.hpp AND EXISTS boost/spirit.hpp AND EXISTS boost/graph/adjacency_list.hpp )

# Look for Xerces
SET(XERCES ${XERCES} INTERNAL "if xerces is found")
FIND_LIBRARY(XERCES NAMES xerces-c 
  PATHS /usr/local/lib /usr/lib ${XERCES_INCLUDE_DIR}/../lib)
IF ( XERCES )
  MESSAGE("-- Looking for Xerces - found")
  SET(OROPKG_SUPPORT_XERCES_C TRUE CACHE INTERNAL "" FORCE)
  SET(MARSH_INCLUDE \"marsh/CPFDemarshaller.hpp\" CACHE INTERNAL "" FORCE)
  SET(MARSHALLER CPFDemarshaller CACHE INTERNAL "" FORCE)
ELSE ( XERCES )
  MESSAGE("-- Looking for Xerces - not found")
  SET(OROPKG_SUPPORT_XERCES_C FALSE CACHE INTERNAL "" FORCE)
  SET(MARSH_INCLUDE \"marsh/TinyDemarshaller.hpp\" CACHE INTERNAL "" FORCE)
  SET(MARSHALLER TinyDemarshaller CACHE INTERNAL "" FORCE)
ENDIF ( XERCES )

SET( OROCOS_TARGET gnulinux CACHE STRING "The Operating System target. One of [lxrt gnulinux xenomai]")
SET(LINUX_SOURCE_DIR ${LINUX_SOURCE_DIR} CACHE PATH "path to linux source dir" FORCE)

# Look for TAO and ACE
INCLUDE(${PROJ_SOURCE_DIR}/config/FindCorbaDeps.cmake)
IF(TAO_FOUND)
  #EXEC_PROGRAM(tao_idl)
ENDIF(TAO_FOUND)


IF(OROCOS_TARGET STREQUAL "lxrt")
  SET(OROPKG_OS_LXRT TRUE CACHE INTERNAL "" FORCE)
  # Look for LXRT
  SET(RTAI_INSTALL_DIR ${RTAI_INSTALL_DIR} CACHE PATH "path to rtai installation dir" FORCE)
  IF(EXISTS ${RTAI_INSTALL_DIR}/include/rtai_lxrt.h)
    MESSAGE("-- Looking for LXRT - found")
    SET(OROPKG_SUPPORT_RTAI TRUE CACHE INTERNAL "" FORCE)
    INCLUDE_DIRECTORIES(${RTAI_INSTALL_DIR}/include ${LINUX_SOURCE_DIR}/include)
  ELSE(EXISTS ${RTAI_INSTALL_DIR}/include/rtai_lxrt.h)
    MESSAGE("-- Looking for LXRT - not found")
    SET(OROPKG_SUPPORT_RTAI FALSE CACHE INTERNAL "" FORCE)
  ENDIF(EXISTS ${RTAI_INSTALL_DIR}/include/rtai_lxrt.h)
ELSE(OROCOS_TARGET STREQUAL "lxrt")
  SET(RTAI_INSTALL_DIR "/usr/realtime" CACHE INTERNAL "path to rtai installation dir")
  SET(OROPKG_OS_LXRT FALSE CACHE INTERNAL "" FORCE)
ENDIF(OROCOS_TARGET STREQUAL "lxrt")


IF(OROCOS_TARGET STREQUAL "xenomai")
  # Look for Xenomai
  SET(XENOMAI_INSTALL_DIR ${XENOMAI_INSTALL_DIR} CACHE PATH "path to xenomai installation dir" FORCE)
  SET(OROPKG_OS_XENOMAI TRUE CACHE INTERNAL "" FORCE)
  IF(EXISTS ${XENOMAI_INSTALL_DIR}/include/native.h)
    MESSAGE("-- Looking for XENOMAI - found")
    SET(XENOMAI_SUPPORT TRUE CACHE INTERNAL FORCE)
  ELSE(EXISTS ${XENOMAI_INSTALL_DIR}/include/native.h)
    MESSAGE("-- Looking for XENOMAI - not found")
    SET(XENOMAI_SUPPORT FALSE CACHE INTERNAL FORCE)
  ENDIF(EXISTS ${XENOMAI_INSTALL_DIR}/include/native.h)
ELSE(OROCOS_TARGET STREQUAL "xenomai")
  SET(XENOMAI_INSTALL_DIR "/usr/realtime" CACHE INTERNAL "path to xenomai installation dir")
  SET(OROPKG_OS_XENOMAI FALSE CACHE INTERNAL "" FORCE)
ENDIF(OROCOS_TARGET STREQUAL "xenomai")


IF(OROCOS_TARGET STREQUAL "gnulinux")
  SET(OROPKG_OS_GNULINUX TRUE CACHE INTERNAL "" FORCE)
ELSE(OROCOS_TARGET STREQUAL "gnulinux")
  SET(OROPKG_OS_GNULINUX FALSE CACHE INTERNAL "" FORCE)
ENDIF(OROCOS_TARGET STREQUAL "gnulinux")


#Check machine type:
IF(UNIX)
  FIND_PROGRAM(CMAKE_UNAME uname /bin /usr/bin /usr/local/bin )
  IF(CMAKE_UNAME)
    EXEC_PROGRAM(uname ARGS -m OUTPUT_VARIABLE CMAKE_SYSTEM_PROCESSOR)
    SET(CMAKE_SYSTEM_PROCESSOR ${CMAKE_SYSTEM_PROCESSOR} CACHE INTERNAL "processor type (i386 and x86_64)")
  ELSE(CMAKE_UNAME)
    MESSAGE(SEND_ERROR "Can not determine machine-type")
  ENDIF(CMAKE_UNAME)
ELSE(UNIX)
  MESSAGE(SEND_ERROR "Machine-type determination only for unix types")
ENDIF(UNIX)

IF(CMAKE_SYSTEM_PROCESSOR STREQUAL "i386" OR CMAKE_SYSTEM_PROCESSOR STREQUAL "i486" OR CMAKE_SYSTEM_PROCESSOR STREQUAL "i586" OR CMAKE_SYSTEM_PROCESSOR STREQUAL "i686")
  SET(OROBLD_OS_ARCH_i386 TRUE CACHE INTERNAL "" FORCE)
  SET(OROBLD_OS_ARCH_x86_64 FALSE CACHE INTERNAL "" FORCE)
ELSE(CMAKE_SYSTEM_PROCESSOR STREQUAL "i386" OR CMAKE_SYSTEM_PROCESSOR STREQUAL "i486" OR CMAKE_SYSTEM_PROCESSOR STREQUAL "i586" OR CMAKE_SYSTEM_PROCESSOR STREQUAL "i686")
  IF(CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64")
    FILE( APPEND ${PROJ_BINARY_DIR}/src/rtt-config.h "#define OROBLD_OS_ARCH_x86_64\n")
    SET(OROBLD_OS_ARCH_i386 FALSE CACHE INTERNAL "" FORCE)
    SET(OROBLD_OS_ARCH_x86_64 TRUE CACHE INTERNAL "" FORCE)
  ELSE(CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64")
    MESSAGE(SEND_ERROR "Machine type not recognised")
  ENDIF(CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64")
ENDIF(CMAKE_SYSTEM_PROCESSOR STREQUAL "i386" OR CMAKE_SYSTEM_PROCESSOR STREQUAL "i486" OR CMAKE_SYSTEM_PROCESSOR STREQUAL "i586" OR CMAKE_SYSTEM_PROCESSOR STREQUAL "i686")


CONFIGURE_FILE( ${PROJ_SOURCE_DIR}/src/rtt-config.h.in ${PROJ_BINARY_DIR}/src/rtt-config.h @ONLY)

#
# Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license: https://github.com/azerothcore/azerothcore-wotlk/blob/master/LICENSE-AGPL3
#

# Set the module name here
set(MODULENAME "mod-discord")

# Add source files
file(GLOB_RECURSE SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

AC_ADD_SCRIPT("${SRC_FILES}")
AC_ADD_SCRIPT_LOADER("DiscordLink" "${CMAKE_CURRENT_SOURCE_DIR}/src/loader.cpp")

# Add config file
AC_ADD_CONFIG_FILE("${CMAKE_CURRENT_SOURCE_DIR}/conf/discord_link.conf.dist")

# Find required packages
find_package(CURL REQUIRED)
find_package(jsoncpp REQUIRED)

# Message to confirm CURL and JSONCPP are found
message(STATUS "CURL found: ${CURL_FOUND}")
message(STATUS "JSONCPP found: ${JSONCPP_FOUND}")

# Add include directories and link libraries
if(CURL_FOUND AND JSONCPP_FOUND)
    message(STATUS "Building ${MODULENAME}")

    # Need to link against common project libs
    target_link_libraries(${MODULENAME}
      PRIVATE
        acore-core-interface
        ${CURL_LIBRARIES}
        ${JSONCPP_LIBRARIES}
    )

    target_include_directories(${MODULENAME}
      PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CURL_INCLUDE_DIRS}
        ${JSONCPP_INCLUDE_DIRS}
    )
else()
    message(WARNING "CURL or JSONCPP not found - ${MODULENAME} module will not be built!")
    set(SKIP_MODULE ON)
endif()
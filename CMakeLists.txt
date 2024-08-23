#
# Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license: https://github.com/azerothcore/azerothcore-wotlk/blob/master/LICENSE-AGPL3
#

# Set the module name here
set(MODULE_NAME "mod-discord")

# Add the module's source files
AC_ADD_SCRIPT("${CMAKE_CURRENT_LIST_DIR}/src/DiscordLink.cpp")
AC_ADD_SCRIPT("${CMAKE_CURRENT_LIST_DIR}/src/loader.cpp")

# Add additional dependencies
AC_ADD_SCRIPT_LOADER("DiscordLink" "${CMAKE_CURRENT_LIST_DIR}/src/loader.cpp")

# Include the module's configuration directory
AC_ADD_CONFIG_FILE("${CMAKE_CURRENT_LIST_DIR}/conf/discord_link.conf.dist")

# Find required packages
find_package(CURL REQUIRED)
find_package(jsoncpp REQUIRED)

# Message to confirm CURL and JSONCPP are found
message(STATUS "CURL found: ${CURL_FOUND}")
message(STATUS "JSONCPP found: ${JSONCPP_FOUND}")

# Add include directories and link libraries to the module target
target_include_directories(${MODULE_NAME}
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CURL_INCLUDE_DIRS}
    ${JSONCPP_INCLUDE_DIRS}
)

target_link_libraries(${MODULE_NAME}
  PRIVATE
    ${CURL_LIBRARIES}
    ${JSONCPP_LIBRARIES}
)
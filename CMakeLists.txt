#
# Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license: https://github.com/azerothcore/azerothcore-wotlk/blob/master/LICENSE-AGPL3
#

# Set the module name here
set(MOD_DISCORD_LINK "mod-discord")

# Add the module's source files
AC_ADD_SCRIPT("${CMAKE_CURRENT_LIST_DIR}/src/DiscordLink.cpp")
AC_ADD_SCRIPT("${CMAKE_CURRENT_LIST_DIR}/src/loader.cpp")

# Add additional dependencies
find_package(CURL REQUIRED)
find_package(jsoncpp REQUIRED)

# Include directories
target_include_directories(${MOD_DISCORD_LINK}
    PRIVATE
        ${CURL_INCLUDE_DIRS}
        ${JSONCPP_INCLUDE_DIRS}
)

# Link against the additional libraries
target_link_libraries(${MOD_DISCORD_LINK}
    PRIVATE
        ${CURL_LIBRARIES}
        ${JSONCPP_LIBRARIES}
)

# Include the module's configuration directory
AC_ADD_CONFIG_FILE("${CMAKE_CURRENT_LIST_DIR}/conf/discord_link.conf.dist")

# Add the script loader
AC_ADD_SCRIPT_LOADER("DiscordLink" "${CMAKE_CURRENT_LIST_DIR}/src/loader.cpp")
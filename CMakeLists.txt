#
# Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license: https://github.com/azerothcore/azerothcore-wotlk/blob/master/LICENSE-AGPL3
#

# Set the module name here
set(MODULE_NAME "mod-discord")

# Set the module path
set(MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}")

# Add the module's source files
AC_ADD_SCRIPT("${MODULE_PATH}/src/DiscordLink.cpp")
AC_ADD_SCRIPT("${MODULE_PATH}/src/loader.cpp")

# Add additional dependencies
find_package(CURL REQUIRED)
find_package(jsoncpp REQUIRED)

# Link against the additional libraries
target_link_libraries(${MODULE_NAME} PRIVATE
    CURL::libcurl
    jsoncpp_lib
)

# Include the module's configuration directory
AC_ADD_CONFIG_FILE("${MODULE_PATH}/conf/discord_link.conf.dist")

# Register the module with AzerothCore
AC_ADD_SCRIPT_LOADER("DiscordLink" "${MODULE_PATH}/src/loader.cpp")
AC_ADD_SCRIPT("${CMAKE_CURRENT_LIST_DIR}/src/mount-at-twenty.cpp")
AC_ADD_SCRIPT_LOADER("MountAtTwenty" "${CMAKE_CURRENT_LIST_DIR}/src/loader.h")

AC_ADD_CONFIG_FILE("${CMAKE_CURRENT_LIST_DIR}/conf/mount-at-twenty.conf.dist")
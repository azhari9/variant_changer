cmake_minimum_required (VERSION 2.8.11)
project(VariantChanger)

find_package(Threads)
find_package(PkgConfig)

pkg_check_modules(DBUS REQUIRED dbus-1)
pkg_check_modules(DBUS-GLIB REQUIRED dbus-glib-1)
pkg_check_modules(GLIB REQUIRED glib-2.0)
pkg_check_modules(DLT REQUIRED automotive-dlt)

include_directories(${DBUS_INCLUDE_DIRS})
include_directories(${GLIB_INCLUDE_DIRS})
include_directories(${DBUS-GLIB_INCLUDE_DIRS})
include_directories(${DLT_INCLUDE_DIRS})
include_directories(./)

SET(sources 	variant_changer.c
		variant_changer.h
		sw_update.c
		ipc_test_app.c
		dbus_send_receive.c
   )

#SET(includes 	)

#SET(IPC_PROCESS_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

#SET(IPC_PROCESS_INC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/inc)

#SET(TC_LINK_INC_DIR ../libtc-link/inc)

#include_directories(${IPC_PROCESS_SRC_DIR})
#include_directories(${IPC_PROCESS_INC_DIR})
#include_directories(${TC_LINK_INC_DIR})


#find_library(TCLINK_LIB_PATH NAMES TCLink PATHS lib)
#message(tc-link-library --> ${TCLINK_LIB_PATH})
#add_executable(IPCTestApp ${sources} ${includes})
add_executable(VariantChanger ${sources} )

# it depends on pthread, glib-2.0 and dbus libraries
target_link_libraries(VariantChanger pthread -rdynamic ${DBUS_LIBRARIES} ${GLIB_LIBRARIES} ${DBUS-GLIB_LIBRARIES} ${DLT_LIBRARIES})

# tip for install command, define component public headers files
#set_target_properties(PowerProcessorIPC PROPERTIES PUBLIC_HEADER "AudioEnhancer.h")

install(TARGETS VariantChanger

        RUNTIME DESTINATION /usr/bin

        LIBRARY DESTINATION /usr/lib

	PUBLIC_HEADER DESTINATION /usr/include)

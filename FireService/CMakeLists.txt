# This is a cmake script. Process it with the CMake gui or command line utility
# to produce makefiles / Visual Studio project files on Mac OS X and Windows.
#
# To configure the build options either use the CMake gui, or run the command
# line utility including the "-i" option.

# usage in linux every path:
# cmake -DCMAKE_C_COMPILER=mips-openwrt-linux-gcc -DCMAKE_CXX_COMPILER=mips-openwrt-linux-g++

cmake_minimum_required(VERSION 3.17)

project(fire_service)

# Only for version 3 and up. cmake_policy(SET CMP0042 NEW)

#set (VERSION 0.0.1)

# set environment
#when export env is set, no need to do this ,unless you need IDE to debug
set(ENV{STAGING_DIR} /opt/OpenWrt-Toolchain-ar71xx-for-mips_34kc-gcc-4.8-linaro_uClibc-0.9.33.2/toolchain-mips_34kc_gcc-4.8-linaro_uClibc-0.9.33.2/bin)

# define micro
set(CMAKE_CXX_STANDARD 11)

#add -Dxx definitions, to that to_string can be used
add_definitions(-D_GLIBCXX_USE_C99=1)
add_definitions(-DCMAKE_BUILD_TYPE=Release)

#
#add_compile_options(-std=c++11)
#add_compile_options(-D_GLIBCXX_USE_C99 1)
#
#message(STATUS "add std=c++11")



# mqtt lib
include_directories(/opt/lib_pahomqtt/include)
link_directories(/opt/lib_pahomqtt/lib)
link_libraries(paho-mqtt3a
        paho-mqtt3as
        paho-mqtt3c
        paho-mqtt3cs
        )

link_directories(/opt/paho.mqtt.c/ar9331_openssl/lib)
link_libraries(crypto ssl)

link_directories(/opt/OpenWrt-Toolchain-ar71xx-for-mips_34kc-gcc-4.8-linaro_uClibc-0.9.33.2/toolchain-mips_34kc_gcc-4.8-linaro_uClibc-0.9.33.2/lib)

#dl: dlopen;
#rt" librc.so
link_libraries(pthread dl rt)


# add include file
include_directories(
        /opt/OpenWrt-Toolchain-ar71xx-for-mips_34kc-gcc-4.8-linaro_uClibc-0.9.33.2/toolchain-mips_34kc_gcc-4.8-linaro_uClibc-0.9.33.2/include
        src
        src/mqtt
        src/modbus
        src/glog
        src/glog/base
        src/glog/glog
        src/rapidjson
        src/rapidjson/error
        src/rapidjson/internal
   #     src/rapidjson/msinttypes
        )

#add_executable(fire_service testmain.cpp)

#
add_executable(fire_service
        src/gsvr_app_main.cpp
        src/gsvr_app_public.cpp
        src/globalFun.cpp
        src/gsvr_info_break.cpp
        src/gsvr_app_config.cpp
        src/gsvr_app_log.cpp
        src/gsvr_app_modbus.cpp
        src/gsvr_app_mqtt.cpp
        src/gsvr_app_server.cpp
        src/gsvr_app_threadpool.h
        src/gsvr_thread_listen.cpp
        src/gsvr_thread_report.cpp
        src/modbus/modbus-data.c
        src/modbus/modbus-rtu.c
        src/modbus/modbus.c
        src/mqtt/async_client.cpp
        src/mqtt/client.cpp
        src/mqtt/connect_options.cpp
        src/mqtt/create_options.cpp
        src/mqtt/disconnect_options.cpp
        src/mqtt/iclient_persistence.cpp
        src/mqtt/message.cpp
        src/mqtt/properties.cpp
        src/mqtt/response_options.cpp
        src/mqtt/ssl_options.cpp
        src/mqtt/string_collection.cpp
        src/mqtt/subscribe_options.cpp
        src/mqtt/token.cpp
        src/mqtt/topic.cpp
        src/mqtt/will_options.cpp
        src/glog/base/commandlineflags.h
        src/glog/base/googleinit.h
        src/glog/base/mutex.h
        src/glog/glog/log_severity.h
        src/glog/glog/logging.h.in
        src/glog/glog/raw_logging.h.in
        src/glog/demangle.cc
        src/glog/logging.cc
        src/glog/raw_logging.cc
        src/glog/signalhandler.cc
        src/glog/symbolize.cc
        src/glog/vlog_is_on.cc
        src/glog/utilities.cc
        src/gsvr_app_shmutex.c
        )

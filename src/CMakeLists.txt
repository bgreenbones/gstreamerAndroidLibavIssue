# The Flutter tooling requires that developers have CMake 3.10 or later
# installed. You should not increase this version, as doing so will cause
# the plugin to fail to compile for some customers of the plugin.
cmake_minimum_required(VERSION 3.10)


# project(native_binding_library VERSION 0.0.1 LANGUAGES C)


# Trying to get C++...
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
enable_language(CXX)
project(native_binding_library VERSION 0.0.1 LANGUAGES CXX C)



find_package(PkgConfig REQUIRED)

pkg_search_module(GST REQUIRED gstreamer-1.0)

IF(WIN32)
  find_program(CMAKE_PKGCONFIG_EXECUTABLE pkg-config)
  IF(CMAKE_PKGCONFIG_EXECUTABLE)
  # pkg-config.exe gstreamer-1.0 --libs --msvc-syntax
    EXEC_PROGRAM(${CMAKE_PKGCONFIG_EXECUTABLE}
    ARGS " --libs --msvc-syntax gstreamer-1.0"
    OUTPUT_VARIABLE GST_LDFLAGS)
    # replace spaces with semicolons so that we don't have quotation marks in command line option
    string(REPLACE " " ";" GST_LDFLAGS ${GST_LDFLAGS})
    message("GST_LDFLAGS: ${GST_LDFLAGS}")
  ENDIF()
ENDIF()

IF(ANDROID OR __ANDROID__)
  message(CMAKECURRENTSOURCEDIR:${CMAKE_CURRENT_SOURCE_DIR})
  SET(GST_FOLDER ${CMAKE_CURRENT_SOURCE_DIR}/../third-party/gst-android)
  SET(ABI_SUFFIX ${ANDROID_ABI})

  IF(${ANDROID_ABI} STREQUAL "armeabi-v7a")
    SET(ABI_SUFFIX armv7)
  ELSEIF(${ANDROID_ABI} STREQUAL "arm64-v8a")
    SET(ABI_SUFFIX arm64)
  ELSEIF(${ANDROID_ABI} STREQUAL "x86")
    # skipped
  ELSEIF(${ANDROID_ABI} STREQUAL "x86_64")
    # skipped
  ENDIF()

  SET(GST_ROOT ${GST_FOLDER}/${ABI_SUFFIX})

  # -I/usr/include/gstreamer-1.0 -I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include
  include_directories(
    ${GST_ROOT}/include/gstreamer-1.0
    ${GST_ROOT}/include/glib-2.0
    ${GST_ROOT}/lib/glib-2.0/include
  )

  link_directories(
    ${GST_ROOT}/lib
    ${GST_ROOT}/lib/gstreamer-1.0
  )

  SET(PLUGINS_DECLARATION)
  SET(PLUGINS_REGISTRATION)

  LIST(APPEND GST_PLUGINS coreelements coretracers adder app audioconvert audiomixer audiorate audioresample audiotestsrc compositor gio rawparse
    libav # linking libav
    autodetect opensles videobox videocrop autoconvert rtp rtpmanager rtsp)
  foreach(GST_P ${GST_PLUGINS})
    LIST(APPEND LINK_LIBS "gst${GST_P}")
    LIST(APPEND PLUGINS_DECLARATION "\nGST_PLUGIN_STATIC_DECLARE(${GST_P})")
    LIST(APPEND PLUGINS_REGISTRATION "\nGST_PLUGIN_STATIC_REGISTER(${GST_P})")
  endforeach()
  
  configure_file(gst_android.c.in ${CMAKE_CURRENT_SOURCE_DIR}/gst_plugin_init_android.c)

  LIST(APPEND APPENDED_SOURCE gst_plugin_init_android.c)

  LIST(APPEND LINK_LIBS intl ffi iconv gmodule-2.0
      pcre2-8
      gstbase-1.0 gstpbutils-1.0 gsttag-1.0 gstrtsp-1.0 gstaudio-1.0 gstvideo-1.0
      # linking libav
      bz2 swresample avutil avfilter avformat avcodec
      orc-0.4 gstapp-1.0 gio-2.0 log z OpenSLES)

  pkg_check_modules(UTILS_PKG REQUIRED IMPORTED_TARGET gstreamer-pbutils-1.0)
  pkg_check_modules(PARSE_PKG REQUIRED IMPORTED_TARGET gstreamer-codecparsers-1.0)

  # linking libav
  pkg_check_modules(BZ2_PKG REQUIRED IMPORTED_TARGET bzip2)
  pkg_check_modules(RESAMPLE_PKG REQUIRED IMPORTED_TARGET libswresample)
  pkg_check_modules(LIBAVUTIL_PKG REQUIRED IMPORTED_TARGET libavutil)
  pkg_check_modules(LIBAVFILTER_PKG REQUIRED IMPORTED_TARGET libavfilter)
  pkg_check_modules(LIBAVFORMAT_PKG REQUIRED IMPORTED_TARGET libavformat)
  pkg_check_modules(LIBAV_PKG REQUIRED IMPORTED_TARGET libavcodec)

  pkg_check_modules(NET_PKG REQUIRED IMPORTED_TARGET gstreamer-net-1.0)
  pkg_check_modules(RTP_PKG REQUIRED IMPORTED_TARGET gstreamer-rtp-1.0)
  pkg_check_modules(X264_PKG REQUIRED IMPORTED_TARGET x264)

  pkg_check_modules(RTSP_PKG REQUIRED IMPORTED_TARGET gstreamer-rtsp-1.0)
  LIST(APPEND LINK_PKGS PkgConfig::PARSE_PKG PkgConfig::NET_PKG
    # linking libav
    PkgConfig::BZ2_PKG PkgConfig::RESAMPLE_PKG PkgConfig::LIBAVUTIL_PKG PkgConfig::LIBAVFILTER_PKG PkgConfig::LIBAVFORMAT_PKG PkgConfig::LIBAV_PKG
    PkgConfig::RTP_PKG PkgConfig::X264_PKG PkgConfig::RTSP_PKG)
ELSE()
  include_directories(${GST_INCLUDE_DIRS})
  ENDIF()
  

# add_subdirectory(select_device)

add_library(native_binding SHARED
  native_binding.cpp
  ${APPENDED_SOURCE}
)

target_link_libraries(native_binding)
# target_link_libraries(native_binding PRIVATE PkgConfig::TAG_PKG PkgConfig::RTP_PKG PkgConfig::X264_PKG PkgConfig::RTSP_PKG)

IF(WIN32)
  target_link_options(native_binding PRIVATE ${GST_LDFLAGS})
ELSE()
  # target_link_libraries(native_binding PRIVATE ${GST_LDFLAGS} ${LINK_LIBS})
  # target_link_libraries(native_binding PRIVATE ${GST_LDFLAGS} ${LINK_LIBS} ${LINK_PKGS})
  target_link_libraries(native_binding PRIVATE -Bsymbolic -Wl,-z,notext ${GST_LDFLAGS} ${LINK_LIBS} ${LINK_PKGS})
ENDIF()

set_target_properties(native_binding PROPERTIES
  PUBLIC_HEADER native_binding.h
  OUTPUT_NAME "native_binding"
)

target_compile_definitions(native_binding PUBLIC DART_SHARED_LIB)
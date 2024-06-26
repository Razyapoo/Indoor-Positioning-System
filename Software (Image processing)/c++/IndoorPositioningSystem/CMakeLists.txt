cmake_minimum_required(VERSION 3.5)

project(IndoorPositioningSystem VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(QT_PATH "/home/oskar/Qt/6.6.1/gcc_64")
set(CMAKE_PREFIX_PATH ${QT_PATH})

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS DBus Gui OpenGLWidgets Widgets LinguistTools Charts Multimedia MultimediaWidgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS DBus Gui OpenGLWidgets Widgets LinguistTools Multimedia MultimediaWidgets Charts)
find_package(OpenCV)


set(MANUAL_LIB_PATHS
    "/home/oskar/opt/local/local/lib/libxgboost.so"
    "/home/oskar/Qt/6.6.1/gcc_64/lib/libQt6Charts.so.6"
    "/home/oskar/Qt/6.6.1/gcc_64/lib/libQt6Charts.so.6.6.1"
    "/home/oskar/Qt/6.6.1/gcc_64/lib/libQt6Widgets.so.6"
    "/home/oskar/Qt/6.6.1/gcc_64/lib/libQt6Widgets.so.6.6.1"
    "/home/oskar/Qt/6.6.1/gcc_64/lib/libQt6Gui.so.6"
    "/home/oskar/Qt/6.6.1/gcc_64/lib/libQt6Gui.so.6.6.1"
    "/lib/x86_64-linux-gnu/libGLX.so.0"
    "/lib/x86_64-linux-gnu/libOpenGL.so.0"
    "/home/oskar/Qt/6.6.1/gcc_64/lib/libQt6Core.so.6"
    "/home/oskar/Qt/6.6.1/gcc_64/lib/libQt6Core.so.6.6.1"
    "/lib/x86_64-linux-gnu/libopencv_videoio.so.4.5d"
    "/lib/x86_64-linux-gnu/libopencv_videoio.so.4.5.4d"
    "/lib/x86_64-linux-gnu/libopencv_calib3d.so.4.5d"
    "/lib/x86_64-linux-gnu/libopencv_calib3d.so.4.5.4d"
    "/lib/x86_64-linux-gnu/libopencv_dnn.so.4.5d"
    "/lib/x86_64-linux-gnu/libopencv_dnn.so.4.5.4d"
    "/lib/x86_64-linux-gnu/libopencv_imgproc.so.4.5d"
    "/lib/x86_64-linux-gnu/libopencv_imgproc.so.4.5.4d"
    "/lib/x86_64-linux-gnu/libopencv_core.so.4.5d"
    "/lib/x86_64-linux-gnu/libopencv_core.so.4.5.4d"
    "/lib/x86_64-linux-gnu/libstdc++.so.6"
    "/lib/x86_64-linux-gnu/libm.so.6"
    "/lib/x86_64-linux-gnu/libgcc_s.so.1"
    "/lib/x86_64-linux-gnu/libc.so.6"
    "/home/oskar/Qt/6.6.1/gcc_64/lib/libQt6OpenGLWidgets.so.6"
    "/home/oskar/Qt/6.6.1/gcc_64/lib/libQt6OpenGLWidgets.so.6.6.1"
    "/home/oskar/Qt/6.6.1/gcc_64/lib/libQt6OpenGL.so.6"
    "/home/oskar/Qt/6.6.1/gcc_64/lib/libQt6OpenGL.so.6.6.1"
    "/lib/x86_64-linux-gnu/libGL.so.1"
    "/lib/x86_64-linux-gnu/libpthread.so.0"
    "/lib/x86_64-linux-gnu/libgomp.so.1"
    "/lib/x86_64-linux-gnu/libxkbcommon.so.0"
    "/lib/x86_64-linux-gnu/libEGL.so.1"
    "/lib/x86_64-linux-gnu/libfontconfig.so.1"
    "/lib/x86_64-linux-gnu/libX11.so.6"
    "/lib/x86_64-linux-gnu/libdl.so.2"
    "/lib/x86_64-linux-gnu/libglib-2.0.so.0"
    "/home/oskar/Qt/6.6.1/gcc_64/lib/libQt6DBus.so.6"
    "/home/oskar/Qt/6.6.1/gcc_64/lib/libQt6DBus.so.6.6.1"
    "/lib/x86_64-linux-gnu/libz.so.1"
    "/lib/x86_64-linux-gnu/libfreetype.so.6"
    "/lib/x86_64-linux-gnu/libgthread-2.0.so.0"
    "/lib/x86_64-linux-gnu/libGLdispatch.so.0"
    "/home/oskar/Qt/6.6.1/gcc_64/lib/libicui18n.so.56"
    "/home/oskar/Qt/6.6.1/gcc_64/lib/libicui18n.so.56.1"
    "/home/oskar/Qt/6.6.1/gcc_64/lib/libicuuc.so.56"
    "/home/oskar/Qt/6.6.1/gcc_64/lib/libicudata.so.56"
    "/lib/x86_64-linux-gnu/librt.so.1"
    "/lib/x86_64-linux-gnu/libopencv_imgcodecs.so.4.5d"
    "/lib/x86_64-linux-gnu/libdc1394.so.25"
    "/lib/x86_64-linux-gnu/libgstbase-1.0.so.0"
    "/lib/x86_64-linux-gnu/libgstreamer-1.0.so.0"
    "/lib/x86_64-linux-gnu/libgobject-2.0.so.0"
    "/lib/x86_64-linux-gnu/libgstapp-1.0.so.0"
    "/lib/x86_64-linux-gnu/libgstriff-1.0.so.0"
    "/lib/x86_64-linux-gnu/libgstpbutils-1.0.so.0"
    "/lib/x86_64-linux-gnu/libgstvideo-1.0.so.0"
    "/lib/x86_64-linux-gnu/libavcodec.so.58"
    "/lib/x86_64-linux-gnu/libavformat.so.58"
    "/lib/x86_64-linux-gnu/libavutil.so.56"
    "/lib/x86_64-linux-gnu/libswscale.so.5"
    "/lib/x86_64-linux-gnu/libgphoto2.so.6"
    "/lib/x86_64-linux-gnu/libgphoto2_port.so.12"
    "/lib/x86_64-linux-gnu/libopencv_features2d.so.4.5d"
    "/lib/x86_64-linux-gnu/libopencv_flann.so.4.5d"
    "/lib/x86_64-linux-gnu/libprotobuf.so.23"
    "/lib/x86_64-linux-gnu/libtbb.so.2"
    "/lib/x86_64-linux-gnu/libexpat.so.1"
    "/lib/x86_64-linux-gnu/libuuid.so.1"
    "/lib/x86_64-linux-gnu/libxcb.so.1"
    "/lib/x86_64-linux-gnu/libpcre.so.3"
    "/lib/x86_64-linux-gnu/libdbus-1.so.3"
    "/lib/x86_64-linux-gnu/libpng16.so.16"
    "/lib/x86_64-linux-gnu/libbrotlidec.so.1"
    "/lib/x86_64-linux-gnu/libjpeg.so.8"
    "/lib/x86_64-linux-gnu/libwebp.so.7"
    "/lib/x86_64-linux-gnu/libgdcmMSFF.so.3.0"
    "/lib/x86_64-linux-gnu/libtiff.so.5"
    "/lib/x86_64-linux-gnu/libopenjp2.so.7"
    "/lib/x86_64-linux-gnu/libIlmImf-2_5.so.25"
    "/lib/libgdal.so.30"
    "/lib/x86_64-linux-gnu/libgdcmDSED.so.3.0"
    "/lib/x86_64-linux-gnu/libraw1394.so.11"
    "/lib/x86_64-linux-gnu/libusb-1.0.so.0"
    "/lib/x86_64-linux-gnu/libgmodule-2.0.so.0"
    "/lib/x86_64-linux-gnu/libunwind.so.8"
    "/lib/x86_64-linux-gnu/libdw.so.1"
    "/lib/x86_64-linux-gnu/libffi.so.8"
    "/lib/x86_64-linux-gnu/libgstaudio-1.0.so.0"
    "/lib/x86_64-linux-gnu/libgsttag-1.0.so.0"
    "/lib/x86_64-linux-gnu/liborc-0.4.so.0"
    "/lib/x86_64-linux-gnu/libswresample.so.3"
    "/lib/x86_64-linux-gnu/libvpx.so.7"
    "/lib/x86_64-linux-gnu/libwebpmux.so.3"
    "/lib/x86_64-linux-gnu/liblzma.so.5"
    "/lib/x86_64-linux-gnu/libaribb24.so.0"
    "/lib/x86_64-linux-gnu/libdav1d.so.5"
    "/lib/x86_64-linux-gnu/libopencore-amrwb.so.0"
    "/lib/x86_64-linux-gnu/librsvg-2.so.2"
    "/lib/x86_64-linux-gnu/libcairo.so.2"
    "/lib/x86_64-linux-gnu/libzvbi.so.0"
    "/lib/x86_64-linux-gnu/libsnappy.so.1"
    "/lib/x86_64-linux-gnu/libaom.so.3"
    "/lib/x86_64-linux-gnu/libcodec2.so.1.0"
    "/lib/x86_64-linux-gnu/libgsm.so.1"
    "/lib/x86_64-linux-gnu/libmp3lame.so.0"
    "/lib/x86_64-linux-gnu/libopencore-amrnb.so.0"
    "/lib/x86_64-linux-gnu/libopus.so.0"
    "/lib/x86_64-linux-gnu/libshine.so.3"
    "/lib/x86_64-linux-gnu/libspeex.so.1"
    "/lib/x86_64-linux-gnu/libtheoraenc.so.1"
    "/lib/x86_64-linux-gnu/libtheoradec.so.1"
    "/lib/x86_64-linux-gnu/libtwolame.so.0"
    "/lib/x86_64-linux-gnu/libvo-amrwbenc.so.0"
    "/lib/x86_64-linux-gnu/libvorbis.so.0"
    "/lib/x86_64-linux-gnu/libvorbisenc.so.2"
    "/lib/x86_64-linux-gnu/libx264.so.163"
    "/lib/x86_64-linux-gnu/libx265.so.199"
    "/lib/x86_64-linux-gnu/libxvidcore.so.4"
    "/lib/x86_64-linux-gnu/libva.so.2"
    "/lib/x86_64-linux-gnu/libmfx.so.1"
    "/lib/x86_64-linux-gnu/libxml2.so.2"
    "/lib/x86_64-linux-gnu/libbz2.so.1.0"
    "/lib/x86_64-linux-gnu/libgme.so.0"
    "/lib/x86_64-linux-gnu/libopenmpt.so.0"
    "/lib/x86_64-linux-gnu/libchromaprint.so.1"
    "/lib/x86_64-linux-gnu/libbluray.so.2"
    "/lib/x86_64-linux-gnu/libgnutls.so.30"
    "/lib/x86_64-linux-gnu/librabbitmq.so.4"
    "/lib/x86_64-linux-gnu/libsrt-gnutls.so.1.4"
    "/lib/x86_64-linux-gnu/libssh-gcrypt.so.4"
    "/lib/x86_64-linux-gnu/libzmq.so.5"
    "/lib/x86_64-linux-gnu/libva-drm.so.2"
    "/lib/x86_64-linux-gnu/libva-x11.so.2"
    "/lib/x86_64-linux-gnu/libvdpau.so.1"
    "/lib/x86_64-linux-gnu/libdrm.so.2"
    "/lib/x86_64-linux-gnu/libOpenCL.so.1"
    "/lib/x86_64-linux-gnu/libltdl.so.7"
    "/lib/x86_64-linux-gnu/libexif.so.12"
    "/lib/x86_64-linux-gnu/libXau.so.6"
    "/lib/x86_64-linux-gnu/libXdmcp.so.6"
    "/lib/x86_64-linux-gnu/libsystemd.so.0"
    "/lib/x86_64-linux-gnu/libbrotlicommon.so.1"
    "/lib/x86_64-linux-gnu/libgdcmDICT.so.3.0"
    "/lib/x86_64-linux-gnu/libgdcmjpeg8.so.3.0"
    "/lib/x86_64-linux-gnu/libgdcmjpeg12.so.3.0"
    "/lib/x86_64-linux-gnu/libgdcmjpeg16.so.3.0"
    "/lib/x86_64-linux-gnu/libcharls.so.2"
    "/lib/x86_64-linux-gnu/libjson-c.so.5"
    "/lib/x86_64-linux-gnu/libgdcmIOD.so.3.0"
    "/lib/x86_64-linux-gnu/libgdcmCommon.so.3.0"
    "/lib/x86_64-linux-gnu/libzstd.so.1"
    "/lib/x86_64-linux-gnu/libjbig.so.0"
    "/lib/x86_64-linux-gnu/libdeflate.so.0"
    "/lib/x86_64-linux-gnu/libImath-2_5.so.25"
    "/lib/x86_64-linux-gnu/libIlmThread-2_5.so.25"
    "/lib/x86_64-linux-gnu/libHalf-2_5.so.25"
    "/lib/x86_64-linux-gnu/libIex-2_5.so.25"
    "/lib/x86_64-linux-gnu/libheif.so.1"
    "/lib/libarmadillo.so.10"
    "/lib/x86_64-linux-gnu/libpoppler.so.118"
    "/lib/x86_64-linux-gnu/libfreexl.so.1"
    "/lib/x86_64-linux-gnu/libqhull_r.so.8.0"
    "/lib/x86_64-linux-gnu/libgeos_c.so.1"
    "/lib/x86_64-linux-gnu/libodbc.so.2"
    "/lib/x86_64-linux-gnu/libodbcinst.so.2"
    "/lib/x86_64-linux-gnu/libkmlbase.so.1"
    "/lib/x86_64-linux-gnu/libkmldom.so.1"
    "/lib/x86_64-linux-gnu/libkmlengine.so.1"
    "/lib/x86_64-linux-gnu/libxerces-c-3.2.so"
    "/lib/x86_64-linux-gnu/libnetcdf.so.19"
    "/lib/x86_64-linux-gnu/libhdf5_serial.so.103"
    "/lib/libmfhdfalt.so.0"
    "/lib/libdfalt.so.0"
    "/lib/libogdi.so.4.1"
    "/lib/x86_64-linux-gnu/libgif.so.7"
    "/lib/x86_64-linux-gnu/libgeotiff.so.5"
    "/lib/x86_64-linux-gnu/libcfitsio.so.9"
    "/lib/x86_64-linux-gnu/libpq.so.5"
    "/lib/x86_64-linux-gnu/liblz4.so.1"
    "/lib/x86_64-linux-gnu/libblosc.so.1"
    "/lib/x86_64-linux-gnu/libproj.so.22"
    "/lib/x86_64-linux-gnu/libsqlite3.so.0"
    "/lib/x86_64-linux-gnu/libspatialite.so.7"
    "/lib/x86_64-linux-gnu/libpcre2-8.so.0"
    "/lib/x86_64-linux-gnu/libcurl.so.4"
    "/lib/x86_64-linux-gnu/libfyba.so.0"
    "/lib/x86_64-linux-gnu/libmysqlclient.so.21"
    "/lib/x86_64-linux-gnu/libcrypto.so.3"
    "/lib/x86_64-linux-gnu/libudev.so.1"
    "/lib/x86_64-linux-gnu/libelf.so.1"
    "/lib/x86_64-linux-gnu/libsoxr.so.0"
    "/lib/x86_64-linux-gnu/libcairo-gobject.so.2"
    "/lib/x86_64-linux-gnu/libgdk_pixbuf-2.0.so.0"
    "/lib/x86_64-linux-gnu/libgio-2.0.so.0"
    "/lib/x86_64-linux-gnu/libpangocairo-1.0.so.0"
    "/lib/x86_64-linux-gnu/libpango-1.0.so.0"
    "/lib/x86_64-linux-gnu/libpixman-1.so.0"
    "/lib/x86_64-linux-gnu/libxcb-shm.so.0"
    "/lib/x86_64-linux-gnu/libxcb-render.so.0"
    "/lib/x86_64-linux-gnu/libXrender.so.1"
    "/lib/x86_64-linux-gnu/libXext.so.6"
    "/lib/x86_64-linux-gnu/libogg.so.0"
    "/lib/x86_64-linux-gnu/libnuma.so.1"
    "/lib/x86_64-linux-gnu/libicuuc.so.70"
    "/lib/x86_64-linux-gnu/libmpg123.so.0"
    "/lib/x86_64-linux-gnu/libvorbisfile.so.3"
    "/lib/x86_64-linux-gnu/libudfread.so.0"
    "/lib/x86_64-linux-gnu/libp11-kit.so.0"
    "/lib/x86_64-linux-gnu/libidn2.so.0"
    "/lib/x86_64-linux-gnu/libunistring.so.2"
    "/lib/x86_64-linux-gnu/libtasn1.so.6"
    "/lib/x86_64-linux-gnu/libnettle.so.8"
    "/lib/x86_64-linux-gnu/libhogweed.so.6"
    "/lib/x86_64-linux-gnu/libgmp.so.10"
    "/lib/x86_64-linux-gnu/libssl.so.3"
    "/lib/x86_64-linux-gnu/libgcrypt.so.20"
    "/lib/x86_64-linux-gnu/libgpg-error.so.0"
    "/lib/x86_64-linux-gnu/libgssapi_krb5.so.2"
    "/lib/x86_64-linux-gnu/libbsd.so.0"
    "/lib/x86_64-linux-gnu/libsodium.so.23"
    "/lib/x86_64-linux-gnu/libpgm-5.3.so.0"
    "/lib/x86_64-linux-gnu/libnorm.so.1"
    "/lib/x86_64-linux-gnu/libXfixes.so.3"
    "/lib/x86_64-linux-gnu/libcap.so.2"
    "/lib/x86_64-linux-gnu/libde265.so.0"
    "/lib/x86_64-linux-gnu/libblas.so.3"
    "/lib/x86_64-linux-gnu/liblapack.so.3"
    "/lib/x86_64-linux-gnu/libarpack.so.2"
    "/lib/x86_64-linux-gnu/libsuperlu.so.5"
    "/lib/x86_64-linux-gnu/liblcms2.so.2"
    "/lib/x86_64-linux-gnu/libnss3.so"
    "/lib/x86_64-linux-gnu/libsmime3.so"
    "/lib/x86_64-linux-gnu/libplc4.so"
    "/lib/x86_64-linux-gnu/libnspr4.so"
    "/lib/x86_64-linux-gnu/libgeos.so.3.10.2"
    "/lib/x86_64-linux-gnu/libminizip.so.1"
    "/lib/x86_64-linux-gnu/liburiparser.so.1"
    "/lib/x86_64-linux-gnu/libcurl-gnutls.so.4"
    "/lib/x86_64-linux-gnu/libhdf5_serial_hl.so.100"
    "/lib/x86_64-linux-gnu/libsz.so.2"
    "/lib/x86_64-linux-gnu/libtirpc.so.3"
    "/lib/x86_64-linux-gnu/libldap-2.5.so.0"
    "/lib/x86_64-linux-gnu/librttopo.so.1"
    "/lib/x86_64-linux-gnu/libnghttp2.so.14"
    "/lib/x86_64-linux-gnu/librtmp.so.1"
    "/lib/x86_64-linux-gnu/libssh.so.4"
    "/lib/x86_64-linux-gnu/libpsl.so.5"
    "/lib/x86_64-linux-gnu/liblber-2.5.so.0"
    "/lib/x86_64-linux-gnu/libfyut.so.0"
    "/lib/x86_64-linux-gnu/libfygm.so.0"
    "/lib/x86_64-linux-gnu/libresolv.so.2"
    "/lib/x86_64-linux-gnu/libmount.so.1"
    "/lib/x86_64-linux-gnu/libselinux.so.1"
    "/lib/x86_64-linux-gnu/libpangoft2-1.0.so.0"
    "/lib/x86_64-linux-gnu/libharfbuzz.so.0"
    "/lib/x86_64-linux-gnu/libfribidi.so.0"
    "/lib/x86_64-linux-gnu/libthai.so.0"
    "/lib/x86_64-linux-gnu/libicudata.so.70"
    "/lib/x86_64-linux-gnu/libkrb5.so.3"
    "/lib/x86_64-linux-gnu/libk5crypto.so.3"
    "/lib/x86_64-linux-gnu/libcom_err.so.2"
    "/lib/x86_64-linux-gnu/libkrb5support.so.0"
    "/lib/x86_64-linux-gnu/libmd.so.0"
    "/lib/x86_64-linux-gnu/libatlas.so.3"
    "/lib/x86_64-linux-gnu/libgfortran.so.5"
    "/lib/x86_64-linux-gnu/libnssutil3.so"
    "/lib/x86_64-linux-gnu/libplds4.so"
    "/lib/x86_64-linux-gnu/libaec.so.0"
    "/lib/x86_64-linux-gnu/libsasl2.so.2"
    "/lib/x86_64-linux-gnu/libblkid.so.1"
    "/lib/x86_64-linux-gnu/libgraphite2.so.3"
    "/lib/x86_64-linux-gnu/libdatrie.so.1"
    "/lib/x86_64-linux-gnu/libkeyutils.so.1"
    "/lib/x86_64-linux-gnu/libquadmath.so.0"
)


set(TS_FILES IndoorPositioningSystem_en_GB.ts)

set(PROJECT_SOURCES
    main.cpp
    indoorpositioningsystemui.cpp
    indoorpositioningsystemui.h
    indoorpositioningsystemui.ui
    ${TS_FILES}
    videoprocessor.h videoprocessor.cpp
    threadsafequeue.h threadsafequeue.cpp
    dataprocessor.h dataprocessor.cpp
    structures.h
    dataanalysiswindow.h dataanalysiswindow.cpp dataanalysiswindow.ui
    customchartview.h customchartview.cpp
    uwblocalizationwindow.h uwblocalizationwindow.cpp uwblocalizationwindow.ui
    customgraphicitems.h customgraphicitems.cpp
    humandetector.h humandetector.cpp
    exporttimerangesetter.h exporttimerangesetter.cpp exporttimerangesetter.ui
    json.h
    customlabel.h customlabel.cpp
    indoorpositioningsystemviewmodel.h indoorpositioningsystemviewmodel.cpp
    coordinateswindow.h coordinateswindow.cpp coordinateswindow.ui
    anchorinputwindow.h anchorinputwindow.cpp anchorinputwindow.ui
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(IndoorPositioningSystem
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
    qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
else()
    if(ANDROID)
        add_library(IndoorPositioningSystem SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(IndoorPositioningSystem
            ${PROJECT_SOURCES}
        )
    endif()
    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif()

target_link_libraries(IndoorPositioningSystem PRIVATE
    Qt${QT_VERSION_MAJOR}::DBus
    Qt${QT_VERSION_MAJOR}::Gui
    Qt${QT_VERSION_MAJOR}::OpenGLWidgets
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Multimedia
    Qt${QT_VERSION_MAJOR}::Charts
    Qt${QT_VERSION_MAJOR}::MultimediaWidgets
    ${OpenCV_LIBS}
    ${MANUAL_LIB_PATHS}
)

# set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -no-pie")

# Ensure RPATH is set so the application can find Qt libraries at runtime
set_target_properties(IndoorPositioningSystem PROPERTIES
    INSTALL_RPATH_USE_LINK_PATH TRUE
    BUILD_WITH_INSTALL_RPATH TRUE
    INSTALL_RPATH "\$ORIGIN/../lib"
)

if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.IndoorPositioningSystem)
endif()
set_target_properties(IndoorPositioningSystem PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)

install(TARGETS IndoorPositioningSystem
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)


if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(IndoorPositioningSystem)
endif()

## CPack
# Get link libraries of the target
get_target_property(link_libraries IndoorPositioningSystem LINK_LIBRARIES)

# Initialize the variable to store library paths
set(LIB_LOCATIONS "")

# Iterate over each link entry
foreach (lib IN LISTS link_libraries)
    # Check if the entry is a target and a library
    if (TARGET ${lib})
        # Obtain the full path to the library file
        get_target_property(lib_location ${lib} LOCATION)

        # Append the path to the LIB_LOCATIONS list
        if (lib_location)
            list(APPEND LIB_LOCATIONS ${lib_location})
            message(STATUS "Added ${lib_location} to LIB_LOCATIONS")
        else ()
            message(WARNING "Location for ${lib} not found.")
        endif ()
    else ()
        # Manually handle non-target libraries
        foreach (manual_lib IN LISTS MANUAL_LIB_PATHS)
            if (${lib} STREQUAL ${manual_lib})
                list(APPEND LIB_LOCATIONS ${manual_lib})
                message(STATUS "Added ${manual_lib} to LIB_LOCATIONS manually")
            endif ()
        endforeach ()
    endif ()
endforeach ()

# Output the collected library locations
message(STATUS "LIB_LOCATIONS: ${LIB_LOCATIONS}")

set(CPACK_PACKAGE_NAME "IndoorPositioningSystem")
set(CPACK_PACKAGE_VENDOR "Your Name")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Indoor Positioning System - A system for indoor positioning using various techniques")
set(CPACK_PACKAGE_VERSION "${PROJECT_VERSION}")
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "Your Company")
set(CPACK_PACKAGE_CONTACT "Your Name <your.email@example.com>")
SET(CPACK_OUTPUT_FILE_PREFIX packages)

# Create symbolic links for required libraries
# execute_process(COMMAND ${CMAKE_COMMAND} -E create_symlink
#     "${CMAKE_INSTALL_LIBDIR}/libQt6Charts.so.6.6.1"
#     "${CMAKE_INSTALL_LIBDIR}/libQt6Charts.so.6"
# )
# execute_process(COMMAND ${CMAKE_COMMAND} -E create_symlink
#     "${CMAKE_INSTALL_LIBDIR}/libQt6Widgets.so.6.6.1"
#     "${CMAKE_INSTALL_LIBDIR}/libQt6Widgets.so.6"
# )
# execute_process(COMMAND ${CMAKE_COMMAND} -E create_symlink
#     "${CMAKE_INSTALL_LIBDIR}/libQt6Gui.so.6.6.1"
#     "${CMAKE_INSTALL_LIBDIR}/libQt6Gui.so.6"
# )
# execute_process(COMMAND ${CMAKE_COMMAND} -E create_symlink
#     "${CMAKE_INSTALL_LIBDIR}/libQt6Core.so.6.6.1"
#     "${CMAKE_INSTALL_LIBDIR}/libQt6Core.so.6"
# )
# execute_process(COMMAND ${CMAKE_COMMAND} -E create_symlink
#     "${CMAKE_INSTALL_LIBDIR}/libopencv_videoio.so.4.5.4d"
#     "${CMAKE_INSTALL_LIBDIR}/libopencv_videoio.so.4.5d"
# )
# execute_process(COMMAND ${CMAKE_COMMAND} -E create_symlink
#     "${CMAKE_INSTALL_LIBDIR}/libopencv_calib3d.so.4.5.4d"
#     "${CMAKE_INSTALL_LIBDIR}/libopencv_calib3d.so.4.5d"
# )
# execute_process(COMMAND ${CMAKE_COMMAND} -E create_symlink
#     "${CMAKE_INSTALL_LIBDIR}/libopencv_dnn.so.4.5.4d"
#     "${CMAKE_INSTALL_LIBDIR}/libopencv_dnn.so.4.5d"
# )
# execute_process(COMMAND ${CMAKE_COMMAND} -E create_symlink
#     "${CMAKE_INSTALL_LIBDIR}/libopencv_imgproc.so.4.5.4d"
#     "${CMAKE_INSTALL_LIBDIR}/libopencv_imgproc.so.4.5"
# )
# execute_process(COMMAND ${CMAKE_COMMAND} -E create_symlink
#     "${CMAKE_INSTALL_LIBDIR}/libopencv_core.so.4.5.4d"
#     "${CMAKE_INSTALL_LIBDIR}/libopencv_core.so.4.5d"
# )
# execute_process(COMMAND ${CMAKE_COMMAND} -E create_symlink
#     "${CMAKE_INSTALL_LIBDIR}/libQt6OpenGLWidgets.so.6.6.1"
#     "${CMAKE_INSTALL_LIBDIR}/libQt6OpenGLWidgets.so.6"
# )
# execute_process(COMMAND ${CMAKE_COMMAND} -E create_symlink
#     "${CMAKE_INSTALL_LIBDIR}/libQt6OpenGL.so.6.6.1"
#     "${CMAKE_INSTALL_LIBDIR}/libQt6OpenGL.so.6"
# )
# execute_process(COMMAND ${CMAKE_COMMAND} -E create_symlink
#     "${CMAKE_INSTALL_LIBDIR}/libQt6DBus.so.6.6.1"
#     "${CMAKE_INSTALL_LIBDIR}/libQt6DBus.so.6"
# )
# execute_process(COMMAND ${CMAKE_COMMAND} -E create_symlink
#     "${CMAKE_INSTALL_LIBDIR}/libicui18n.so.56.1"
#     "${CMAKE_INSTALL_LIBDIR}/libicui18n.so.56"
# )

if (UNIX)
    set(CPACK_GENERATOR "TGZ;DEB")
elseif (WIN32)
    set(CPACK_GENERATOR "ZIP;NSIS")
endif ()
set(CPACK_COMPONENTS_ALL Runtime)

include(CPack)
include(InstallRequiredSystemLibraries)
install(TARGETS IndoorPositioningSystem
        BUNDLE DESTINATION .
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# Install Qt libraries
install(
        FILES
        ${LIB_LOCATIONS}
        DESTINATION
        ${CMAKE_INSTALL_LIBDIR}
        COMPONENT Runtime
)

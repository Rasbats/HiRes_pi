##---------------------------------------------------------------------------
## Author:      Rasbats aka. Mike Rossiter
## Copyright:
## License:     wxWidgets License
##---------------------------------------------------------------------------

# define minimum cmake version
CMAKE_MINIMUM_REQUIRED(VERSION 2.6.2)

PROJECT(HiRes_pi)

SET(PACKAGE_NAME HiRes_pi)
SET(VERBOSE_NAME HiRes)
SET(TITLE_NAME HiRes)
SET(CPACK_PACKAGE_CONTACT "Mike Rossiter")

SET(VERSION_MAJOR "0")
SET(VERSION_MINOR "1")

#SET(CMAKE_BUILD_TYPE Debug)
SET( wxWidgets_USE_DEBUG OFF)
SET( wxWidgets_USE_UNICODE ON)
SET( wxWidgets_USE_UNIVERSAL OFF)
SET( wxWidgets_USE_STATIC OFF)

INCLUDE("cmake/PluginConfigure.cmake")

SET(SRC_HiRes
        src/HiRes_pi.h
        src/HiRes_pi.cpp
        src/icons.h
        src/icons.cpp
	    src/HiResgui.h
	    src/HiResgui.cpp
	    src/HiResgui_impl.cpp
	    src/HiResgui_impl.h
		src/ocpn_plugin.h
	)


ADD_LIBRARY(${PACKAGE_NAME} SHARED ${SRC_HiRes})

ADD_DEFINITIONS(-DTIXML_USE_STL)

TARGET_LINK_LIBRARIES( ${PACKAGE_NAME} ${wxWidgets_LIBRARIES} )


INCLUDE("cmake/PluginInstall.cmake")
INCLUDE("cmake/PluginLocalization.cmake")
INCLUDE("cmake/PluginPackage.cmake")

#1. Set platform folder.

if(WIN32)
    set(PLATFORM_FOLDER win32)
elseif(APPLE)
    set(PLATFORM_FOLDER ios)
else()
    set(PLATFORM_FOLDER android)
endif()

SET(CMAKE_CXX_FLAGS "-std=c++0x")

#2. Set Define Marco
if (WIN32)
else()
endif()

if(WIN32)
  ADD_DEFINITIONS (-DWIN32 -D_WINDOWS)  
  if(MSVC)
    SET(CMAKE_CXX_FLAGS_DEBUG -D_DEBUG " /MDd /Od /ZI")
    SET(CMAKE_CXX_FLAGS_RELEASE -DNDEBUG " /MD /O2")
  	ADD_DEFINITIONS (-D_CRT_SECURE_NO_WARNINGS -D_CRT_NON_CONFORMING_SWPRINTFS -D_SCL_SECURE_NO_WARNINGS -wd4100 -wd4127)
  else(MSVC)  
  endif(MSVC)
elseif(APPLE)
elseif(ANDROID)
endif()

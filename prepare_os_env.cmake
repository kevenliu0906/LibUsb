# Determine current OS platform
if(UNIX)
    set(SUBARCH "Linux")
    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
        SET(ARCH "Linux64")
    else(CMAKE_SIZEOF_VOID_P EQUAL 8)
        SET(ARCH "Linux32")
    endif(CMAKE_SIZEOF_VOID_P EQUAL 8)
endif(UNIX)

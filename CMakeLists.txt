cmake_minimum_required (VERSION 3.2...4.0)
project(libOPNChipSet VERSION 1.6.2 LANGUAGES C CXX)

include(GNUInstallDirs)
include(CheckCCompilerFlag)
include(CheckCXXCompilerFlag)

# Prefer C90 standard
set(CMAKE_C_STANDARD 90)
# Prefer C++98 standard
set(CMAKE_CXX_STANDARD 98)

if(MSVC)
    check_cxx_compiler_flag("/std:c++14" COMPILER_SUPPORTS_CXX14)
    if(COMPILER_SUPPORTS_CXX14)
        set(FLAG_CPP14 "/std:c++14")
    endif()
    if(MSVC_VERSION LESS 1910)
        unset(COMPILER_SUPPORTS_CXX14) # MSVC older than 2017 fails to build YMFM
    endif()
else()
    check_cxx_compiler_flag("-std=c++14" COMPILER_SUPPORTS_CXX14)
    if(COMPILER_SUPPORTS_CXX14)
        set(FLAG_CPP14 "-std=c++14")
    endif()
endif()

function(CheckCPP14Build OUTVAR)
    set(CMAKE_CXX_STANDARD 14)
    try_compile(${OUTVAR}
        ${CMAKE_CURRENT_BINARY_DIR}/cmake-checks
        ${CMAKE_CURRENT_SOURCE_DIR}/cmake/checks/cpp14.cpp
        CMAKE_FLAGS -DCMAKE_CXX_FLAGS=${FLAG_CPP14}
    )
endfunction()

if(COMPILER_SUPPORTS_CXX14)
    CheckCPP14Build(COMPILER_BUILDS_CXX14)
    if(COMPILER_BUILDS_CXX14)
        message("== Your C++ compiler supports C++14, YMFM emulator will be ENABLED")
    else()
        message("== Your C++ compiler can't build C++14, YMFM emulator will be DISABLED")
        set(COMPILER_SUPPORTS_CXX14 FALSE)
    endif()
else()
    message("== Your C++ compiler does NOT supports C++14, YMFM emulator will be DISABLED")
endif()

if(NOT CMAKE_RUNTIME_OUTPUT_DIRECTORY)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
endif()
if(NOT CMAKE_LIBRARY_OUTPUT_DIRECTORY)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
endif()
if(NOT CMAKE_ARCHIVE_OUTPUT_DIRECTORY)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
endif()


#===========================================================================================
# Strip garbage
if(APPLE)
    set(LINK_FLAGS_RELEASE  "${LINK_FLAGS_RELEASE} -dead_strip")
elseif(NOT MSVC AND NOT MSDOS AND NOT OPENBSD_LOCALBASE)
    string(REGEX REPLACE "-O3" ""
            CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE}")
    string(REGEX REPLACE "-O3" ""
            CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}")
    set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -O2 -fdata-sections -ffunction-sections")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2 -fdata-sections -ffunction-sections")
    string(REGEX REPLACE "-O3" ""
           CMAKE_C_FLAGS_RELWITHDEBINFO "${CMAKE_C_FLAGS_RELWITHDEBINFO}")
    string(REGEX REPLACE "-O3" ""
           CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO}")
    set(CMAKE_C_FLAGS_RELWITHDEBINFO "${CMAKE_C_FLAGS_RELWITHDEBINFO} -O2 -fdata-sections -ffunction-sections")
    set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} -O2 -fdata-sections -ffunction-sections")
    if(ANDROID)
        set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -funwind-tables")
        set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -funwind-tables")
    endif()
    if(NOT "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
        set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -s -Wl,--gc-sections -Wl,-s")
        set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -s -Wl,--gc-sections -Wl,-s")
        set(LINK_FLAGS_RELEASE  "${LINK_FLAGS_RELEASE} -Wl,--gc-sections -Wl,-s")
    endif()
endif()

# Global optimization flags
if(NOT MSVC AND NOT MSDOS)
# Global optimization flags
    set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -fno-omit-frame-pointer")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -fno-omit-frame-pointer")
    set(CMAKE_C_FLAGS_RELWITHDEBINFO "${CMAKE_C_FLAGS_RELWITHDEBINFO} -fno-omit-frame-pointer")
    set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} -fno-omit-frame-pointer")
# Turn on all warnings
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
# Deny undefined symbols
    if(NOT APPLE)
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--no-undefined" )
        set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,--no-undefined" )
        set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} -Wl,--no-undefined" )
    endif()
# Supress the std::vector::insert() GCC change warning
    if(VITA)
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DVITA -DVITA=1 -fcompare-debug-second")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DVITA -DVITA=1 -fcompare-debug-second")
    endif()
    if(NINTENDO_3DS OR NINTENDO_WII OR NINTENDO_WIIU OR NINTENDO_SWITCH)
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fcompare-debug-second")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fcompare-debug-second")
    endif()
endif()

if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX OR CMAKE_C_COMPILER_ID STREQUAL "Clang")
    if(NINTENDO_3DS OR NINTENDO_WII OR NINTENDO_WIIU OR NINTENDO_SWITCH)
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu90")
    else()
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c89")
    endif()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++98")
endif()

if(DEFINED CMAKE_BUILD_TYPE)
    string(TOLOWER "${CMAKE_BUILD_TYPE}" CMAKE_BUILD_TYPE_LOWER)
endif()
if(CMAKE_BUILD_TYPE_LOWER EQUAL "release")
    add_definitions(-DNDEBUG)
ENDIF()

# Disable bogus MSVC warnings
if(MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
ENDIF()

if(OPENBSD_LOCALBASE)
    message("OpenBSD Detected!")
    include_directories(/usr/local/include)
endif()

if(WIN32)
    add_definitions(-DUNICODE -D_UNICODE)
endif()


function(set_visibility_hidden destTarget)
    if(CMAKE_C_COMPILER_ID MATCHES "^(GNU|Clang)$" AND NOT DJGPP)
        if(${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION} VERSION_GREATER 3.2)
            target_compile_options(${destTarget} PRIVATE
              $<$<COMPILE_LANGUAGE:CXX>:-fvisibility-inlines-hidden> -fvisibility=hidden)
        else()
            target_compile_options(${destTarget} PRIVATE -fvisibility=hidden)
        endif()
        target_compile_definitions(${destTarget} PRIVATE -DOPNCHIPSET_VISIBILITY)
    endif()
endfunction()


include(chipset.cmake)


set(OPN_CHIPSET_SRC ${CHIPS_SOURCES} opn2_chipset_api.cpp)

# FIXME: Implement static and normal shared libraries to make this thing can be
# used as a third-party library

add_library(opnemuchipset MODULE ${OPN_CHIPSET_SRC})
target_include_directories(opnemuchipset PRIVATE ${CMAKE_CURRENT_LIST_DIR})
target_include_directories(opnemuchipset PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)
target_compile_definitions(opnemuchipset PRIVATE -DOPNCHIPSET_BUILD -DOPNCHIPSET_BUILD_DLL)

if(COMPILER_SUPPORTS_CXX14)
    target_compile_definitions(opnemuchipset PRIVATE -DOPNCHIPSET_ENABLE_YMFM)
endif()

set_visibility_hidden(opnemuchipset)


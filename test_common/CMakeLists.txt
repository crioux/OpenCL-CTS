
set(HARNESS_SOURCES
    harness/threadTesting.cpp
    harness/typeWrappers.cpp
    harness/mt19937.cpp
    harness/conversions.cpp
    harness/rounding_mode.cpp
    harness/msvc9.c
    harness/crc32.c
    harness/errorHelpers.cpp
    harness/genericThread.cpp
    harness/imageHelpers.cpp
    harness/kernelHelpers.cpp
    harness/deviceInfo.cpp
    harness/os_helpers.cpp
    harness/parseParameters.cpp
    harness/testHarness.cpp
    harness/ThreadPool.cpp
    miniz/miniz.c
# ---
    harness/dllio.cpp
# ---
)

add_library(harness STATIC ${HARNESS_SOURCES})

# ---
FOREACH(source ${HARNESS_SOURCES})
	IF(NOT ${source} MATCHES ".*dllio.cpp$")
		IF(MSVC)
			SET_PROPERTY(SOURCE ${source} APPEND_STRING PROPERTY COMPILE_FLAGS " /FI\"dllio.h\" ")
		ELSE()
			SET_PROPERTY(SOURCE ${source} APPEND_STRING PROPERTY COMPILE_FLAGS " -include \"dllio.h\" ")
		ENDIF()
	ENDIF()
ENDFOREACH()
# ---
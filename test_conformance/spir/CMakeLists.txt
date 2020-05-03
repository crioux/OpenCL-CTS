# Import function list from math_brute_force
add_definitions(-DFUNCTION_LIST_ULPS_ONLY)

set(SPIR_OUT ${CONFORMANCE_PREFIX}spir${CONFORMANCE_SUFFIX})

set (SPIR_SOURCES
    main.cpp
    datagen.cpp
    run_build_test.cpp
    run_services.cpp
    kernelargs.cpp
    ../math_brute_force/FunctionList.cpp
)

# add_executable(${SPIR_OUT}
#    ${SPIR_SOURCES})

# ---
add_library(${SPIR_OUT} SHARED ${SPIR_SOURCES})
set_property(TARGET ${SPIR_OUT} PROPERTY WINDOWS_EXPORT_ALL_SYMBOLS YES)
LIST(APPEND CONFORMANCE_TEST_TARGETS ${SPIR_OUT})
set(CONFORMANCE_TEST_TARGETS ${CONFORMANCE_TEST_TARGETS} PARENT_SCOPE)
set_property(TARGET ${SPIR_OUT} PROPERTY FOLDER "CONFORMANCE${CONFORMANCE_SUFFIX}")
# ---


if(UNIX)
    set_target_properties(${SPIR_OUT} PROPERTIES
       COMPILE_FLAGS "-fexceptions -frtti")
elseif(MSVC)
    set_target_properties(${SPIR_OUT} PROPERTIES
       COMPILE_FLAGS "/GR /EHs /EHc")
endif()

TARGET_LINK_LIBRARIES(${SPIR_OUT} harness
    ${CLConform_LIBRARIES})


set_source_files_properties(${SPIR_SOURCES} PROPERTIES LANGUAGE CXX)

# Need to copy the spir zips to sit beside the executable

set(SPIR_FILES
    "khr.csv"
    "api.zip"
    "atomics.zip"
    "basic.zip"
    "compile_and_link.zip"
    "commonfns.zip"
    "conversions.zip"
    "geometrics.zip"
    "enum_values.zip"
    "half.zip"
    "kernel_attributes.zip"
    "kernel_image_methods.zip"
    "images_kernel_read_write.zip"
    "images_samplerlessRead.zip"
    "integer_ops.zip"
    "math_brute_force.zip"
    "printf.zip"
    "profiling.zip"
    "relationals.zip"
    "select.zip"
    "sampler_enumeration.zip"
    "vec_align.zip"
    "vec_step.zip"
    "binary_type.zip")

foreach(input_file ${SPIR_FILES})
    add_custom_command(
    TARGET ${SPIR_OUT}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_SOURCE_DIR}/${input_file}" "$<TARGET_FILE_DIR:${SPIR_OUT}>/"
)
endforeach(input_file)

# ---
FOREACH(source ${SPIR_FILES})
    IF(MSVC)
        SET_PROPERTY(SOURCE ${source} APPEND_STRING PROPERTY COMPILE_FLAGS " /FI\"dllio.h\" ")
    ELSE()
        SET_PROPERTY(SOURCE ${source} APPEND_STRING PROPERTY COMPILE_FLAGS " -include \"dllio.h\" ")
    ENDIF()
ENDFOREACH()
# ---
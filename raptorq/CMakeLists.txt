cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

set(PYRQ_SOURCE
  python_raptorq.cpp
)

add_library(pyraptorq SHARED ${PYRQ_SOURCE})

target_include_directories(pyraptorq PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>/..
)

#if (APPLE)
#    set_target_properties(pyraptorq PROPERTIES LINK_FLAGS "-Wl,-exported_symbols_list,${CMAKE_CURRENT_SOURCE_DIR}/raptorq_export_list")
#endif()

target_link_libraries(pyraptorq PUBLIC tdutils)
target_link_libraries(pyraptorq PUBLIC tdfec)


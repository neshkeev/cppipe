file(GLOB SOURCE_FILES *.cpp)

foreach(SOURCE_PATH ${SOURCE_FILES})
  get_filename_component(EXECUTABLE_NAME ${SOURCE_PATH} NAME_WE)

  add_executable(${EXECUTABLE_NAME})
  target_sources(${EXECUTABLE_NAME} PRIVATE ${SOURCE_PATH})

  target_include_directories(${EXECUTABLE_NAME}
      PRIVATE $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>/include
      $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>)

  add_test(NAME ${EXECUTABLE_NAME} COMMAND ${EXECUTABLE_NAME})
endforeach()

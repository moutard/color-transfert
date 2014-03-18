add_executable(
  tests
  ${TRANSFERT_INC}
  ${TRANSFERT_SRC}
  tests.cpp
  test_main.cpp)

target_link_libraries(tests ${OpenCV_LIBS})


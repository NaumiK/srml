cmake_minimum_required(VERSION 3.29.0)
project(SRML VERSION 0.0)
find_package(FLEX REQUIRED)
flex_target(scanner
  numlex.l
  ${CMAKE_CURRENT_BINARY_DIR}/lexer.cc
)

set(SRCS 
  src/main.cc
  src/n_term.cc
  src/psdnf.cc
  src/save_set.cc
)

add_executable(${PROJECT_NAME} ${SRCS} ${FLEX_scanner_OUTPUTS})
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(${PROJECT_NAME} PRIVATE "C:/Users/NaumiK/scoop/apps/winflexbison/2.5.25")
target_compile_definitions(${PROJECT_NAME} PRIVATE YY_NO_UNISTD_H)
cmake_minimum_required(VERSION 3.0.0)
project(MontyPlython)

# Configuração do C++
set(CMAKE_BUILD_TYPE Release)
# set(CMAKE_CXX_STANDARD 17)

set(LIB_DIR lib)
set(LIB_FILES ${LIB_DIR}/rply-1.1.4/rply.c)

set(SRC_DIR src)
set(SOURCE_FILES ${SRC_DIR}/montyplython.cpp ${SRC_DIR}/glut_ply.cpp)

# Configuração do OpenGL e GLUT
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS} )

include_directories(${CMAKE_CURRENT_BINARY_DIR})

include_directories(${LIB_DIR})
include_directories(${SRC_DIR})


# Executável
add_executable(${PROJECT_NAME} ${LIB_FILES} ${SOURCE_FILES})


target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} )
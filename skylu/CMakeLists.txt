set(CMAKE_CXX_FLAGS "$ENV{CXXFLAGS} -rdynamic -O0 -g -std=c++11 -Wall -Wno-deprecated -Werror -Wno-unused-function")   #设置 编译选项  set  设置变量的意思
set(LIB_SRC 
   log.cc
   env.cc
   config.cc
   mutex.cc
   thread.cc
   util.cc
   )
   

add_library(skylu SHARED ${LIB_SRC}) 
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/../lib)

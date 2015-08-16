cmake_policy (SET CMP0015 NEW)

add_project (LedServer
             BIN
             main.cpp
             ledserver.cpp
             ledserver.h
             ledserver.ui
             ThreadPool.h
             ThreadPool.cpp
			 net.h
             net.cpp)

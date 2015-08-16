cmake_policy (SET CMP0015 NEW)

add_project (LedClient
             BIN
             main.cpp
             ledclient.cpp
             ledclient.h
			 net.cpp
             net.h
			 widgets.cpp
             widgets.h
             ledclient.ui)

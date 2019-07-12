
HEADERS += \
	parser_p.h \ # generated with qlalr
	Command.hpp \
	CommandParser.hpp # generated with qlalr

SOURCES += \
	main.cpp \
	parser.cpp \ # generated with qlalr
	CommandParser.cpp # generated with qlalr

OTHER_FILES += \
    plover.g

CONFIG += qt console c++17 strict_c++

QT -= gui

win32-msvc:QMAKE_CXXFLAGS += "/Zc:__cplusplus" "/std:c++17"


set(target hamtest1)
set(sources 
		main.cpp
		serial.h
		serial.cpp
		ic7300.h
		ic7300.cpp
		audiocapture.h
		audiocapture.cpp
	)

add_executable(${target} ${sources})
target_link_libraries(${target} portaudio_x64)

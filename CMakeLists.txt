cmake_minimum_required(VERSION 3.20)
project(Boundforge LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

# GLFW
FetchContent_Declare(
	glfw
	URL
	https://github.com/glfw/glfw/archive/refs/heads/master.zip
)
FetchContent_MakeAvailable(glfw)

# GLAD
FetchContent_Declare(
	glad
	URL
	https://github.com/Dav1ddRe/glad/archive/refs/heads/master.zip
)
FetchContent_MakeAvailable(glad)

# GLM
FetchContent_Declare(
	glm
	URL
	https://github.com/g-truc/glm/archive/refs/heads/master.zip
)
FetchContent_MakeAvailable(glm)

# Gather Files
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.cpp)

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME}
	PRIVATE
	include
)

target_link_libraries(${PROJECT_NAME}
	PRIVATE
	glfw
	glad
	glm
)

# Cross-platform linking
if(MINGW)
	target_link_libaries(${PROJECT_NAME}
		PRIVATE
		opengl32
		gdi32
	)
elseif(UNIX AND NOT APPLE)
	find_package(OpenGL REQUIRED)
	target_link_libraries(${PROJECT_NAME}
		PRIVATE
		${CMAKE_DL_LIBS}
		OpenGL::GL
		pthread
	)
endif()

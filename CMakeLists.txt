cmake_minimum_required(VERSION 3.24...3.50)
project(match3)

include(FetchContent)

# Generate compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Dependencies
set(RAYLIB_VERSION 5.0)

FetchContent_Declare(
    raylib
    DOWNLOAD_EXTRACT_TIMESTAMP OFF
    URL https://github.com/raysan5/raylib/archive/refs/tags/${RAYLIB_VERSION}.tar.gz
    FIND_PACKAGE_ARGS
)

FetchContent_MakeAvailable(raylib)

# Our Project
add_executable(${PROJECT_NAME})
add_subdirectory(src)

set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${PROJECT_NAME})

set_property(TARGET ${PROJECT_NAME} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY $<TARGET_FILE_DIR:${PROJECT_NAME}>)

if ("${PLATFORM}" STREQUAL "Web")
    add_custom_command(
        TARGET ${PROJECT_NAME} PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/src/resources $<TARGET_FILE_DIR:${PROJECT_NAME}>/../resources
    )
else()
    add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/src/resources $<TARGET_FILE_DIR:${PROJECT_NAME}>/resources
    )
endif()

target_link_libraries(${PROJECT_NAME} raylib)

if ("${PLATFORM}" STREQUAL "Web")
    set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX ".html")
    target_link_options(${PROJECT_NAME} PUBLIC -sUSE_GLFW=3 PUBLIC --preload-file resources)
endif()

if (APPLE)
    target_link_libraries(${PROJECT_NAME} "-framework IOKit")
    target_link_libraries(${PROJECT_NAME} "-framework Cocoa")
    target_link_libraries(${PROJECT_NAME} "-framework OpenGL")
endif()

# --- CPack Configuration ---
# IMPORTANT: Define CPACK variables BEFORE including CPack!

set(CPACK_PACKAGE_NAME "match3")
set(CPACK_PACKAGE_VENDOR "Your Name/Company")
set(CPACK_PACKAGE_VERSION_MAJOR "1")
set(CPACK_PACKAGE_VERSION_MINOR "0")
set(CPACK_PACKAGE_VERSION_PATCH "0")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "A simple match-3 game made with raylib.")
set(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/README.md")
set(CPACK_PACKAGE_HOMEPAGE "https://yourwebsite.com/match3")
set(CPACK_PACKAGE_CONTACT "your.email@example.com") # This is also good for general CPack info

# Explicitly set the Debian maintainer field (resolves the error)
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Your Name <your.email@example.com>")

set(CPACK_PACKAGE_DEBIAN_SECTION "games")

# Ensure these dependencies are explicitly set here (resolves the warning)
set(CPACK_DEBIAN_PACKAGE_DEPENDS
    "libx11-6, libgl1, libasound2, libpulse0, libwayland-client0, libwayland-cursor0, libwayland-egl1, libxkbcommon0"
)

set(CPACK_GENERATOR "DEB")
set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)

# Include CPack *after* all CPACK_ variables are defined
include(CPack)

# --- Installation directives for CPack ---
install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/src/resources/
    DESTINATION share/${PROJECT_NAME}/resources
    FILE_PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ WORLD_READ
    PATTERN "*.png" PATTERN "*.otf" PATTERN "*.ttf"
)

set(ICON_NAME "match3-icon")
install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/${ICON_NAME}.png"
    DESTINATION share/icons/hicolor/scalable/apps
)

install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}.desktop"
    DESTINATION share/applications
)
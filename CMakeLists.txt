cmake_minimum_required(VERSION 3.13)

# If do't define version number, specify the version number
set(VERSION "1.0.0" CACHE STRING "define project version")

# project 有两种写法，这里建议使用这种写法，补全信息有助于后续使用
project(dbus-test
    LANGUAGES C CXX
    VERSION ${VERSION}
)

add_subdirectory(qtdbus)
add_subdirectory(sdbus)
add_subdirectory(memfd)

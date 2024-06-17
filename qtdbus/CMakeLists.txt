# 定义GNU标准安装目录，使用此定义可兼容不同发行版之间的安装目录差异
include(GNUInstallDirs)
# 设置C标准
set(CMAKE_CXX_STANDARD 17)
# 设置为检查C标准打开，若未设置CMAKE_C_STANDARD，则会报错
set(CMAKE_CXX_STANDARD_REQUIRED on)
# 生成编译命令，用于支持clangd lsp
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# 若使用标准C++开发，则不需要设置，使用Qt则需要
set(CMAKE_AUTOMOC ON)

# 若未指定安装前缀，则指定前缀为/usr
# 注意大部分发行版默认会将前缀设置为/usr/local，
# 所以若需要安装到/usr，还需在编译时指定 -DCMAKE_INSTALL_PREFIX=/usr
if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX /usr)
endif()

# 若未指定编译类型，则指定为Release，防止某些发行版不指定编译类型
if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose Debug or Release" FORCE)
endif()

# 若目标库安装了.cmake文件，则可以直接使用find_package
find_package(QT NAMES Qt6 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core DBus)
# 若目标库未安装.cmake文件，但是安装了.pc文件，则可以使用pkgconfig
# pkg_check_modules(Systemd REQUIRED IMPORTED_TARGET libsystemd)

# 建议对不同的模块进行分类，可使用通配符，可指定具名文件
file(GLOB_RECURSE SRCS
    "src/*.h"
    "src/*.cpp"
)

set(BIN_NAME qtdbus)

# 生成可执行文件
add_executable(${BIN_NAME}
    ${SRCS}
)

# 这里针对当前二进制定义了一个宏，保存着版本号，方便程序代码中调用，若程序中需要自定义宏变量并且在代码中使用，可参考此方法。
# 注意CMAKE_PROJECT_XXX系列变量，某些属性是cmake帮助提供，但某些属性是project方法设置的，所以这里需要注意变量是否可用。
# target_compile_definitions(${BIN_NAME} PRIVATE
#     VERSION="${CMAKE_PROJECT_VERSION}"
# )

target_include_directories(${BIN_NAME} PUBLIC
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::DBus
    # PkgConfig::Systemd
)

target_link_libraries(${BIN_NAME} PRIVATE
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::DBus
    # PkgConfig::Systemd
)

# 指定安装目录，一般常用有3个：
# TARGETS（编译出来的目标二进制）
# FILES（指定路径的文件，通常是配置文件或服务文件）
# DIRECTORY（一般是配置文件较多时使用）。
install(TARGETS ${BIN_NAME} DESTINATION ${CMAKE_INSTALL_BINDIR})


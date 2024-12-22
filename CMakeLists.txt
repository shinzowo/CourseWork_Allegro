# Минимальная версия CMake
cmake_minimum_required(VERSION 3.10)

# Имя проекта
project(Game)

# Установите флаг для C++ стандарта
set(CMAKE_CXX_STANDARD 11)

# Укажите путь к заголовочным файлам Allegro
include_directories("allegro/include")

# Укажите путь к библиотекам Allegro
link_directories("allegro/lib")

# Добавьте ваш исходный файл
add_executable(Game main.cpp)

# Линкуем Allegro (возможно, нужно добавить другие библиотеки)
target_link_libraries(Game
    PRIVATE
    allegro
    allegro_primitives
    allegro_font
    allegro_ttf
    allegro_image
    allegro_audio
    allegro_acodec
    kernel32
    user32
    gdi32
    winspool
    shell32
    ole32
    oleaut32
    uuid
    comdlg32
    advapi32
)
# Опции компилятора для поддержки концептов в GCC
target_compile_options(Game PRIVATE -fconcepts)

# Устанавливаем путь для исполняемых файлов при установке пакета
install(TARGETS Game DESTINATION bin)

# Включаем CPack для создания пакетов (DEB и ZIP)
set(CPACK_GENERATOR "DEB;ZIP")

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT example@example.com)
include(CPack)


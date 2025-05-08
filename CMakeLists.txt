cmake_minimum_required(VERSION 3.10)
project(SistemaDifuso)

# Configuración estándar
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Directorios de inclusión
include_directories(include)

# Fuentes del proyecto
set(SOURCES
    src/ConjuntoDifuso.cpp
    src/Variable.cpp
    src/ValorReal.cpp
    src/SistemaDifuso.cpp
    main.cpp
)

file(COPY 
    ${CMAKE_SOURCE_DIR}/descripcion-variables.txt
    ${CMAKE_SOURCE_DIR}/base-de-conocimiento.txt
    DESTINATION ${CMAKE_BINARY_DIR}
)

# Ejecutable principal
add_executable(entrada ${SOURCES})


# Si necesitas enlazar con otras bibliotecas
# target_link_libraries(entrada m)  # Por ejemplo para math.h
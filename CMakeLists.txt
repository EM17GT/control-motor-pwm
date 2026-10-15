cmake_minimum_required(VERSION 3.16)

project(ControlMotor C)

# Agregamos todos los archivos fuente que conforman tu proyecto
add_executable(ControlMotor
    main.c
    conf.c
)

# Vinculamos la librería lgpio en lugar de gpiod
target_link_libraries(ControlMotor
    lgpio
)
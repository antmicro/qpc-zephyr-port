cmake_minimum_required(VERSION 3.13.1)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})

project(app LANGUAGES C VERSION 1.0.0)

target_include_directories(app PRIVATE ${CMAKE_BINARY_DIR}/app/include src)

target_include_directories(app PRIVATE $ENV{QPC_PATH}/include)
target_include_directories(app PRIVATE $ENV{QPC_PATH}/src)
target_include_directories(app PRIVATE ../../../port)

target_sources(app PRIVATE src/main.c)
target_sources(app PRIVATE src/philo.c)
target_sources(app PRIVATE src/table.c)
target_sources(app PRIVATE src/bsp.c)

target_sources(app PRIVATE ../../../port/qf_port.c)

target_sources(app PRIVATE $ENV{QPC_PATH}/src/qf/qf_time.c)
target_sources(app PRIVATE $ENV{QPC_PATH}/src/qf/qf_act.c)
target_sources(app PRIVATE $ENV{QPC_PATH}/src/qf/qf_actq.c)
target_sources(app PRIVATE $ENV{QPC_PATH}/src/qf/qf_qact.c)
target_sources(app PRIVATE $ENV{QPC_PATH}/src/qf/qf_dyn.c)
target_sources(app PRIVATE $ENV{QPC_PATH}/src/qf/qf_mem.c)
target_sources(app PRIVATE $ENV{QPC_PATH}/src/qf/qf_qeq.c)
target_sources(app PRIVATE $ENV{QPC_PATH}/src/qf/qf_ps.c)
target_sources(app PRIVATE $ENV{QPC_PATH}/src/qf/qep_hsm.c)

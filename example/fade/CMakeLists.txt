cmake_minimum_required(VERSION 3.1.0)
project(fade LANGUAGES C)

set(LIBHIFIVE_DIR "" CACHE PATH "Path to libhifive root directory")

set(CMAKE_ASM_FLAGS "-march=rv32imac -mabi=ilp32 -mcmodel=medlow")
set(CMAKE_C_FLAGS "-march=rv32imac -mabi=ilp32 -mcmodel=medlow -ffunction-sections -fdata-sections")
set(CMAKE_CXX_FLAGS "-march=rv32imac -mabi=ilp32 -mcmodel=medlow -ffunction-sections -fdata-sections")
set(CMAKE_EXE_LINKER_FLAGS "-Wl,--gc-sections -nostartfiles -nostdlib -Wl,-Map,${PROJECT_NAME}.map -T${LIBHIFIVE_DIR}/hifive.default.lds -L${LIBHIFIVE_DIR}/build")

set(CMAKE_ASM_FLAGS "${CMAKE_ASM_FLAGS} -Wall")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

include_directories(${LIBHIFIVE_DIR}/include)

set(ELF_TARGET ${PROJECT_NAME}.elf)
add_executable(${ELF_TARGET} ${PROJECT_NAME}.c)
target_link_libraries(${ELF_TARGET} "-Wl,--start-group -lhifive -lc -lgcc -lm -Wl,--end-group")

set(HEX_TARGET ${PROJECT_NAME}.hex)
add_custom_target(${HEX_TARGET} ALL
    COMMAND ${RISCV_OBJCOPY} -O ihex ${ELF_TARGET} ${HEX_TARGET}
    COMMAND ${RISCV_SIZE} ${ELF_TARGET}
    DEPENDS ${ELF_TARGET}
    BYPRODUCTS ${HEX_TARGET}
    VERBATIM
)

add_custom_target(upload
    COMMAND hifive upload ${HEX_TARGET}
    DEPENDS ${HEX_TARGET}
    VERBATIM
)

add_custom_target(debug
    COMMAND hifive debug ${ELF_TARGET}
    DEPENDS ${ELF_TARGET}
    VERBATIM
)

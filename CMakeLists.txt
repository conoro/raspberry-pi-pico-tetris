add_executable(
  tetris
  picoris.cpp
)

# Pull in pico libraries that we need
target_link_libraries(tetris pico_stdlib hardware_spi hardware_pwm hardware_dma pico_display)

# create map/bin/hex file etc.
pico_add_extra_outputs(tetris)
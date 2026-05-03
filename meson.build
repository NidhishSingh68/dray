project('lcd_driver',
        'cpp',
        'c'
        )

ScreenFillsrc = files('./examples/Rectangle.cpp',
            './src/gpio.cpp',
            './src/timer.cpp',
            './src/lcd.cpp',
            './src/syscalls.c',
            './src/startup.s'
          )

Conwaysrc = files('./examples/ConwaysGameOfLife.cpp',
            './src/gpio.cpp',
            './src/timer.cpp',
            './src/lcd.cpp',
            './src/syscalls.c',
            './src/startup.s'
          )

add_project_arguments(
  '-mthumb',
  '-mcpu=cortex-m4',
  '-mfloat-abi=hard',
  '-mfpu=fpv4-sp-d16',
  '-ggdb',
  language: ['c','cpp']
)

add_project_link_arguments(
  '-mcpu=cortex-m4',
  '-mfloat-abi=hard',
  '--specs=nosys.specs',
  '--specs=nano.specs',
  '-T../STM32F446RETX_FLASH.ld',
  language: ['c','cpp']
)

stm32f4_args = [
      '-ffunction-sections',
      '-fdata-sections'
]

inc = include_directories('./include')

libopencm3_proj = subproject('libopencm3')
libopencm3_core = libopencm3_proj.get_variable('libopencm3_stm32f4')
libopencm3_common = libopencm3_proj.get_variable('stm32f4_cm3')

ScreenFillfirmware = executable(
  'Rectangle',
  ScreenFillsrc,
  cpp_args: stm32f4_args,
  dependencies: [ libopencm3_core,libopencm3_common ],
  include_directories : inc
)

Conwayfirmware = executable(
  'Conway',
  Conwaysrc,
  cpp_args: stm32f4_args,
  dependencies: [ libopencm3_core,libopencm3_common ],
  include_directories : inc
)

ScreenFillfirmware_bin = custom_target(
  'ScreenFill' + '_bin',
  input: ScreenFillfirmware,
  output: 'ScreenFill' + '.bin',
  command: [
    'arm-none-eabi-objcopy',
    '-O',
    'binary',
    '@INPUT@',
    '@OUTPUT@',
  ],
  build_by_default: true,
)

Conwayfirmware_bin = custom_target(
    'ConwayGameOfLife' + '_bin',
  input: Conwayfirmware,
  output:   'ConwayGameOfLife' + '.bin',
  command: [
    'arm-none-eabi-objcopy',
    '-O',
    'binary',
    '@INPUT@',
    '@OUTPUT@',
  ],
  build_by_default: true,
)

run_target(
  'flash_R',
  command: [
    'st-flash',
    '--reset',
    'write',
    ScreenFillfirmware_bin.full_path(),
    '0x08000000',
  ],
  depends: ScreenFillfirmware_bin,
)

run_target(
  'flash_C',
  command: [
    'st-flash',
    '--reset',
    'write',
    Conwayfirmware_bin.full_path(),
    '0x08000000',
  ],
  depends: Conwayfirmware_bin,
)

run_target(
  'debug',
  command: [
    'openocd',
    '-f',
    'interface/stlink.cfg',
    '-f',
    'target/stm32f4x.cfg',
  ],
)

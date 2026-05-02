project('lcd_driver',
        'cpp',
        'c'
        )

src = files('./src/main.cpp',
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

firmware = executable(
  meson.project_name(),
  src,
  cpp_args: stm32f4_args,
  dependencies: [ libopencm3_core,libopencm3_common ],
  include_directories : inc

)

firmware_bin = custom_target(
  meson.project_name() + '_bin',
  input: firmware,
  output: meson.project_name() + '.bin',
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
  'flash',
  command: [
    'st-flash',
    '--reset',
    'write',
    firmware_bin.full_path(),
    '0x08000000',
  ],
  depends: firmware_bin,
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

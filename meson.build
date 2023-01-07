project('riscv64', 'c')



executable(
    'kernel.elf',
    'main.c',
    'start.S',
    c_args: [
        '-Wall',
        '-Wextra',
        '-c',
        '-mcmodel=medany',
        '-ffreestanding',
    ],
    link_args: [
        '-T',
        meson.source_root() / 'link.ld',
        '-nostdlib',
    ],
)

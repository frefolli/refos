project('efi', 'c',
  version : '0.1',
  default_options : ['warning_level=3'])

cc = meson.get_compiler('c')

gnu_efi_crto='/usr/lib/crt0-efi-x86_64.o'
gnu_efi_lds='/usr/lib/elf_x86_64_efi.lds'
libefi = cc.find_library('efi', required: true)
libgnuefi = cc.find_library('gnuefi', required: true)

c_flags = [
  '-fpic',
  '-shared',
  '-ffreestanding',
  '-fno-stack-protector',
  '-fno-stack-check',
  '-fshort-wchar',
  '-mno-red-zone',
  '-maccumulate-outgoing-args'
]
ld_flags = [
  '-Wl,-Bsymbolic',
  '-Wl,-T' + gnu_efi_lds,
  '-Wl,' + gnu_efi_crto
]
objcopy_flags = [
  '-j', '.text', 
  '-j', '.sdata', 
  '-j', '.data', 
  '-j', '.rodata', 
  '-j', '.dynamic', 
  '-j', '.dynsym', 
  '-j', '.rel', 
  '-j', '.rela', 
  '-j', '.rel.*', 
  '-j', '.rela.*', 
  '-j', '.reloc', 
  '--target', 'efi-app-x86_64', 
  '--subsystem=10'
]

librefos = shared_library('refos', [
    'src/main.c'
  ],
  c_args: c_flags,
  link_args: ld_flags,
  dependencies: [libefi, libgnuefi]
)
refos_efi = custom_target('refos_efi',
  output: 'REFOS.EFI',
  input: librefos,
  depends: [librefos],
  build_by_default: true,
  command: ['objcopy'] + objcopy_flags + [librefos, 'REFOS.EFI']
)
refos_iso = custom_target('refos.iso',
  output: 'refos.iso',
  input: refos_efi,
  build_by_default: true,
  command: [meson.current_source_dir() / 'scripts/build_iso.sh']
)


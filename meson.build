project('muteki-shims', 'c',
    version : '3.1.0',
    default_options : ['warning_level=3'])

as_flags = ['-mcpu=arm7tdmi']
ld_flags = [as_flags, '-nostdlib', '-nostdinc']

syscall_files_sdk = ['syscall_def/syscalls_sdk.json', 'syscall_def/syscalls_sdk_hpprime.json']
syscall_files_krnl = ['syscall_def/syscalls_krnl.json', 'syscall_def/syscalls_krnl_hpprime.json']

# For gen_syscall_shim.py.
python_interp = import('python').find_installation('python3')

syscalls_krnl = custom_target(
    'syscalls_krnl.s',
    output : 'syscalls_krnl.s',
    input : ['syscall_def/gen_syscall_shim.py', syscall_files_krnl],
    command: [python_interp, '@INPUT0@', '-o', '@OUTPUT@', '@INPUT1@', '@INPUT2@'],
)

syscalls_sdk = custom_target('syscalls_sdk.s',
    output : 'syscalls_sdk.s',
    input : ['syscall_def/gen_syscall_shim.py', syscall_files_sdk],
    command: [python_interp, '@INPUT0@', '-o', '@OUTPUT@', '@INPUT1@', '@INPUT2@'],)

_syscall_table = run_command([python_interp, 'syscall_def/gen_syscall_shim.py',
    '-Sd',
    'syscall_def/syscalls_sdk.json', 'syscall_def/syscalls_sdk_hpprime.json',
    'syscall_def/syscalls_krnl.json', 'syscall_def/syscalls_krnl_hpprime.json',],
    check : true)

_syscall_split_files = _syscall_table.stdout().strip().split('\n')

syscalls_split = custom_target('syscalls_split',
    output : _syscall_split_files,
    input : ['syscall_def/gen_syscall_shim.py', syscall_files_sdk, syscall_files_krnl],
    command: [python_interp, '@INPUT0@', '-t', 'gas-eabi', '-So', '@BUILD_ROOT@', '@INPUT1@', '@INPUT2@', '@INPUT3@', '@INPUT4@'],)

# Static library that bundles both sdk and krnl shims.
static_library(
    'mutekishims',
    syscalls_split,
    install : true,
    c_args : as_flags,
    link_args : ld_flags,
)

if get_option('generate_dll_replicas')

# Need this for "bare metal" system or "none" OS since it will use UNIX/ELF-style naming otherwise.
lib_suffix_override = 'dll'
lib_prefix_override = ''

# sdklib replica
shared_library(
    'sdklib',
    'src/dll_entry_stub.s',
    syscalls_sdk,
    install : true,
    c_args : as_flags,
    link_args : ld_flags,
    name_suffix : lib_suffix_override,
    name_prefix : lib_prefix_override,
)

# krnllib replica
shared_library(
    'krnllib',
    'src/dll_entry_stub.s',
    syscalls_krnl,
    install : true,
    c_args : as_flags,
    link_args : ld_flags,
    name_suffix : lib_suffix_override,
    name_prefix : lib_prefix_override,
)

endif # get_option('generate_dll_replicas')

# Headers for the shims
install_subdir('include/muteki/',
    install_dir : 'include/')

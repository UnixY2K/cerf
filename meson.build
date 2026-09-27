project(
    'cerf',
    'cpp',
    default_options: ['cpp_std=c++23'],
    version: '1.0.0',
    license: 'BSD-3',
)

nlohmannjson_dep = dependency(
                'nlohmann_json_multiple_headers',
                fallback: ['nlohmann_json', 'nlohmann_json_dep']
)

slirp_dep = disabler()
if host_machine.system() != 'darwin'
    slirp_dep = dependency('slirp', required: false,
                           fallback: ['slirp', 'slirp_dep'])
endif

if host_machine.system() != 'windows'
    subdir('libwin32coded')
endif
subdir('cerf')

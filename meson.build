project(
    'unit-system',
    'cpp',
    default_options : [
        'cpp_std=c++17',
        'warning_level=3'
    ],
    version : '0.8.0',
    meson_version: '>= 1.1.0',
)

use_cpp_17 = false
if get_option('use_cpp_17').enabled()
    use_cpp_17 = true
elif get_option('use_cpp_17').disabled()
    use_cpp_17 = false
else
    cpp_20_supported_versions = [
        'c++20', 'c++23', 'c++26', 'c++2a',
        'gnu++20', 'gnu++23', 'gnu++26', 'gnu++2a',
        'vc++20', 'vc++23', 'vc++26', 'vc++2a',
    ]
    use_cpp_17 = get_option('cpp_std' ) not in cpp_20_supported_versions
endif


#set windows linker argument
cpp = meson.get_compiler('cpp')

if target_machine.system() == 'windows'
    if get_option('buildtype') != 'static'
        add_project_arguments('-DUNIT_SYSTEM_EXPORT_MACRO=__declspec(dllexport)', language: 'cpp')
    else
        add_project_arguments('-DUNIT_SYSTEM_EXPORT_MACRO=__declspec(dllimport)', language: 'cpp')
    endif
endif

incdirs = include_directories('include', '.')

if use_cpp_17
    sources = [
        
        'src/time_si.cpp',
        
        'src/length.cpp',
        
        'src/mass.cpp',
        
        'src/temperature.cpp',
        
        'src/amount.cpp',
        
        'src/electric_current.cpp',
        
        'src/luminous_intensity.cpp',
        
        'src/energy.cpp',
        
        'src/power.cpp',
        
        'src/speed.cpp',
        
        'src/acceleration.cpp',
        
        'src/area.cpp',
        
        'src/force.cpp',
        
        'src/momentum.cpp',
        
    ]

    unit_system = library(
        'unit-system', 
        sources,
        version : meson.project_version(), 
        soversion : '0',
        include_directories : incdirs,
        install : true,
    )

    unit_system_dep = declare_dependency(
        include_directories : incdirs,
        link_with : unit_system,
        version: meson.project_version(),
    )
    
    install_headers('include/unit_system_17.hpp', subdir : 'unit_system')

    pkg = import('pkgconfig')
    pkg.generate(unit_system, subdirs: 'unit_system')
else
    unit_system_dep = declare_dependency(
        include_directories : incdirs,
        version: meson.project_version(),
    )
    
    install_headers('include/unit_system_20.hpp', subdir : 'unit_system')
endif

conf_data = configuration_data()
conf_data.set('USE_CPP_17', use_cpp_17)
configure_file(
    input : 'include/unit_system.hpp.in',
    output : 'unit_system.hpp',
    configuration : conf_data,
    install: true,
    install_dir: 'unit_system',
)

meson.override_dependency('unit-system', unit_system_dep)

build_tests = get_option('build_tests').enable_auto_if(not meson.is_subproject())
if build_tests.enabled()
    subdir('tests')
endif
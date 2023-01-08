project(
    'gol',
    'cpp',
    default_options : ['warning_level=3', 'werror=true', 'optimization=3', 'cpp_std=c++20']
)


####################################################################################################
# Variables tracking sources and libraries
####################################################################################################
# .cpp files (This might be used later for example clang-format)
project_sources = []
# Directories where compiler is able to include from
project_include_directories = []
project_include_directories += include_directories('include')


# Currently unused:
# .hpp files (This might be used later for example clang-format)
project_header_files = [] 


####################################################################################################
# Subprojects (using meson wraps)
####################################################################################################
catch2 = subproject('catch2')
fmt = subproject('fmt')
cxxopts = subproject('cxxopts')
gsl = subproject('microsoft-gsl')
####################################################################################################
# Dependencies
####################################################################################################
project_dependencies = []

project_dependencies += dependency('threads')
project_dependencies += dependency('catch2-with-main')
project_dependencies += dependency('fmt')
project_dependencies += dependency('microsoft_gsl')


# Conan
conan_pkgs= {
    # Names must match the package that cmake looks for
    #'fmt': 'fmt/9.0.0@',  # <- Must contain @, otherwise Conan will think it is a path
    
    # ... the dependency list goes on
}
# Adding new dependencies to this dict is error-free, but if you 
# change an existing entry, e.g. the pakage version, you probably need
# `meson configure --clearcache` to let new setting takes effect.

conan_program = find_program(['conan'], required: true, native: true)

foreach pkg_name, conan_ref : conan_pkgs
    module_path = meson.current_build_dir() / 'conan-cmake' / pkg_name
    run_command(conan_program, 'install', conan_ref, '-if', module_path,
        '-g', 'cmake_find_package', check: true)
    project_dependencies += dependency(pkg_name, method: 'cmake', cmake_module_path: module_path)
endforeach


####################################################################################################
# Subdirectors 
####################################################################################################
subdir('include')
subdir('unit_tests')



# Command to generate release build dir
'''
CC=gcc-11 CXX=g++-11 meson setup build --buildtype=release
'''

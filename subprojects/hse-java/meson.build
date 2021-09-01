project(
    'hse-java',
    ['c', 'java'],
    version: '2.1.0', # if updated, update pom.xml
    license: 'Apache-2.0',
    default_options: [
        'c_std=gnu11',
        'buildtype=debugoptimized',
        'warning_level=2',
    ],
    meson_version: '>=0.59.0'
)

version_components = meson.project_version().split('.')

hse_java_major_version = version_components[0]
hse_java_minor_version = version_components[1]
hse_java_patch_version = version_components[2]

fs = import('fs')

cc = meson.get_compiler('c')
javac = meson.get_compiler('java')
mvn = find_program('mvn')

hse_dep = dependency('hse-2', version: '>=2.0.0')
jdk_dep = dependency('jdk', version: '>=1.8.0')

subdir('lib')
subdir('src')

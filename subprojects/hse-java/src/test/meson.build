wrapper = [
    find_program('python3'),
    find_program('test-runner'),
    '--'
]

add_test_setup(
    'default',
    exe_wrapper: wrapper,
    is_default: not meson.is_subproject()
)

add_test_setup(
    'ci',
    exe_wrapper: wrapper
)

subdir('java')

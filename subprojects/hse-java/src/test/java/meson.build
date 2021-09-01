tests = [
	'KvdbTest',
	'KvdbTransactionTest',
	'KvsCursorTest',
	'KvsTest',
]

foreach t : tests
	test(
		t,
		mvn,
		args: [
			'-f',
			meson.project_source_root() / 'pom.xml',
			'test',
			'-Dtest=com.micron.hse.@0@'.format(t),
			'-Dmeson.build.dir=@0@/subprojects/@1@/lib'.format(meson.global_build_root(), meson.project_name()),
			# '-Djava.library.path=@0@/lib'.format(fs.parent(fs.parent(meson.project_build_root()))),
		],
		is_parallel: false,
		suite: ['unit'],
		depends: [
			hse_java,
		],
		env: environment({
			'HSE_TEST_RUNNER_RUNNER': 'java',
		})
	)
endforeach

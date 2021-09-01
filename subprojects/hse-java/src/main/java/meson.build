java_sources = files(
	'com/micron/hse/Hse.java',
	'com/micron/hse/HseException.java',
	'com/micron/hse/Kvdb.java',
	'com/micron/hse/KvdbTransaction.java',
	'com/micron/hse/Kvs.java',
	'com/micron/hse/KvsCursor.java',
	'com/micron/hse/Limits.java',
	'com/micron/hse/Version.java',
)

class_path = run_command(
	'sh',
	'-c',
	'mvn dependency:build-classpath -DincludeScope=compile -Dmdep.outputFile=/dev/stdout'
		+ ' 2>/dev/null -q -f @0@/pom.xml'.format(meson.project_source_root()),
	check: true,
).stdout().strip()

hse_jar = jar(
	'hse-@0@'.format(meson.project_version()),
	java_sources,
	java_args: [
		'--class-path=@0@'.format(class_path),
	],
	install: true,
	install_dir: get_option('prefix') / get_option('datadir') / 'java',
)

hse_java_sources = files(
	'hse_jni.c',
	'hse_kvdb_jni.c',
	'hse_kvdb_txn_jni.c',
	'hse_kvs_jni.c',
	'hse_kvs_cursor_jni.c',
	'hse_version_jni.c'
)

hse_java = shared_module(
	'hse-jni-2',
	hse_java_sources,
	include_directories: include_directories('.'),
	dependencies: [
		hse_dep,
		jdk_dep,
	],
)

/* SPDX-License-Identifier: Apache-2.0 */
/*
 * Copyright (C) 2021 Micron Technology, Inc.  All rights reserved.
 */

package com.micron.hse;

import java.nio.file.Path;
import java.util.Arrays;
import java.util.Collections;
import java.util.List;

public final class Kvdb {
	private static native void jni_hse_kvdb_create(String kvdb_home, String[] params) throws HseException;
	private static native long jni_hse_kvdb_open(String kvdb_home, String[] params) throws HseException;
	private static native void jni_hse_kvdb_close(long kvdb_addr) throws HseException;
	private static native void jni_hse_kvdb_drop(String kvdb_home) throws HseException;
	private static native String[] jni_hse_kvdb_kvs_names_get(long kvdb_addr);
	private static native void jni_hse_kvdb_kvs_create(long kvdb_addr, String kvs_name, String[] params) throws HseException;
	private static native long jni_hse_kvdb_kvs_open(long kvdb_addr, String kvs_name, String[] params) throws HseException;
	private static native void jni_hse_kvdb_kvs_drop(long kvdb_addr, String kvs_name) throws HseException;
	private static native long jni_hse_kvdb_txn_alloc(long kvdb_addr);
	private static native void jni_hse_kvdb_sync(long kvdb_addr, int flags) throws HseException;
	private static native void jni_hse_kvdb_flush(long kvdb_addr) throws HseException;
	private static native void jni_hse_kvdb_compact(long kvdb_addr, int flags) throws HseException;
	private static native CompactStatus jni_hse_kvdb_compact_status_get(long kvdb_addr) throws HseException;
	private static native MclassInfo jni_hse_kvdb_mclass_info_get(long kvdb_addr, int mclass) throws HseException;

	long c_hse_kvdb = 0;

	private Kvdb() {}

	public static void create(final Path kvdbHome, final String... params) {
		jni_hse_kvdb_create(kvdbHome.toString(), params);
	}

	public static void create(final String kvdbHome, final String... params) {
		jni_hse_kvdb_create(kvdbHome, params);
	}

	public static Kvdb open(final Path kvdbHome, final String... params) {
		Kvdb k = new Kvdb();

		k.c_hse_kvdb = jni_hse_kvdb_open(kvdbHome.toString(), params);

		return k;
	}

	public static Kvdb open(final String kvdbHome, final String... params) {
		Kvdb k = new Kvdb();

		k.c_hse_kvdb = jni_hse_kvdb_open(kvdbHome, params);

		return k;
	}

	public synchronized void close() {
		if (c_hse_kvdb != 0) {
			jni_hse_kvdb_close(c_hse_kvdb);
			c_hse_kvdb = 0;
		}
	}

	public static void drop(final Path kvdbHome) {
		jni_hse_kvdb_drop(kvdbHome.toString());
	}

	public static void drop(final String kvdbHome) {
		jni_hse_kvdb_drop(kvdbHome);
	}

	public List<String> kvsNames() {
		return Collections.unmodifiableList(Arrays.asList(jni_hse_kvdb_kvs_names_get(c_hse_kvdb)));
	}

	public void kvsCreate(final String kvsName, final String... params) {
		jni_hse_kvdb_kvs_create(c_hse_kvdb, kvsName, params);
	}

	public Kvs kvsOpen(final String kvsName, final String... params) {
		Kvs kvs = new Kvs();

		kvs.c_hse_kvs = jni_hse_kvdb_kvs_open(c_hse_kvdb, kvsName, params);

		return kvs;
	}

	public void kvsDrop(final String kvsName) {
		jni_hse_kvdb_kvs_drop(c_hse_kvdb, kvsName);
	}

	public void sync() {
		jni_hse_kvdb_sync(c_hse_kvdb, 0);
	}

	public MclassInfo getMclassInfo(final Mclass mclass) {
		return jni_hse_kvdb_mclass_info_get(c_hse_kvdb, mclass.value);
	}

	public CompactStatus getCompactStatus() {
		return jni_hse_kvdb_compact_status_get(c_hse_kvdb);
	}

	public KvdbTransaction transaction() {
		KvdbTransaction txn = new KvdbTransaction(this);

		txn.c_hse_kvdb_txn = jni_hse_kvdb_txn_alloc(c_hse_kvdb);

		return txn;
	}

	public static final class CompactStatus {
		private int sampLwm;
		private int sampHwm;
		private int sampCurr;
		private boolean active;
		private boolean canceled;

		private CompactStatus() {}

		public int getSampLwm() {
			return sampLwm;
		}

		public int getSampHwm() {
			return sampHwm;
		}

		public int getSampCurr() {
			return sampCurr;
		}

		public boolean isActive() {
			return active;
		}

		public boolean wasCanceled() {
			return canceled;
		}
	}

	static {
		System.loadLibrary("hse-jni-2");
	}
}

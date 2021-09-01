/* SPDX-License-Identifier: Apache-2.0 */
/*
 * Copyright (C) 2021 Micron Technology, Inc.  All rights reserved.
 */

package com.micron.hse;

import java.util.EnumSet;
import java.util.Optional;

import com.micron.hse.KvsCursor.CreateFlags;

public final class Kvs {
	long c_hse_kvs = 0;

	private static native void jni_hse_kvdb_kvs_close(long kvs_addr) throws HseException;
	private static native void jni_hse_kvs_put(long kvs_addr, byte[] key, byte[] value, int flags, long txn_addr) throws HseException;
	private static native byte[] jni_hse_kvs_get(long kvs_addr, byte[] key, byte[] buf, int flags, long txn_addr) throws HseException;
	private static native void jni_hse_kvs_delete(long kvs_addr, byte[] key, int flags, long txn_addr) throws HseException;
	private static native void jni_hse_kvs_prefix_delete(long kvs_addr, byte[] filt, int flags, long txn_addr) throws HseException;
	private static native int jni_hse_kvs_put_prio();
	private static native int jni_hse_kvs_put_vcomp_off();

	Kvs() {}

	private void putImpl(final byte[] key, final byte[] value, final KvdbTransaction txn, final EnumSet<PutFlags> flags) {
		jni_hse_kvs_put(this.c_hse_kvs, key, value, flags != null ? PutFlags.getValue(flags) : 0, txn != null ? txn.c_hse_kvdb_txn : 0);
	}

	private Optional<byte[]> getImpl(final byte[] key, byte[] buf, final KvdbTransaction txn) {
		return Optional.ofNullable(jni_hse_kvs_get(this.c_hse_kvs, key, buf, 0, txn != null ? txn.c_hse_kvdb_txn : 0));
	}

	private void deleteImpl(final byte[] key, final KvdbTransaction txn) {
		jni_hse_kvs_delete(this.c_hse_kvs, key, 0, txn != null ? txn.c_hse_kvdb_txn : 0);
	}

	private void prefixDeleteImpl(final byte[] filt, final KvdbTransaction txn) {
		jni_hse_kvs_prefix_delete(this.c_hse_kvs, filt, 0, txn.c_hse_kvdb_txn);
	}

	public void put(final byte[] key) {
		putImpl(key, null, null, null);
	}

	public void put(final byte[] key, final byte[] value) {
		putImpl(key, value, null, null);
	}

	public void put(final byte[] key, final KvdbTransaction txn) {
		putImpl(key, null, txn, null);
	}

	public void put(final byte[] key, final EnumSet<PutFlags> flags) {
		putImpl(key, null, null, flags);
	}

	public void put(final byte[] key, final byte[] value, final KvdbTransaction txn) {
		putImpl(key, value, txn, null);
	}

	public void put(final byte[] key, final byte[] value, final EnumSet<PutFlags> flags) {
		putImpl(key, value, null, flags);
	}

	public void put(final byte[] key, final KvdbTransaction txn, final EnumSet<PutFlags> flags) {
		putImpl(key, null, txn, flags);
	}

	public void put(final byte[] key, final byte[] value, final KvdbTransaction txn, final EnumSet<PutFlags> flags) {
		putImpl(key, value, txn, flags);
	}

	public Optional<byte[]> get(final byte[] key) {
		return getImpl(key, null, null);
	}

	public Optional<byte[]> get(final byte[] key, final KvdbTransaction txn) {
		return getImpl(key, null, txn);
	}

	public Optional<byte[]> get(final byte[] key, byte[] buf) {
		return getImpl(key, buf, null);
	}

	public Optional<byte[]> get(final byte[] key, byte[] buf, final KvdbTransaction txn) {
		return getImpl(key, buf, txn);
	}

	public void delete(final byte[] key) {
		deleteImpl(key, null);
	}

	public void delete(final byte[] key, final KvdbTransaction txn) {
		deleteImpl(key, txn);
	}

	public void prefixDelete(final byte[] filter) {
		prefixDeleteImpl(filter, null);
	}

	public void prefixDelete(final byte[] filter, final KvdbTransaction txn) {
		prefixDeleteImpl(filter, txn);
	}

	public KvsCursor cursor() {
		return new KvsCursor(this);
	}

	public KvsCursor cursor(final byte[] filt) {
		return new KvsCursor(this, filt);
	}

	public KvsCursor cursor(final EnumSet<CreateFlags> flags) {
		return new KvsCursor(this, flags);
	}

	public KvsCursor cursor(final KvdbTransaction txn) {
		return new KvsCursor(this, txn);
	}

	public KvsCursor cursor(final byte[] filt, final EnumSet<CreateFlags> flags) {
		return new KvsCursor(this, filt, flags);
	}


	public KvsCursor cursor(final byte[] filt, final KvdbTransaction txn) {
		return new KvsCursor(this, filt, txn);
	}

	public KvsCursor cursor(final EnumSet<CreateFlags> flags, final KvdbTransaction txn) {
		return new KvsCursor(this, flags, txn);
	}

	public KvsCursor cursor(final byte[] filt, final EnumSet<CreateFlags> flags, final KvdbTransaction txn) {
		return new KvsCursor(this, filt, flags, txn);
	}

	public synchronized void close() {
		if (c_hse_kvs != 0) {
			jni_hse_kvdb_kvs_close(c_hse_kvs);
			c_hse_kvs = 0;
		}
	}

	public enum PutFlags {
		PRIO(jni_hse_kvs_put_prio()),
		VCOMP_OFF(jni_hse_kvs_put_vcomp_off());

		private int value;

		private PutFlags(final int value) {
			this.value = value;
		}

		public static int getValue(EnumSet<PutFlags> flags) {
			int flagsValue = 0;
			for (final PutFlags flag : flags) {
				flagsValue |= flag.value;
			}

			return flagsValue;
		}
	}

	static {
		System.loadLibrary("hse-jni-2");
	}
}

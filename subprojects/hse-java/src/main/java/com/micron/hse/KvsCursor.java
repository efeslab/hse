/* SPDX-License-Identifier: Apache-2.0 */
/*
 * Copyright (C) 2021 Micron Technology, Inc.  All rights reserved.
 */

package com.micron.hse;

import java.io.EOFException;
import java.util.EnumSet;
import java.util.Optional;
import java.util.AbstractMap.SimpleImmutableEntry;

public final class KvsCursor implements AutoCloseable {
	private long c_hse_kvs_cursor = 0;

	private static native long jni_hse_kvs_cursor_create(long kvs_addr, byte[] filt, int flags, long txn_addr) throws HseException;
	private static native void jni_hse_kvs_cursor_update_view(long cursor_addr, int flags) throws HseException;
	private static native byte[] jni_hse_kvs_cursor_seek(long cursor_addr, byte[] key) throws HseException;
	private static native byte[] jni_hse_kvs_cursor_seek_range(long cursor_addr, byte[] filt_min, byte[] filt_max) throws HseException;
	private static native SimpleImmutableEntry<byte[], Optional<byte[]>> jni_hse_kvs_cursor_read(long cursor_addr) throws HseException;
	private static native void jni_hse_kvs_cursor_destroy(long cursor_addr) throws HseException;
	private static native int jni_hse_cursor_create_rev();

	KvsCursor(final Kvs kvs) {
		create(kvs, null, null, null);
	}

	KvsCursor(final Kvs kvs, final byte[] filt) {
		create(kvs, filt, null, null);
	}

	KvsCursor(final Kvs kvs, final EnumSet<CreateFlags> flags) {
		create(kvs, null, flags, null);
	}

	KvsCursor(final Kvs kvs, final KvdbTransaction txn) {
		create(kvs, null, null, txn);
	}

	KvsCursor(final Kvs kvs, final byte[] filt, final EnumSet<CreateFlags> flags) {
		create(kvs, filt, flags, null);
	}

	KvsCursor(final Kvs kvs, byte[] filt, final KvdbTransaction txn) {
		create(kvs, filt, null, txn);
	}

	KvsCursor(final Kvs kvs, EnumSet<CreateFlags> flags, final KvdbTransaction txn) {
		create(kvs, null, flags, txn);
	}

	KvsCursor(final Kvs kvs, byte[] filt, EnumSet<CreateFlags> flags, final KvdbTransaction txn) {
		create(kvs, filt, flags, txn);
	}

	private void create(final Kvs kvs, final byte[] filt, final EnumSet<CreateFlags> flags, final KvdbTransaction txn) throws HseException {
		c_hse_kvs_cursor = jni_hse_kvs_cursor_create(kvs.c_hse_kvs, filt, flags != null ? CreateFlags.getValue(flags) : 0, txn != null ? txn.c_hse_kvdb_txn : 0);
	}

	public SimpleImmutableEntry<byte[], Optional<byte[]>> read() throws EOFException {
		return jni_hse_kvs_cursor_read(c_hse_kvs_cursor);
	}

	public Optional<byte[]> seek(final byte[] key) {
		return Optional.ofNullable(jni_hse_kvs_cursor_seek(c_hse_kvs_cursor, key));
	}

	public Optional<byte[]> seekRange(final byte[] filtMin, final byte[] filtMax) {
		return Optional.ofNullable(jni_hse_kvs_cursor_seek_range(c_hse_kvs_cursor, filtMin, filtMax));
	}

	public void updateView() {
		jni_hse_kvs_cursor_update_view(c_hse_kvs_cursor, 0);
	}

	@Override
	public void close() {
		destroy();
	}

	public void destroy() {
		if (c_hse_kvs_cursor != 0) {
			jni_hse_kvs_cursor_destroy(this.c_hse_kvs_cursor);
			this.c_hse_kvs_cursor = 0;
		}
	}

	public static enum CreateFlags {
		REV(jni_hse_cursor_create_rev());

		private int value;

		private CreateFlags(final int value) {
			this.value = value;
		}

		public static int getValue(EnumSet<CreateFlags> flags) {
			int flagsValue = 0;
			for (final CreateFlags flag : flags) {
				flagsValue |= flag.value;
			}

			return flagsValue;
		}
	}

	static {
		System.loadLibrary("hse-jni-2");
	}
}

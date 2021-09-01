/* SPDX-License-Identifier: Apache-2.0 */
/*
 * Copyright (C) 2021 Micron Technology, Inc.  All rights reserved.
 */

package com.micron.hse;

public final class KvdbTransaction implements AutoCloseable {
	private static native void jni_hse_kvdb_txn_free(final long kvdb_addr, final long txn_addr);
	private static native void jni_hse_kvdb_txn_begin(final long kvdb_addr, final long txn_addr) throws HseException;
	private static native void jni_hse_kvdb_txn_abort(final long kvdb_addr, final long txn_addr) throws HseException;
	private static native void jni_hse_kvdb_txn_commit(final long kvdb_addr, final long txn_addr) throws HseException;
	private static native int jni_hse_kvdb_txn_state_get(final long kvdb_addr, final long txn_addr);

	private final Kvdb kvdb;
	long c_hse_kvdb_txn = 0;

	public enum State {
		INVALID,
		ACTIVE,
		COMMITTED,
		ABORTED;

		private static State valueOf(final int value) {
			switch (value) {
				case 0: return INVALID;
				case 1: return ACTIVE;
				case 2: return COMMITTED;
				case 3: return ABORTED;
				default: throw new IllegalStateException(String.format("Unknown state ordinal: %d", value));
			}
		}
	}

	KvdbTransaction(final Kvdb kvdb) {
		this.kvdb = kvdb;
	}

	public void begin() throws HseException {
		jni_hse_kvdb_txn_begin(kvdb.c_hse_kvdb, c_hse_kvdb_txn);
	}

	public void abort() throws HseException {
		jni_hse_kvdb_txn_abort(kvdb.c_hse_kvdb, c_hse_kvdb_txn);
	}

	public void commit() throws HseException {
		jni_hse_kvdb_txn_commit(kvdb.c_hse_kvdb, c_hse_kvdb_txn);
	}

	public State getState() {
		return State.valueOf(jni_hse_kvdb_txn_state_get(kvdb.c_hse_kvdb, c_hse_kvdb_txn));
	}

	static {
		System.loadLibrary("hse-jni-2");
	}

	@Override
	public void close() throws HseException {
		if (c_hse_kvdb_txn == 0)
			return;

		final State state = getState();
		if (state == State.ACTIVE) {
			commit();
		} else {
			abort();
		}

		jni_hse_kvdb_txn_free(kvdb.c_hse_kvdb, c_hse_kvdb_txn);
		c_hse_kvdb_txn = 0;
	}
}

/* SPDX-License-Identifier: Apache-2.0 */
/*
 * Copyright (C) 2021 Micron Technology, Inc.  All rights reserved.
 */

package com.micron.hse;

public final class Limits {
	private static native int jni_hse_kvs_count_max();
	private static native int jni_hse_kvs_key_len_max();
	private static native int jni_hse_kvs_value_len_max();
	private static native int jni_hse_kvs_pfx_len_max();
	private static native int jni_hse_kvs_name_len_max();

	public static final int KVS_COUNT_MAX = jni_hse_kvs_count_max();
	public static final int KVS_KEY_LEN_MAX = jni_hse_kvs_key_len_max();
	public static final int KVS_VALUE_LEN_MAX = jni_hse_kvs_value_len_max();
	public static final int KVS_PFX_LEN_MAX = jni_hse_kvs_pfx_len_max();
	public static final int KVS_NAME_LEN_MAX = jni_hse_kvs_value_len_max();

	static {
		System.loadLibrary("hse-jni-2");
	}
}

/* SPDX-License-Identifier: Apache-2.0 */
/*
 * Copyright (C) 2021 Micron Technology, Inc.  All rights reserved.
 */

package com.micron.hse;

public final class Version {
	private static final native String jni_hse_version_string();
	private static final native String jni_hse_version_tag();
	private static final native String jni_hse_version_sha();
	private static final native int jni_hse_version_major();
	private static final native int jni_hse_version_minor();
	private static final native int jni_hse_version_patch();

	// public static final String STRING = jni_hse_version_string();
	// public static final String TAG = jni_hse_version_tag();
	// public static final String SHA = jni_hse_version_sha();
	// public static final int MAJOR = jni_hse_version_major();
	// public static final int MINOR = jni_hse_version_minor();
	// public static final int PATCH = jni_hse_version_patch();

	static {
		System.loadLibrary("hse-jni-2");
	}
}

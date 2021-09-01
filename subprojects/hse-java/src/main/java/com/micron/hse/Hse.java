/* SPDX-License-Identifier: Apache-2.0 */
/*
 * Copyright (C) 2021 Micron Technology, Inc.  All rights reserved.
 */

package com.micron.hse;

import java.nio.file.Path;

public final class Hse {
	private static native void jni_hse_init(String config, String[] params) throws HseException;
	private static native void jni_hse_fini();

	public static void init() {
		jni_hse_init(null, null);
	}

	public static void init(final Path config) {
		jni_hse_init(config.toString(), null);
	}

	public static void init(final String config) {
		jni_hse_init(config, null);
	}

	public static void init(final Path config, final String... params) {
		jni_hse_init(config.toString(), params);
	}

	public static void init(final String config, final String... params) {
		jni_hse_init(config, params);
	}

	public static void fini() {
		jni_hse_fini();
	}

	static {
		System.loadLibrary("hse-jni-2");
	}
}

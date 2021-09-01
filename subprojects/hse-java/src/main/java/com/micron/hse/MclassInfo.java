/* SPDX-License-Identifier: Apache-2.0 */
/*
 * Copyright (C) 2021 Micron Technology, Inc.  All rights reserved.
 */

package com.micron.hse;

import java.nio.file.Path;

public final class MclassInfo {
    private long allocatedBytes;
    private long usedBytes;
    private Path path;

    private MclassInfo() {}

    public long getAllocatedBytes() {
        return allocatedBytes;
    }

    public long getUsedBytes() {
        return usedBytes;
    }

    public Path getPath() {
        return path;
    }

    static {
		System.loadLibrary("hse-jni-2");
	}
}

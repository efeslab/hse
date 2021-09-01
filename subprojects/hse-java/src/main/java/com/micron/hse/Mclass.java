/* SPDX-License-Identifier: Apache-2.0 */
/*
 * Copyright (C) 2021 Micron Technology, Inc.  All rights reserved.
 */

package com.micron.hse;

public enum Mclass {
    CAPACITY(Impl.jni_hse_mclass_capacity_get()),
    STAGING(Impl.jni_hse_mclass_staging_get()),
    PMEM(Impl.jni_hse_mclass_pmem_get());

    int value;

    private Mclass(final int value) {
        this.value = value;
    }

    @Override
    public String toString() {
        return Impl.jni_hse_mclass_name_get(this.value);
    }

    private static final class Impl {
        private static final native int jni_hse_mclass_capacity_get();
        private static final native int jni_hse_mclass_staging_get();
        private static final native int jni_hse_mclass_pmem_get();


        private static final native String jni_hse_mclass_name_get(int value);
    }
}

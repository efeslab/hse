/* SPDX-License-Identifier: Apache-2.0 */
/*
 * Copyright (C) 2021 Micron Technology, Inc.  All rights reserved.
 */

package com.micron.hse;

import org.junit.jupiter.api.AfterAll;
import org.junit.jupiter.api.BeforeAll;

public class HseTest {
    @BeforeAll
    public static void initializeHse() {
        Hse.init();
    }

    @AfterAll
    public static void finalizeHse() {
        Hse.fini();
    }
}

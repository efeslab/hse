/* SPDX-License-Identifier: Apache-2.0 */
/*
 * Copyright (C) 2021 Micron Technology, Inc.  All rights reserved.
 */

package com.micron.hse;

import org.junit.jupiter.api.AfterAll;
import org.junit.jupiter.api.BeforeAll;

public final class KvsTest extends KvdbTest {
	private static final String kvsName = "kvs-tests";

	private static Kvs kvs;

	@BeforeAll
	public static void setupKvs() {
		kvs = addKvs(kvsName);
	}

	@AfterAll
	public static void tearDownKvs() {
		kvs.close();
		kvdb.kvsDrop(kvsName);
	}
}

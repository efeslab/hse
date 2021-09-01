/* SPDX-License-Identifier: Apache-2.0 */
/*
 * Copyright (C) 2021 Micron Technology, Inc.  All rights reserved.
 */

package com.micron.hse;

import static org.junit.jupiter.api.Assertions.fail;

import org.junit.jupiter.api.AfterAll;
import org.junit.jupiter.api.BeforeAll;

import jnr.constants.platform.Errno;

public class KvdbTest extends HseTest {
	private static final String kvdbHome = System.getProperty("home");

	protected static Kvdb kvdb;

	@BeforeAll
	public static void setupKvdb() {
		try {
			Kvdb.create(kvdbHome);
			kvdb = Kvdb.open(kvdbHome);
		} catch (HseException e) {
			if (e.getErrno() == Errno.EEXIST) {
				kvdb = Kvdb.open(kvdbHome);
			} else {
				throw e;
			}
		}
	}

	protected static Kvs addKvs(final String kvsName) {
		try {
			kvdb.kvsCreate(kvsName);
			return kvdb.kvsOpen(kvsName);
		} catch (HseException e) {
			if (e.getErrno() == Errno.EEXIST) {
				return kvdb.kvsOpen(kvsName);
			} else {
				fail(e);
			}
		}

		return null;
	}


	@AfterAll
	public static void tearDownKvdb() {
		kvdb.close();
		Kvdb.drop(kvdbHome);
	}
}

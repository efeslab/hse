/* SPDX-License-Identifier: Apache-2.0 */
/*
 * Copyright (C) 2021 Micron Technology, Inc.  All rights reserved.
 */

package com.micron.hse;

import static org.junit.jupiter.api.Assertions.assertEquals;

import com.micron.hse.KvdbTransaction.State;

import org.junit.jupiter.api.AfterEach;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

public final class KvdbTransactionTest extends KvdbTest {
	private KvdbTransaction txn;

	@BeforeEach
	public void createTxn() {
		txn = kvdb.transaction();
	}

	@Test
	public void stateTransitions() {
		assertEquals(txn.getState(), State.INVALID);
		txn.begin();
		assertEquals(txn.getState(), State.ACTIVE);
		txn.abort();
		assertEquals(txn.getState(), State.ABORTED);
		txn.begin();
		txn.commit();
		assertEquals(txn.getState(), State.COMMITTED);
	}

	@AfterEach
	public void closeTxn() {
		txn.close();
	}
}

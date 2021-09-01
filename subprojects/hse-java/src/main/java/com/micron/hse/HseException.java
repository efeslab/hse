/* SPDX-License-Identifier: Apache-2.0 */
/*
 * Copyright (C) 2021 Micron Technology, Inc.  All rights reserved.
 */

package com.micron.hse;

import jnr.constants.platform.Errno;

public final class HseException extends RuntimeException {
	private static final long serialVersionUID = 8995408998818557762L;

	private final Errno errno;
	private final ErrCtx ctx;

	public HseException(final String message, final Errno errno, final ErrCtx ctx) {
		super(message);

		this.errno = errno;
		this.ctx = ctx;
	}

	public Errno getErrno() {
		return this.errno;
	}

	public ErrCtx getContext() {
		return this.ctx;
	}
}

/* SPDX-License-Identifier: Apache-2.0 */
/*
 * Copyright (C) 2021 Micron Technology, Inc.  All rights reserved.
 */

/** @file hse.h
 */

#ifndef HSE_FLAGS_H
#define HSE_FLAGS_H

#ifdef __cplusplus
extern "C" {
#endif

/* Documentation for these flags appears in hse.h */

/* hse_kvdb_sync() flags */
#define HSE_KVDB_SYNC_ASYNC (1u << 0)
#define HSE_KVDB_SYNC_RSVD1 (1u << 1)

/* hse_kvs_put() flags */
#define HSE_KVS_PUT_PRIO      (1u << 0)
#define HSE_KVS_PUT_VCOMP_OFF (1u << 1)

/* hse_kvs_cursor_create() flags */
#define HSE_CURSOR_CREATE_REV (1u << 0)

#ifdef HSE_EXPERIMENTAL
/* hse_kvdb_compact() flags */
#define HSE_KVDB_COMPACT_CANCEL   (1u << 0)
#define HSE_KVDB_COMPACT_SAMP_LWM (1u << 1)
#endif

#ifdef __cplusplus
}
#endif

#endif

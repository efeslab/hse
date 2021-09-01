/* SPDX-License-Identifier: Apache-2.0 */
/*
 * Copyright (C) 2021 Micron Technology, Inc.  All rights reserved.
 */

#ifndef HSE_KVDB_TXN_JNI
#define HSE_KVDB_TXN_JNI

#include <jni.h>

#define KVDB_TRANSACTION_CLASS "com/micron/hse/KvdbTransaction"

void
jni_hse_kvdb_txn_free(JNIEnv *env, jobject obj, jlong kvdb_addr, jlong txn_addr);

void
jni_hse_kvdb_txn_begin(JNIEnv *env, jobject obj, jlong kvdb_addr, jlong txn_addr);

void
jni_hse_kvdb_txn_commit(JNIEnv *env, jobject obj, jlong kvdb_addr, jlong txn_addr);

void
jni_hse_kvdb_txn_abort(JNIEnv *env, jobject obj, jlong kvdb_addr, jlong txn_addr);

jint
jni_hse_kvdb_txn_state_get(JNIEnv *env, jobject obj, jlong kvdb_addr, jlong txn_addr);

#endif /* _HSE_KVDB_TXN_JNI_ */

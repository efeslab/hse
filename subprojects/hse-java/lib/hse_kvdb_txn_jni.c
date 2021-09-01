/* SPDX-License-Identifier: Apache-2.0 */
/*
 * Copyright (C) 2021 Micron Technology, Inc.  All rights reserved.
 */

#include <jni.h>
#include <hse/hse.h>

#include "hse_jni.h"
#include "hse_kvdb_txn_jni.h"

void
jni_hse_kvdb_txn_free(JNIEnv *env, jobject obj, jlong kvdb_addr, jlong txn_addr)
{
    (void)env;
    (void)obj;

    struct hse_kvdb *    kvdb = (struct hse_kvdb *)kvdb_addr;
    struct hse_kvdb_txn *txn = (struct hse_kvdb_txn *)txn_addr;

    hse_kvdb_txn_free(kvdb, txn);
}

void
jni_hse_kvdb_txn_begin(JNIEnv *env, jobject obj, jlong kvdb_addr, jlong txn_addr)
{
    (void)obj;

    struct hse_kvdb *    kvdb = (struct hse_kvdb *)kvdb_addr;
    struct hse_kvdb_txn *txn = (struct hse_kvdb_txn *)txn_addr;

    const hse_err_t err = hse_kvdb_txn_begin(kvdb, txn);
    if (err)
        throw_new_hse_exception(env, err);
}

void
jni_hse_kvdb_txn_commit(JNIEnv *env, jobject obj, jlong kvdb_addr, jlong txn_addr)
{
    (void)obj;

    struct hse_kvdb *    kvdb = (struct hse_kvdb *)kvdb_addr;
    struct hse_kvdb_txn *txn = (struct hse_kvdb_txn *)txn_addr;

    const hse_err_t err = hse_kvdb_txn_commit(kvdb, txn);
    if (err)
        throw_new_hse_exception(env, err);
}

void
jni_hse_kvdb_txn_abort(JNIEnv *env, jobject obj, jlong kvdb_addr, jlong txn_addr)
{
    (void)obj;

    struct hse_kvdb *    kvdb = (struct hse_kvdb *)kvdb_addr;
    struct hse_kvdb_txn *txn = (struct hse_kvdb_txn *)txn_addr;

    const hse_err_t err = hse_kvdb_txn_abort(kvdb, txn);
    if (err)
        throw_new_hse_exception(env, err);
}

jint
jni_hse_kvdb_txn_state_get(JNIEnv *env, jobject obj, jlong kvdb_addr, jlong txn_addr)
{
    (void)env;
    (void)obj;

    struct hse_kvdb *    kvdb = (struct hse_kvdb *)kvdb_addr;
    struct hse_kvdb_txn *txn = (struct hse_kvdb_txn *)txn_addr;

    return hse_kvdb_txn_state_get(kvdb, txn);
}

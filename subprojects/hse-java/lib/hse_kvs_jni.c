/* SPDX-License-Identifier: Apache-2.0 */
/*
 * Copyright (C) 2021 Micron Technology, Inc.  All rights reserved.
 */

#include <stdbool.h>

#include <jni.h>
#include <hse/hse.h>

#include "hse_jni.h"
#include "hse_kvs_jni.h"

void
jni_hse_kvdb_kvs_close(JNIEnv *env, jobject obj, jlong kvs_addr)
{
    (void)obj;

    struct hse_kvs *kvs = (struct hse_kvs *)kvs_addr;

    const hse_err_t err = hse_kvdb_kvs_close(kvs);
    if (err) {
        throw_new_hse_exception(env, err);
    }
}

void
jni_hse_kvs_put(
    JNIEnv *   env,
    jobject    obj,
    jlong      kvs_addr,
    jbyteArray key,
    jbyteArray value,
    jint       flags,
    jlong      txn_addr)
{
    (void)obj;

    struct hse_kvs *     kvs = (struct hse_kvs *)kvs_addr;
    struct hse_kvdb_txn *txn = (struct hse_kvdb_txn *)txn_addr;

    jbyte *     key_data = (*env)->GetByteArrayElements(env, key, NULL);
    const jsize key_len = (*env)->GetArrayLength(env, key);
    jbyte *     value_data = (*env)->GetByteArrayElements(env, value, NULL);
    const jsize value_len = (*env)->GetArrayLength(env, value);

    const hse_err_t err = hse_kvs_put(kvs, flags, txn, key_data, key_len, value_data, value_len);

    (*env)->ReleaseByteArrayElements(env, key, key_data, JNI_ABORT);
    (*env)->ReleaseByteArrayElements(env, value, value_data, JNI_ABORT);

    if (err) {
        throw_new_hse_exception(env, err);
    }
}

jbyteArray
jni_hse_kvs_get(
    JNIEnv *   env,
    jobject    obj,
    jlong      kvs_addr,
    jobject    key,
    jbyteArray buf,
    jint       flags,
    jlong      txn_addr)
{
    (void)obj;

    struct hse_kvs *     kvs = (struct hse_kvs *)kvs_addr;
    struct hse_kvdb_txn *txn = (struct hse_kvdb_txn *)txn_addr;

    jbyte *     key_data = (*env)->GetByteArrayElements(env, key, NULL);
    const jsize key_len = (*env)->GetArrayLength(env, key);
    jbyte *     buf_data = (*env)->GetByteArrayElements(env, buf, NULL);
    const jsize buf_len = (*env)->GetArrayLength(env, buf);

    bool            found = false;
    size_t          value_len = 0;
    const hse_err_t err =
        hse_kvs_get(kvs, flags, txn, key_data, key_len, &found, buf_data, buf_len, &value_len);

    (*env)->ReleaseByteArrayElements(env, key, key_data, JNI_ABORT);
    (*env)->ReleaseByteArrayElements(env, key, key_data, 0);

    if (err) {
        throw_new_hse_exception(env, err);
    }

    if (!found) {
        return NULL;
    }

    return buf;
}

void
jni_hse_kvs_delete(
    JNIEnv *   env,
    jobject    obj,
    jlong      kvs_addr,
    jbyteArray key,
    jint       flags,
    jlong      txn_addr)
{
    (void)obj;

    struct hse_kvs *     kvs = (struct hse_kvs *)kvs_addr;
    struct hse_kvdb_txn *txn = (struct hse_kvdb_txn *)txn_addr;

    jbyte *     key_data = (*env)->GetByteArrayElements(env, key, NULL);
    const jsize key_len = (*env)->GetArrayLength(env, key);

    const hse_err_t err = hse_kvs_delete(kvs, flags, txn, key_data, key_len);

    (*env)->ReleaseByteArrayElements(env, key, key_data, JNI_ABORT);

    if (err) {
        throw_new_hse_exception(env, err);
    }
}

void
jni_hse_kvs_prefix_delete(
    JNIEnv *   env,
    jobject    obj,
    jlong      kvs_addr,
    jbyteArray filt,
    jint       flags,
    jlong      txn_addr)
{
    (void)obj;

    struct hse_kvs *     kvs = (struct hse_kvs *)kvs_addr;
    struct hse_kvdb_txn *txn = (struct hse_kvdb_txn *)txn_addr;

    jbyte *     filt_data = (*env)->GetByteArrayElements(env, filt, NULL);
    const jsize filt_len = (*env)->GetArrayLength(env, filt);

    const hse_err_t err = hse_kvs_prefix_delete(kvs, flags, txn, filt_data, filt_len);

    (*env)->ReleaseByteArrayElements(env, filt, filt_data, JNI_ABORT);

    if (err) {
        throw_new_hse_exception(env, err);
    }
}

jlong
jni_hse_kvs_cursor_create(
    JNIEnv *   env,
    jobject    obj,
    jlong      kvs_addr,
    jbyteArray filt,
    jint       flags,
    jlong      txn_addr)
{
    (void)obj;

    struct hse_kvs_cursor *cursor = NULL;
    struct hse_kvs *       kvs = (struct hse_kvs *)kvs_addr;
    struct hse_kvdb_txn *  txn = (struct hse_kvdb_txn *)txn_addr;

    jbyte *     filt_data = (*env)->GetByteArrayElements(env, filt, NULL);
    const jsize filt_len = (*env)->GetArrayLength(env, filt);


    const hse_err_t err = hse_kvs_cursor_create(kvs, flags, txn, filt_data, filt_len, &cursor);

    (*env)->ReleaseByteArrayElements(env, filt, filt_data, JNI_ABORT);

    if (err) {
        throw_new_hse_exception(env, err);
    }

    return (jlong)cursor;
}

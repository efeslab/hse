/* SPDX-License-Identifier: Apache-2.0 */
/*
 * Copyright (C) 2021 Micron Technology, Inc.  All rights reserved.
 */

#ifndef HSE_KVS_JNI_H
#define HSE_KVS_JNI_H

#include <jni.h>

#define KVS_CLASS "com/micron/hse/Kvs"

void
jni_hse_kvdb_kvs_close(JNIEnv *env, jobject obj, jlong kvs_addr);

void
jni_hse_kvs_put(
    JNIEnv *   env,
    jobject    obj,
    jlong      kvs_addr,
    jbyteArray key,
    jbyteArray value,
    jint       flags,
    jlong      txn_addr);

jarray
jni_hse_kvs_get(
    JNIEnv *   env,
    jobject    obj,
    jlong      kvs_addr,
    jbyteArray key,
    jbyteArray buf,
    jint       flags,
    jlong      txn_addr);

void
jni_hse_kvs_delete(
    JNIEnv *   env,
    jobject    obj,
    jlong      kvs_addr,
    jbyteArray key,
    jint       flags,
    jlong      txn_addr);

void
jni_hse_kvs_prefix_delete(
    JNIEnv *   env,
    jobject    obj,
    jlong      kvs_addr,
    jbyteArray filt,
    jint       flags,
    jlong      txn_addr);

jlong
jni_hse_kvs_cursor_create(
    JNIEnv *   env,
    jobject    obj,
    jlong      kvs_addr,
    jbyteArray filt,
    jint       flags,
    jlong      txn_addr);

#endif /* _HSE_KVS_JNI_H_ */

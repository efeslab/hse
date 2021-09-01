/* SPDX-License-Identifier: Apache-2.0 */
/*
 * Copyright (C) 2021 Micron Technology, Inc.  All rights reserved.
 */

#ifndef HSE_KVDB_JNI_H
#define HSE_KVDB_JNI_H

#include <jni.h>

#define KVDB_CLASS "com/micron/hse/Kvdb"

void
jni_hse_kvdb_create(JNIEnv *env, jobject obj, jstring kvdb_home, jobjectArray params);

jlong
jni_hse_kvdb_open(JNIEnv *env, jobject obj, jstring kvdb_home, jobjectArray params);

void
jni_hse_kvdb_close(JNIEnv *env, jobject obj, jlong kvdb_addr);

void
jni_hse_kvdb_drop(JNIEnv *env, jobject obj, jstring kvdb_home);

jarray
jni_hse_kvdb_kvs_names_get(JNIEnv *env, jobject obj, jlong kvdb_addr);

void
jni_hse_kvdb_kvs_create(
    JNIEnv *     env,
    jobject      obj,
    jlong        kvdb_addr,
    jstring      kvs_name,
    jobjectArray params);

jlong
jni_hse_kvdb_kvs_open(
    JNIEnv *     env,
    jobject      obj,
    jlong        kvdb_addr,
    jstring      kvs_name,
    jobjectArray params);

void
jni_hse_kvdb_kvs_drop(JNIEnv *env, jobject obj, jlong kvdb_addr, jstring kvs_name);

jlong
jni_hse_kvdb_txn_alloc(JNIEnv *env, jobject obj, jlong kvdb_addr);

void
jni_hse_kvdb_sync(JNIEnv *env, jobject obj, jlong kvdb_addr, jint flags);

jobject
jni_hse_kvdb_mclass_info_get(JNIEnv *env, jobject obj, jlong kvdb_addr, jint mclass);

void
jni_hse_kvdb_compact(JNIEnv *env, jobject obj, jlong kvdb_addr, jint flags);

jobject
jni_hse_kvdb_compact_status_get(JNIEnv *env, jobject obj, jlong kvdb_addr);

#endif

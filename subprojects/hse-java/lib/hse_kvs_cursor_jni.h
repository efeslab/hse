/* SPDX-License-Identifier: Apache-2.0 */
/*
 * Copyright (C) 2021 Micron Technology, Inc.  All rights reserved.
 */

#ifndef HSE_KVS_CURSOR_JNI_H
#define HSE_KVS_CURSOR_JNI_H

#include <jni.h>

#define KVS_CURSOR_CLASS "com/micron/hse/KvsCursor"

void
jni_hse_kvs_cursor_update_view(JNIEnv *env, jobject obj, jlong cursor_addr, jint flags);

jbyteArray
jni_hse_kvs_cursor_seek(JNIEnv *env, jobject obj, jlong cursor_addr, jbyteArray key);

jbyteArray
jni_hse_kvs_cursor_seek_range(
    JNIEnv *   env,
    jobject    obj,
    jlong      cursor_addr,
    jbyteArray filt_min,
    jbyteArray filt_max);

jobject
jni_hse_kvs_cursor_read(JNIEnv *env, jobject obj, jlong cursor_addr);

void
jni_hse_kvs_cursor_destroy(JNIEnv *env, jobject obj, jlong cursor_addr);

#endif /* _HSE_KVS_CURSOR_JNI_H */

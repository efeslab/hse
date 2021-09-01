/* SPDX-License-Identifier: Apache-2.0 */
/*
 * Copyright (C) 2021 Micron Technology, Inc.  All rights reserved.
 */

#include <jni.h>
#include <hse/hse.h>

#include "hse_jni.h"
#include "hse_kvs_cursor_jni.h"

void
jni_hse_kvs_cursor_update_view(JNIEnv *env, jobject obj, jlong cursor_addr, jint flags)
{
    (void)obj;

    struct hse_kvs_cursor *cursor = (struct hse_kvs_cursor *)cursor_addr;

    const hse_err_t err = hse_kvs_cursor_update_view(cursor, flags);
    if (err) {
        throw_new_hse_exception(env, err);
    }
}

jbyteArray
jni_hse_kvs_cursor_seek(JNIEnv *env, jobject obj, jlong cursor_addr, jbyteArray key)
{
    (void)obj;

    struct hse_kvs_cursor *cursor = (struct hse_kvs_cursor *)cursor_addr;

    jbyte *key_data = (*env)->GetByteArrayElements(env, key, NULL);
    jsize  key_len = (*env)->GetArrayLength(env, key);

    const void *    found = NULL;
    size_t          found_len = 0;
    const hse_err_t err = hse_kvs_cursor_seek(cursor, 0, key_data, key_len, &found, &found_len);
    (*env)->ReleaseByteArrayElements(env, key, key_data, JNI_ABORT);
    if (err) {
        throw_new_hse_exception(env, err);
        return NULL;
    }

    if (!found)
        return NULL;

    jbyteArray found_key = (*env)->NewByteArray(env, found_len);
    (*env)->SetByteArrayRegion(env, found_key, 0, found_len - 1, found);

    return found_key;
}

jbyteArray
jni_hse_kvs_cursor_seek_range(
    JNIEnv *   env,
    jobject    obj,
    jlong      cursor_addr,
    jbyteArray filt_min,
    jbyteArray filt_max)
{
    (void)obj;

    struct hse_kvs_cursor *cursor = (struct hse_kvs_cursor *)cursor_addr;

    jbyte *filt_min_data = (*env)->GetByteArrayElements(env, filt_min, NULL);
    jsize  filt_min_len = (*env)->GetArrayLength(env, filt_min);
    jbyte *filt_max_data = (*env)->GetByteArrayElements(env, filt_max, NULL);
    jsize  filt_max_len = (*env)->GetArrayLength(env, filt_max);

    const void *    found = NULL;
    size_t          found_len = 0;
    const hse_err_t err = hse_kvs_cursor_seek_range(
        cursor, 0, filt_min_data, filt_min_len, filt_max_data, filt_max_len, &found, &found_len);
    (*env)->ReleaseByteArrayElements(env, filt_min, filt_min_data, JNI_ABORT);
    (*env)->ReleaseByteArrayElements(env, filt_max, filt_max_data, JNI_ABORT);
    if (err) {
        throw_new_hse_exception(env, err);
        return NULL;
    }

    if (!found)
        return NULL;

    jbyteArray found_key = (*env)->NewByteArray(env, found_len);
    (*env)->SetByteArrayRegion(env, found_key, 0, found_len - 1, found);

    return found_key;
}

jobject
jni_hse_kvs_cursor_read(JNIEnv *env, jobject obj, jlong cursor_addr)
{
    (void)obj;

    struct hse_kvs_cursor *cursor = (struct hse_kvs_cursor *)cursor_addr;

    const void *    key = NULL;
    size_t          key_len = 0;
    const void *    value = NULL;
    size_t          value_len = 0;
    bool            eof = false;
    const hse_err_t err =
        hse_kvs_cursor_read(cursor, 0, &key, &key_len, &value, &value_len, &eof);
    if (err) {
        throw_new_hse_exception(env, err);
        return NULL;
    }

    if (eof) {
        throw_new_eof_exception(env, "End of cursor reached");
        return NULL;
    }

    jbyteArray key_byte_array = (*env)->NewByteArray(env, key_len);
    (*env)->SetByteArrayRegion(env, key_byte_array, 0, key_len - 1, key);

    jbyteArray value_byte_array = NULL;
    if (value) {
        value_byte_array = (*env)->NewByteArray(env, value_len);
        (*env)->SetByteArrayRegion(env, value_byte_array, 0, value_len - 1, value);
    }

    const jclass entry_class = (*env)->FindClass(env, "java.util.AbstractMap$SimpleImmutableEntry");
    if ((*env)->ExceptionCheck(env))
        return NULL;
    const jclass optional_class = (*env)->FindClass(env, "java/util/Optional");
    if ((*env)->ExceptionCheck(env))
        return NULL;

    jmethodID optional_class_ofNullable = (*env)->GetStaticMethodID(
        env, optional_class, "ofNullable", "(Ljava/lang/Object;)Ljava/lang/Object");
    if ((*env)->ExceptionCheck(env))
        return NULL;
    jobject optional_value_byte_array = (*env)->CallStaticObjectMethod(
        env, optional_class, optional_class_ofNullable, value_byte_array);
    if ((*env)->ExceptionCheck(env))
        return NULL;

    jmethodID entry_class_init = (*env)->GetMethodID(
        env, entry_class, "<init>", "(Ljava.util.AbstractMap$SimpleImmutableEntry;[BLjava/util/Optional;)V");
    if ((*env)->ExceptionCheck(env))
        return NULL;

    return (*env)->NewObject(
        env, entry_class, entry_class_init, key_byte_array, optional_value_byte_array);
}

void
jni_hse_kvs_cursor_destroy(JNIEnv *env, jobject obj, jlong cursor_addr)
{
    (void)obj;

    struct hse_kvs_cursor *cursor = (struct hse_kvs_cursor *)cursor_addr;

    const hse_err_t err = hse_kvs_cursor_destroy(cursor);
    if (err) {
        throw_new_hse_exception(env, err);
    }
}

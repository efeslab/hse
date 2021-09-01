/* SPDX-License-Identifier: Apache-2.0 */
/*
 * Copyright (C) 2021 Micron Technology, Inc.  All rights reserved.
 */

#include <assert.h>

#include <jni.h>
#include <hse/hse.h>

#include "hse_jni.h"
#include "hse_kvdb_jni.h"

#if __has_include(<hse/experimental.h>)
#include <hse/experimental.h>
#endif

void
jni_hse_kvdb_create(JNIEnv *env, jobject obj, jstring kvdb_home, jobjectArray params)
{
    (void)obj;

    const char * kvdb_home_chars = NULL;
    jsize        paramc;
    const char **paramv;

    if (kvdb_home)
        kvdb_home_chars = (*env)->GetStringUTFChars(env, kvdb_home, NULL);;

    to_paramv(env, params, &paramc, &paramv);

    const hse_err_t err = hse_kvdb_create(kvdb_home_chars, paramc, paramv);

    if (kvdb_home)
        (*env)->ReleaseStringUTFChars(env, kvdb_home, kvdb_home_chars);
    free_paramv(env, params, paramc, paramv);

    if (err) {
        throw_new_hse_exception(env, err);
    }
}

jlong
jni_hse_kvdb_open(JNIEnv *env, jobject obj, jstring kvdb_home, jobjectArray params)
{
    (void)obj;

    struct hse_kvdb *kvdb;

    const char * kvdb_home_chars = NULL;
    jsize        paramc;
    const char **paramv;

    if (kvdb_home)
        kvdb_home_chars = (*env)->GetStringUTFChars(env, kvdb_home, NULL);

    to_paramv(env, params, &paramc, &paramv);

    const hse_err_t err = hse_kvdb_open(kvdb_home_chars, paramc, paramv, &kvdb);

    if (kvdb_home)
        (*env)->ReleaseStringUTFChars(env, kvdb_home, kvdb_home_chars);
    free_paramv(env, params, paramc, paramv);

    if (err) {
        throw_new_hse_exception(env, err);
        return 0;
    }

    return (jlong)kvdb;
}

void
jni_hse_kvdb_close(JNIEnv *env, jobject obj, jlong kvdb_addr)
{
    (void)obj;

    struct hse_kvdb *kvdb = (struct hse_kvdb *)kvdb_addr;

    const hse_err_t err = hse_kvdb_close(kvdb);
    if (err) {
        throw_new_hse_exception(env, err);
        return;
    }
}

void
jni_hse_kvdb_drop(JNIEnv *env, jobject obj, jstring kvdb_home)
{
    (void)obj;

    const char * kvdb_home_chars = NULL;

    if (kvdb_home)
        kvdb_home_chars = (*env)->GetStringUTFChars(env, kvdb_home, NULL);;

    const hse_err_t err = hse_kvdb_drop(kvdb_home_chars);

    if (kvdb_home)
        (*env)->ReleaseStringUTFChars(env, kvdb_home, kvdb_home_chars);

    if (err) {
        throw_new_hse_exception(env, err);
    }
}

jarray
jni_hse_kvdb_kvs_names_get(JNIEnv *env, jobject obj, jlong kvdb_addr)
{
    (void)obj;

    struct hse_kvdb *kvdb = (struct hse_kvdb *)kvdb_addr;

    char **         namesv = NULL;
    size_t          namesc = 0;
    const hse_err_t err = hse_kvdb_kvs_names_get(kvdb, &namesc, &namesv);
    if (err) {
        throw_new_hse_exception(env, err);
        return NULL;
    }

    const jclass string_class = (*env)->FindClass(env, "java/lang/String");
    jarray       names = (*env)->NewObjectArray(env, namesc, string_class, NULL);
    for (unsigned int i = 0; i < namesc; i++) {
        jstring name = (*env)->NewStringUTF(env, namesv[i]);
        if ((*env)->ExceptionCheck(env))
            return NULL;

        (*env)->SetObjectArrayElement(env, names, i, name);
        if ((*env)->ExceptionCheck(env))
            return NULL;
    }

    hse_kvdb_kvs_names_free(kvdb, namesv);

    return names;
}

void
jni_hse_kvdb_kvs_create(
    JNIEnv *     env,
    jobject      obj,
    jlong        kvdb_addr,
    jstring      kvs_name,
    jobjectArray params)
{
    (void)obj;

    struct hse_kvdb *kvdb = (struct hse_kvdb *)kvdb_addr;

    const char * kvs_name_chars = (*env)->GetStringUTFChars(env, kvs_name, NULL);
    jsize        paramc;
    const char **paramv;

    to_paramv(env, params, &paramc, &paramv);

    const hse_err_t err = hse_kvdb_kvs_create(kvdb, kvs_name_chars, paramc, paramv);

    (*env)->ReleaseStringUTFChars(env, kvs_name, kvs_name_chars);
    free_paramv(env, params, paramc, paramv);

    if (err) {
        throw_new_hse_exception(env, err);
    }
}

jlong
jni_hse_kvdb_kvs_open(
    JNIEnv *     env,
    jobject      obj,
    jlong        kvdb_addr,
    jstring      kvs_name,
    jobjectArray params)
{
    (void)obj;

    assert(kvs_name);

    struct hse_kvdb *kvdb = (struct hse_kvdb *)kvdb_addr;
    struct hse_kvs * kvs;

    const char * kvs_name_chars = (*env)->GetStringUTFChars(env, kvs_name, NULL);
    jsize        paramc;
    const char **paramv;

    to_paramv(env, params, &paramc, &paramv);

    const hse_err_t err = hse_kvdb_kvs_open(kvdb, kvs_name_chars, paramc, paramv, &kvs);

    (*env)->ReleaseStringUTFChars(env, kvs_name, kvs_name_chars);
    free_paramv(env, params, paramc, paramv);

    if (err) {
        throw_new_hse_exception(env, err);
        return 0;
    }

    return (jlong)kvs;
}

void
jni_hse_kvdb_kvs_drop(JNIEnv *env, jobject obj, jlong kvdb_addr, jstring kvs_name)
{
    (void)obj;

    assert(kvs_name);

    struct hse_kvdb *kvdb = (struct hse_kvdb *)kvdb_addr;
    const char *     kvs_name_chars = (*env)->GetStringUTFChars(env, kvs_name, NULL);


    const hse_err_t err = hse_kvdb_kvs_drop(kvdb, kvs_name_chars);
    (*env)->ReleaseStringUTFChars(env, kvs_name, kvs_name_chars);
    if (err) {
        throw_new_hse_exception(env, err);
    }
}

jlong
jni_hse_kvdb_txn_alloc(JNIEnv *env, jobject obj, jlong kvdb_addr)
{
    (void)env;
    (void)obj;

    struct hse_kvdb *kvdb = (struct hse_kvdb *)kvdb_addr;

    struct hse_kvdb_txn *txn = hse_kvdb_txn_alloc(kvdb);

    return (jlong)txn;
}

void
jni_hse_kvdb_sync(JNIEnv *env, jobject obj, jlong kvdb_addr, jint flags)
{
    (void)obj;

    struct hse_kvdb *kvdb = (struct hse_kvdb *)kvdb_addr;

    const hse_err_t err = hse_kvdb_sync(kvdb, flags);
    if (err) {
        throw_new_hse_exception(env, err);
    }
}

jobject
jni_hse_kvdb_mclass_info_get(JNIEnv *env, jobject obj, jlong kvdb_addr, const jint mclass)
{
    (void)obj;

    struct hse_mclass_info info;
    struct hse_kvdb *      kvdb = (struct hse_kvdb *)kvdb_addr;

    const hse_err_t err = hse_kvdb_mclass_info_get(kvdb, (enum hse_mclass)mclass, &info);
    if (err) {
        throw_new_hse_exception(env, err);
        return NULL;
    }

    const jclass storage_info_class = (*env)->FindClass(env, "com/micron/hse/MclassInfo");
    if ((*env)->ExceptionCheck(env))
        return NULL;

    const jmethodID storage_info_init = (*env)->GetMethodID(
        env, storage_info_class, "<init>", "(Lcom/micron/hse/MclassInfo;)V");
    if ((*env)->ExceptionCheck(env))
        return NULL;

    const jfieldID allocated_bytes = (*env)->GetFieldID(env, storage_info_class, "allocatedBytes", "J");
    if ((*env)->ExceptionCheck(env))
        return NULL;
    const jfieldID used_bytes = (*env)->GetFieldID(env, storage_info_class, "usedBytes", "J");
    if ((*env)->ExceptionCheck(env))
        return NULL;

    jobject storage_info_obj = (*env)->NewObject(env, storage_info_class, storage_info_init);
    if ((*env)->ExceptionCheck(env))
        return NULL;

    (*env)->SetLongField(env, storage_info_obj, allocated_bytes, info.mi_allocated_bytes);
    (*env)->SetLongField(env, storage_info_obj, used_bytes, info.mi_used_bytes);

    return storage_info_obj;
}

void
jni_hse_kvdb_compact(JNIEnv *env, jobject obj, jlong kvdb_addr, jint flags)
{
    (void)obj;

#if __has_include(<hse/experimental.h>)
    struct hse_kvdb *kvdb = (struct hse_kvdb *)kvdb_addr;

    const hse_err_t err = hse_kvdb_compact(kvdb, flags);
    if (err) {
        throw_new_hse_exception(env, err);
    }
#else
    (void)kvdb_addr;
    (void)flags;

    throw_new_unsupported_operation_exception(env, "Experimental support is not enabled");
#endif
}

jobject
jni_hse_kvdb_compact_status_get(JNIEnv *env, jobject obj, jlong kvdb_addr)
{
    (void)obj;

#if __has_include(<hse/experimental.h>)
    struct hse_kvdb_compact_status compact_status;
    struct hse_kvdb *              kvdb = (struct hse_kvdb *)kvdb_addr;

    const hse_err_t err = hse_kvdb_compact_status_get(kvdb, &compact_status);
    if (err) {
        throw_new_hse_exception(env, err);
        return NULL;
    }

    const jclass compact_status_class = (*env)->FindClass(env, "com/micron/hse/Kvdb$CompactStatus");
    if ((*env)->ExceptionCheck(env))
        return NULL;

    const jmethodID compact_status_init = (*env)->GetMethodID(
        env, compact_status_class, "<init>", "(Lcom/micron/hse/Kvdb$CompactStatus;)V");
    if ((*env)->ExceptionCheck(env))
        return NULL;

    const jfieldID samp_lwm = (*env)->GetFieldID(env, compact_status_class, "sampLwm", "I");
    if ((*env)->ExceptionCheck(env))
        return NULL;
    const jfieldID samp_hwm = (*env)->GetFieldID(env, compact_status_class, "sampHwm", "I");
    if ((*env)->ExceptionCheck(env))
        return NULL;
    const jfieldID samp_curr = (*env)->GetFieldID(env, compact_status_class, "sampCurr", "I");
    if ((*env)->ExceptionCheck(env))
        return NULL;
    const jfieldID active = (*env)->GetFieldID(env, compact_status_class, "active", "Z");
    if ((*env)->ExceptionCheck(env))
        return NULL;
    const jfieldID canceled = (*env)->GetFieldID(env, compact_status_class, "canceled", "Z");
    if ((*env)->ExceptionCheck(env))
        return NULL;

    jobject compact_status_obj = (*env)->NewObject(env, compact_status_class, compact_status_init);
    if ((*env)->ExceptionCheck(env))
        return NULL;

    (*env)->SetIntField(env, compact_status_obj, samp_lwm, compact_status.kvcs_samp_lwm);
    (*env)->SetIntField(env, compact_status_obj, samp_hwm, compact_status.kvcs_samp_hwm);
    (*env)->SetIntField(env, compact_status_obj, samp_curr, compact_status.kvcs_samp_curr);
    (*env)->SetBooleanField(env, compact_status_obj, active, compact_status.kvcs_active);
    (*env)->SetBooleanField(env, compact_status_obj, canceled, compact_status.kvcs_canceled);

    return compact_status_obj;
#else
    (void)kvdb_addr;

    throw_new_unsupported_operation_exception(env, "Experimental support is not enabled");

    return NULL;
#endif
}

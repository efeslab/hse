/* SPDX-License-Identifier: Apache-2.0 */
/*
 * Copyright (C) 2021 Micron Technology, Inc.  All rights reserved.
 */

#include <assert.h>
#include <string.h>

#include <jni.h>
#include <hse/hse.h>
#include <hse/version.h>

#include "hse_jni.h"
#include "hse_kvdb_jni.h"
#include "hse_kvdb_txn_jni.h"
#include "hse_kvs_jni.h"
#include "hse_kvs_cursor_jni.h"
#include "hse_version_jni.h"

#define REGISTER(CLASS_NAME)                                                         \
    ({                                                                               \
        cls = (*env)->FindClass(env, CLASS_NAME##_CLASS);                            \
        if (!cls)                                                                    \
            return -1;                                                               \
        res = (*env)->RegisterNatives(                                               \
            env,                                                                     \
            cls,                                                                     \
            CLASS_NAME##_NATIVE_FUNCS,                                               \
            sizeof(CLASS_NAME##_NATIVE_FUNCS) / sizeof(*CLASS_NAME##_NATIVE_FUNCS)); \
        if (res)                                                                     \
            return -1;                                                               \
    })

#define UNREGISTER(CLASS_NAME)                            \
    ({                                                    \
        cls = (*env)->FindClass(env, CLASS_NAME##_CLASS); \
        if (!cls)                                         \
            return;                                       \
    })

const JNINativeMethod HSE_NATIVE_FUNCS[] = {
    { "jni_hse_init", "(Ljava/lang/String;[Ljava/lang/String;)V", (void *)jni_hse_init },
    { "jni_hse_fini", "()V", (void *)jni_hse_fini },
};

const JNINativeMethod VERSION_NATIVE_FUNCS[] = {
    { "jni_hse_version_string", "()Ljava/lang/String;", (void *)jni_hse_version_string },
    { "jni_hse_version_major", "()I", (void *)jni_hse_version_major },
    { "jni_hse_version_minor", "()I", (void *)jni_hse_version_minor },
    { "jni_hse_version_patch", "()I", (void *)jni_hse_version_patch },
};

const JNINativeMethod KVDB_NATIVE_FUNCS[] = {
    { "jni_hse_kvdb_create", "(Ljava/lang/String;[Ljava/lang/String;)V", (void *)jni_hse_kvdb_create },
    { "jni_hse_kvdb_open", "(Ljava/lang/String;[Ljava/lang/String;)J", (void *)jni_hse_kvdb_open },
    { "jni_hse_kvdb_close", "(J)V", (void *)jni_hse_kvdb_close },
    { "jni_hse_kvdb_drop", "(Ljava/lang/String;)V", (void *)jni_hse_kvdb_drop },
    { "jni_hse_kvdb_kvs_names_get", "(J)[Ljava/lang/String;", (void *)jni_hse_kvdb_kvs_names_get },
    { "jni_hse_kvdb_kvs_create", "(JLjava/lang/String;[Ljava/lang/String;)V", (void *)jni_hse_kvdb_kvs_create },
    { "jni_hse_kvdb_kvs_open", "(JLjava/lang/String;[Ljava/lang/String;)J", (void *)jni_hse_kvdb_kvs_open },
    { "jni_hse_kvdb_kvs_drop", "(JLjava/lang/String;)V", (void *)jni_hse_kvdb_kvs_drop },
    { "jni_hse_kvdb_txn_alloc", "(J)J", (void *)jni_hse_kvdb_txn_alloc },
    { "jni_hse_kvdb_sync", "(JI)V", (void *)jni_hse_kvdb_sync },
    { "jni_hse_kvdb_mclass_info_get", "(JI)Lcom/micron/hse/MclassInfo;", (void*)jni_hse_kvdb_mclass_info_get },
    { "jni_hse_kvdb_compact", "(JI)V", (void *)jni_hse_kvdb_compact },
    { "jni_hse_kvdb_compact_status_get",
      "(J)Lcom/micron/hse/Kvdb$CompactStatus;",
      (void *)jni_hse_kvdb_compact_status_get },
};

const JNINativeMethod KVDB_TRANSACTION_NATIVE_FUNCS[] = {
    { "jni_hse_kvdb_txn_free", "(JJ)V", (void *)jni_hse_kvdb_txn_free },
    { "jni_hse_kvdb_txn_begin", "(JJ)V", (void *)jni_hse_kvdb_txn_begin },
    { "jni_hse_kvdb_txn_abort", "(JJ)V", (void *)jni_hse_kvdb_txn_abort },
    { "jni_hse_kvdb_txn_commit", "(JJ)V", (void *)jni_hse_kvdb_txn_commit },
    { "jni_hse_kvdb_txn_state_get", "(JJ)I", (void *)jni_hse_kvdb_txn_state_get },
};

const JNINativeMethod KVS_NATIVE_FUNCS[] = {
    { "jni_hse_kvdb_kvs_close", "(J)V", (void *)jni_hse_kvdb_kvs_close },
    { "jni_hse_kvs_put", "(J[B[BIJ)V", (void *)jni_hse_kvs_put },
    { "jni_hse_kvs_get", "(J[B[BIJ)[B", (void *)jni_hse_kvs_get },
    { "jni_hse_kvs_delete", "(J[BIJ)V", (void *)jni_hse_kvs_delete },
    { "jni_hse_kvs_prefix_delete", "(J[BIJ)V", (void *)jni_hse_kvs_prefix_delete },
};

const JNINativeMethod KVS_CURSOR_NATIVE_FUNCS[] = {
    { "jni_hse_kvs_cursor_create", "(J[BIJ)J", (void *)jni_hse_kvs_cursor_create },
    { "jni_hse_kvs_cursor_update_view", "(JI)V", (void *)jni_hse_kvs_cursor_update_view },
    { "jni_hse_kvs_cursor_seek", "(J[B)[B", (void *)jni_hse_kvs_cursor_seek },
    { "jni_hse_kvs_cursor_seek_range", "(J[B[B)[B", (void *)jni_hse_kvs_cursor_seek_range },
    { "jni_hse_kvs_cursor_read", "(J)Ljava/util/AbstractMap$SimpleImmutableEntry;", (void *)jni_hse_kvs_cursor_read },
    { "jni_hse_kvs_cursor_destroy", "(J)V", (void *)jni_hse_kvs_cursor_destroy },
};

void
to_paramv(JNIEnv *env, jobjectArray params, jsize *paramc, const char ***paramv)
{
    jsize        tmp_pc;
    const char **tmp_pv = NULL;

    assert(env);
    assert(paramc);
    assert(paramv);

    *paramc = 0;
    *paramv = NULL;

    if (!params)
        return;

    tmp_pc = (*env)->GetArrayLength(env, params);

    if (tmp_pc == 0)
        return;

    tmp_pv = malloc(tmp_pc * sizeof(char *));
    if (!tmp_pv)
        throw_new_out_of_memory_error(env, "Failed to allocate memory for C-string paramater array");

    for (jsize i = 0; i < tmp_pc; i++) {
        const jobject param = (*env)->GetObjectArrayElement(env, params, i);

        tmp_pv[i] = (*env)->GetStringUTFChars(env, param, NULL);
    }

    *paramc = tmp_pc;
    *paramv = tmp_pv;
}

void
free_paramv(JNIEnv *env, jobjectArray params, jsize paramc, const char **paramv)
{
    assert(env);

    for (jsize i = 0; i < paramc; i++) {
        const jobject param = (*env)->GetObjectArrayElement(env, params, i);

        (*env)->ReleaseStringUTFChars(env, param, paramv[i]);
    }

    free(paramv);
}

void
jni_hse_init(JNIEnv *env, jobject obj, jstring config, jobjectArray params)
{
    (void)obj;

    const char * config_chars = NULL;
    jsize        paramc;
    const char **paramv;

    if (config)
        config_chars = (*env)->GetStringUTFChars(env, config, NULL);

    to_paramv(env, params, &paramc, &paramv);

    const hse_err_t err = hse_init(config_chars, paramc, paramv);

    if (config)
        (*env)->ReleaseStringUTFChars(env, config, config_chars);
    free_paramv(env, params, paramc, paramv);

    if (err)
        throw_new_hse_exception(env, err);
}

void
jni_hse_fini(JNIEnv *env, jobject obj)
{
    (void)env;
    (void)obj;

    hse_fini();
}

jint
throw_new_hse_exception(JNIEnv *env, hse_err_t err)
{
    const jclass hse_exception_class = (*env)->FindClass(env, "com/micron/hse/HseException");
    if ((*env)->ExceptionCheck(env))
        return -1;

    const size_t needed_sz = hse_strerror(err, NULL, 0);
    char *buf = malloc(needed_sz + 1);
    if (!buf)
        return throw_new_out_of_memory_error(env, "Failed to allocate memory for error buffer");

    hse_strerror(err, buf, needed_sz + 1);

    const jstring message = (*env)->NewStringUTF(env, buf);
    free(buf);
    if ((*env)->ExceptionCheck(env))
        return -1;

    const int errno = hse_err_to_errno(err);
    const enum hse_err_ctx ctx = hse_err_to_ctx(err);

    const jmethodID hse_exception_class_init =
        (*env)->GetMethodID(env, hse_exception_class, "<init>", "(Ljava/lang/String;Ljnr/constants/platform/Errno;Lcom/micron/hse/ErrCtx;)V");
    if ((*env)->ExceptionCheck(env))
        return -1;

    const jclass errno_class = (*env)->FindClass(env, "jnr/constants/platform/Errno");
    if ((*env)->ExceptionCheck(env))
        return -1;
    const jmethodID errno_value_of =
        (*env)->GetStaticMethodID(env, errno_class, "valueOf", "(J)Ljnr/constants/platform/Errno;");
    if ((*env)->ExceptionCheck(env))
        return -1;

    const jobject errno_object = (*env)->CallStaticObjectMethod(env, errno_class, errno_value_of, (jlong)errno);
    if ((*env)->ExceptionCheck(env))
        return -1;

    const jclass err_ctx_class = (*env)->FindClass(env, "com/micron/hse/ErrCtx");
    if ((*env)->ExceptionCheck(env))
        return -1;

    const char *err_ctx_field_name = NULL;
    switch (ctx) {
        case HSE_ERR_CTX_NONE:
            err_ctx_field_name = "NONE";
    }

    assert(err_ctx_field_name);

    const jfieldID err_ctx_field = (*env)->GetStaticFieldID(env, err_ctx_class, err_ctx_field_name, "Lcom/micron/hse/ErrCtx;");
    if ((*env)->ExceptionCheck(env))
        return -1;

    const jobject err_ctx_object = (*env)->GetStaticObjectField(env, err_ctx_class, err_ctx_field);
    if ((*env)->ExceptionCheck(env))
        return -1;

    const jobject hse_exception_object = (*env)->NewObject(
        env,
        hse_exception_class,
        hse_exception_class_init,
        message,
        errno_object,
        err_ctx_object);
    if ((*env)->ExceptionCheck(env))
        return -1;

    return (*env)->Throw(env, (jthrowable)hse_exception_object);
}

jint
throw_new_eof_exception(JNIEnv *env, const char *message)
{
    const jclass eof_exception_class = (*env)->FindClass(env, "java/io/EOFException");
    if ((*env)->ExceptionCheck(env))
        return -1;

    return (*env)->ThrowNew(env, eof_exception_class, message);
}

jint
throw_new_out_of_memory_error(JNIEnv *env, const char *message)
{
    const jclass out_of_memory_error_class = (*env)->FindClass(env, "java/lang/OutOfMemoryError");
    if ((*env)->ExceptionCheck(env))
        return -1;

    return (*env)->ThrowNew(env, out_of_memory_error_class, message);
}

jint
throw_new_unsupported_operation_exception(JNIEnv *env, const char *message)
{
    const jclass unsupported_operation_exception_class = (*env)->FindClass(env, "java/lang/UnsupportedOperationException");
    if ((*env)->ExceptionCheck(env))
        return -1;

    return (*env)->ThrowNew(env, unsupported_operation_exception_class, message);
}

JNIEXPORT jint JNICALL
JNI_OnLoad(JavaVM *vm, void *reserved)
{
    (void)reserved;

    jclass cls;
    jint   res;

    JNIEnv *env;
    if ((*vm)->GetEnv(vm, (void **)&env, JNI_VERSION_1_8) != JNI_OK)
        return -1;

    REGISTER(HSE);
    REGISTER(VERSION);
    REGISTER(KVDB);
    REGISTER(KVDB_TRANSACTION);
    REGISTER(KVS);
    REGISTER(KVS_CURSOR);

    return JNI_VERSION_1_8;
}

JNIEXPORT void JNICALL
JNI_OnUnload(JavaVM *vm, void *reserved)
{
    (void)reserved;

    jclass cls;

    JNIEnv *env;
    if ((*vm)->GetEnv(vm, (void **)&env, JNI_VERSION_1_8) != JNI_OK)
        return;

    UNREGISTER(HSE);
    UNREGISTER(VERSION);
    UNREGISTER(KVDB);
    UNREGISTER(KVDB_TRANSACTION);
    UNREGISTER(KVS);
    UNREGISTER(KVS_CURSOR);

    (*env)->UnregisterNatives(env, cls);
}

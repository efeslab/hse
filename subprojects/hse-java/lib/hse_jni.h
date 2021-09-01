/* SPDX-License-Identifier: Apache-2.0 */
/*
 * Copyright (C) 2021 Micron Technology, Inc.  All rights reserved.
 */

#ifndef HSE_JNI_H
#define HSE_JNI_H

#include <jni.h>
#include <hse/hse.h>

#define HSE_CLASS "com/micron/hse/Hse"

void
to_paramv(JNIEnv *env, jobjectArray params, jsize *paramc, const char ***paramv);

void
free_paramv(JNIEnv *env, jobjectArray params, jsize paramc, const char **paramv);

void
jni_hse_init(JNIEnv *env, jobject obj, jstring config, jobjectArray params);

void
jni_hse_fini(JNIEnv *env, jobject obj);

jint
throw_new_hse_exception(JNIEnv *env, hse_err_t err);

jint
throw_new_eof_exception(JNIEnv *env, const char *message);

jint
throw_new_out_of_memory_error(JNIEnv *env, const char *message);

jint
throw_new_unsupported_operation_exception(JNIEnv *env, const char *message);

#endif /* _HSE_JNI_H_ */

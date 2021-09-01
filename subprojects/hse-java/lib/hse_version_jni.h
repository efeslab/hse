/* SPDX-License-Identifier: Apache-2.0 */
/*
 * Copyright (C) 2021 Micron Technology, Inc.  All rights reserved.
 */

#ifndef HSE_VERSION_JNI_H
#define HSE_VERSION_JNI_H

#include <jni.h>

#define VERSION_CLASS "com/micron/hse/Version"

jstring
jni_hse_version_string(JNIEnv *env, jobject obj);

jint
jni_hse_version_major(JNIEnv *env, jobject obj);

jint
jni_hse_version_minor(JNIEnv *env, jobject obj);

jint
jni_hse_version_patch(JNIEnv *env, jobject obj);

#endif

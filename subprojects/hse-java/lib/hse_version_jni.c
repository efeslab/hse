/* SPDX-License-Identifier: Apache-2.0 */
/*
 * Copyright (C) 2021 Micron Technology, Inc.  All rights reserved.
 */

#include <jni.h>

#include <hse/hse.h>
#include <hse/version.h>

#include "hse_version_jni.h"

jstring
jni_hse_version_string(JNIEnv *env, jobject obj)
{
    (void)env;
    (void)obj;

    return (*env)->NewStringUTF(env, HSE_VERSION_STRING);
}

jint
jni_hse_version_major(JNIEnv *env, jobject obj)
{
    (void)env;
    (void)obj;

    return HSE_VERSION_MAJOR;
}

jint
jni_hse_version_minor(JNIEnv *env, jobject obj)
{
    (void)env;
    (void)obj;

    return HSE_VERSION_MINOR;
}

jint
jni_hse_version_patch(JNIEnv *env, jobject obj)
{
    (void)env;
    (void)obj;

    return HSE_VERSION_PATCH;
}

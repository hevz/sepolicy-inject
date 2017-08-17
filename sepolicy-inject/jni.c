/*
 ============================================================================
 Name        : jni.c
 Author      : Heiher <r@hev.cc>
 Copyright   : Copyright (c) 2017 everyone.
 Description : JNI
 ============================================================================
 */

#include <jni.h>
#include <pthread.h>

#include <stdlib.h>
#include <signal.h>
#include <string.h>

#ifndef PKGNAME
#define PKGNAME    hev/htproxy
#endif

#define STR(s)    STR_ARG(s)
#define STR_ARG(c)    #c
#define N_ELEMENTS(arr)        (sizeof (arr) / sizeof ((arr)[0]))

static JavaVM *java_vm;
static pthread_key_t current_jni_env;

extern int main (int, char **);

static int native_sepolicy_inject (JNIEnv *env, jobject thiz, jobjectArray args);

static JNINativeMethod native_methods[] =
{
    { "SepolicyInject", "([Ljava/lang/String;)I", (void *) native_sepolicy_inject },
};

static void
detach_current_thread (void *env)
{
    (*java_vm)->DetachCurrentThread (java_vm);
}

jint
JNI_OnLoad (JavaVM *vm, void *reserved)
{
    JNIEnv *env = NULL;
    jclass klass;

    java_vm = vm;
    if (JNI_OK != (*vm)->GetEnv (vm, (void**) &env, JNI_VERSION_1_4)) {
        return 0;
    }

    klass = (*env)->FindClass (env, STR(PKGNAME)"/SepolicyInjecter");
    (*env)->RegisterNatives (env, klass, native_methods, N_ELEMENTS (native_methods));
    (*env)->DeleteLocalRef (env, klass);

    pthread_key_create (&current_jni_env, detach_current_thread);

    return JNI_VERSION_1_4;
}

static int
native_sepolicy_inject (JNIEnv *env, jobject thiz, jobjectArray args)
{
    jsize i, argc = (*env)->GetArrayLength (env, args);
    char *argv[argc];
    int ret;

    for (i = 0; i < argc; i++) {
        jstring arg = (jstring) (*env)->GetObjectArrayElement (env, args, i);
	argv[i] = (char *) (*env)->GetStringUTFChars (env, arg, 0);
    }

    ret = main (argc, argv);

    for (i = 0; i < argc; i++) {
        jstring arg = (jstring) (*env)->GetObjectArrayElement (env, args, i);
        (*env)->ReleaseStringUTFChars (env, arg, (const char *) argv[i]);
    }

    return ret;
}


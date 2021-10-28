/* SPDX-License-Identifier: Apache-2.0 */
/*
 * Copyright (C) 2021 Micron Technology, Inc.  All rights reserved.
 */

#include <hse_ut/framework.h>
#include <fixtures/kvdb.h>
#include <fixtures/kvs.h>

/* Globals */
struct hse_kvdb *kvdb_handle = NULL;
struct hse_kvs * kvs_handle = NULL;
const int        key_value_pairs = 5;
const char *     kvs_name = "kvs_cursor_test";

int
test_collection_setup(struct mtf_test_info *lcl_ti)
{
    hse_err_t err;

    err = fxt_kvdb_setup(home, 0, NULL, 0, NULL, &kvdb_handle);

    return hse_err_to_errno(err);
}

int
test_collection_teardown(struct mtf_test_info *lcl_ti)
{
    hse_err_t err;

    err = fxt_kvdb_teardown(home, kvdb_handle);

    return hse_err_to_errno(err);
}

int
open_kvs(struct mtf_test_info *lcl_ti)
{
    hse_err_t err;

    err = fxt_kvs_setup(kvdb_handle, kvs_name, 0, NULL, 0, NULL, &kvs_handle);

    return hse_err_to_errno(err);
}

int
populate_kvs(struct mtf_test_info *lcl_ti)
{
    int       n;
    char      key[16], val[16];
    hse_err_t err;

    open_kvs(lcl_ti);

    ASSERT_LT_RET(key_value_pairs, 100, -1);

    for (int i = 0; i < key_value_pairs; i++) {
        n = snprintf(key, sizeof(key), "test_key_%02d", i);
        ASSERT_LT_RET(n, sizeof(key), -1);

        n = snprintf(val, sizeof(val), "test_value_%02d", i);
        ASSERT_LT_RET(n, sizeof(val), -1);

        err = hse_kvs_put(kvs_handle, 0, NULL, key, strlen(key), val, strlen(val));
        ASSERT_EQ_RET(err, 0, -1);
    }

    return 0;
}

int
destroy_kvs(struct mtf_test_info *lcl_ti)
{
    hse_err_t err;

    err = fxt_kvs_teardown(kvdb_handle, kvs_name, kvs_handle);

    return hse_err_to_errno(err);
}

MTF_BEGIN_UTEST_COLLECTION_PREPOST(
    cursor_api_test,
    test_collection_setup,
    test_collection_teardown);

MTF_DEFINE_UTEST(cursor_api_test, cursor_invalid_testcase)
{
    bool                   eof = false;
    const void *           cur_key, *cur_val;
    size_t                 cur_klen, cur_vlen;
    hse_err_t              err;
    struct hse_kvs_cursor *cursor;

    /* TC: A cursor cannot be created without a valid KVS */
    err = hse_kvs_cursor_create(NULL, 0, NULL, NULL, 0, &cursor);
    ASSERT_EQ(hse_err_to_errno(err), EINVAL);

    /* TC: A null cursor cannot be updated */
    err = hse_kvs_cursor_update_view(NULL, 0);
    ASSERT_EQ(hse_err_to_errno(err), EINVAL);

    /* TC: A null cursor cannot be used to read a KVS */
    err = hse_kvs_cursor_read(NULL, 0, &cur_key, &cur_klen, &cur_val, &cur_vlen, &eof);
    ASSERT_EQ(hse_err_to_errno(err), EINVAL);

    /* TC: A null cursor cannot be used to seek */
    err = hse_kvs_cursor_seek(NULL, 0, "search_key", strlen("search_key"), NULL, NULL);
    ASSERT_EQ(hse_err_to_errno(err), EINVAL);

    /* TC: A null cursor cannot be used to move to the closest match to key */
    err = hse_kvs_cursor_seek_range(NULL, 0, &cur_key, cur_klen, cur_val, cur_vlen, NULL, NULL);
    ASSERT_EQ(hse_err_to_errno(err), EINVAL);

    /* TC: A null cursor cannot be destroyed */
    err = hse_kvs_cursor_destroy(NULL);
    ASSERT_EQ(hse_err_to_errno(err), EINVAL);
}

MTF_DEFINE_UTEST_PREPOST(cursor_api_test, cursor_valid_testcase, open_kvs, destroy_kvs)
{
    hse_err_t              err;
    struct hse_kvs_cursor *cursor_handle;
    struct hse_kvs_cursor *duplicate_cursor_handle;

    /* TC: A cursor can be created */
    err = hse_kvs_cursor_create(kvs_handle, 0, NULL, NULL, 0, &cursor_handle);
    ASSERT_EQ(err, 0);

    /* TC: A populated KVS returns a valid cursor */
    ASSERT_NE(cursor_handle, NULL);

    /* TC: A kvs handle can be reused to create multiple cursors */
    err = hse_kvs_cursor_create(kvs_handle, 0, NULL, NULL, 0, &duplicate_cursor_handle);
    ASSERT_EQ(err, 0);

    /* TC: A cursor can be destroyed */
    err = hse_kvs_cursor_destroy(cursor_handle);
    ASSERT_EQ(err, 0);
    err = hse_kvs_cursor_destroy(duplicate_cursor_handle);
    ASSERT_EQ(err, 0);
}

MTF_DEFINE_UTEST_PREPOST(cursor_api_test, cursor_read_testcase, populate_kvs, destroy_kvs)
{
    int                    count = 0, n;
    bool                   eof = false;
    const void *           cur_key, *cur_val;
    size_t                 cur_klen, cur_vlen;
    char                   expec_buff[16];
    hse_err_t              err;
    struct hse_kvs_cursor *cursor;

    err = hse_kvs_cursor_create(kvs_handle, 0, NULL, NULL, 0, &cursor);
    ASSERT_EQ(err, 0);

    /* TC: A cursor can read key value pairs in a KVS, and returns the correct key value pairs */
    while (!eof) {
        cur_key = cur_val = NULL;
        cur_klen = cur_vlen = 0;
        err = hse_kvs_cursor_read(cursor, 0, &cur_key, &cur_klen, &cur_val, &cur_vlen, &eof);
        ASSERT_EQ(err, 0);

        if (!eof) {
            ASSERT_NE(cur_key, NULL);
            ASSERT_NE(cur_val, NULL);
            n = snprintf(
                expec_buff, sizeof(expec_buff), "test_key_%02d", (count)); /* keep within limits */
            ASSERT_LT(n, sizeof(expec_buff));
            ASSERT_EQ(cur_klen, strlen(expec_buff));
            ASSERT_EQ(memcmp(expec_buff, cur_key, cur_klen), 0);

            n = snprintf(expec_buff, sizeof(expec_buff), "test_value_%02d", (count++));
            ASSERT_LT(n, sizeof(expec_buff));
            ASSERT_EQ(cur_vlen, strlen(expec_buff));
            ASSERT_EQ(memcmp(expec_buff, cur_val, cur_vlen), 0);
        }
    }

    err = hse_kvs_cursor_destroy(cursor);
    ASSERT_EQ(err, 0);
}

MTF_DEFINE_UTEST_PREPOST(cursor_api_test, cursor_read_changes_testcase, populate_kvs, destroy_kvs)
{
    int                    count = 0;
    bool                   eof = false;
    const void *           cur_key, *cur_val;
    size_t                 cur_klen, cur_vlen;
    hse_err_t              err;
    struct hse_kvs_cursor *cursor;

    err = hse_kvs_cursor_create(kvs_handle, 0, NULL, NULL, 0, &cursor);
    ASSERT_EQ(err, 0);

    /* TC: An existing cursor cannot see changes to the KVS */
    err = hse_kvs_put(kvs_handle, 0, NULL, "extra_key", 9, "extra_value", 11);
    ASSERT_EQ(err, 0);

    while (!eof) {
        err = hse_kvs_cursor_read(cursor, 0, &cur_key, &cur_klen, &cur_val, &cur_vlen, &eof);
        ASSERT_EQ(err, 0);

        if (!eof)
            count++;
    }

    ASSERT_EQ(count, key_value_pairs);

    err = hse_kvs_cursor_destroy(cursor);
    ASSERT_EQ(err, 0);
}

MTF_DEFINE_UTEST_PREPOST(cursor_api_test, cursor_seek_testcase, populate_kvs, destroy_kvs)
{
    bool                   eof = false;
    const void *           cur_key, *cur_val;
    size_t                 cur_klen, cur_vlen;
    char *                 search_key = "test_key_02";
    hse_err_t              err;
    struct hse_kvs_cursor *cursor;

    err = hse_kvs_cursor_create(kvs_handle, 0, NULL, NULL, 0, &cursor);
    ASSERT_EQ(err, 0);

    /* TC: A cursor can seek to a specific key */
    err = hse_kvs_cursor_seek(cursor, 0, search_key, strlen(search_key), NULL, NULL);
    ASSERT_EQ(err, 0);

    err = hse_kvs_cursor_read(cursor, 0, &cur_key, &cur_klen, &cur_val, &cur_vlen, &eof);
    ASSERT_EQ(err, 0);

    ASSERT_EQ(cur_klen, strlen(search_key));
    ASSERT_EQ(memcmp(search_key, cur_key, cur_klen), 0);

    /* TC: A cursor will be positioned at the first key if the specified key does not exist */
    eof = false;
    err = hse_kvs_cursor_seek(cursor, 0, "fake_key", 8, NULL, NULL);
    ASSERT_EQ(err, 0);

    err = hse_kvs_cursor_read(cursor, 0, &cur_key, &cur_klen, &cur_val, &cur_vlen, &eof);
    ASSERT_EQ(err, 0);

    ASSERT_EQ(cur_klen, strlen("test_key_00"));
    ASSERT_EQ(memcmp("test_key_00", cur_key, cur_klen), 0);

    err = hse_kvs_cursor_destroy(cursor);
    ASSERT_EQ(err, 0);
}

MTF_DEFINE_UTEST_PREPOST(cursor_api_test, cursor_multiple_testcase, populate_kvs, destroy_kvs)
{
    int                    count_1 = 0, count_2 = 0, n;
    bool                   eof = false;
    const void *           cur_key, *cur_val;
    size_t                 cur_klen, cur_vlen;
    char                   buff_1[16], buff_2[16];
    hse_err_t              err;
    struct hse_kvs_cursor *cursor;
    struct hse_kvs_cursor *duplicate_cursor;
    struct hse_kvs_cursor *yet_another_cursor;

    /* TC: A KVS can have multiple cursors reading at different rates */
    err = hse_kvs_cursor_create(kvs_handle, 0, NULL, NULL, 0, &cursor);
    ASSERT_EQ(err, 0);
    err = hse_kvs_cursor_create(kvs_handle, 0, NULL, NULL, 0, &duplicate_cursor);
    ASSERT_EQ(err, 0);

    /* Fast Cursor */
    err = hse_kvs_cursor_read(cursor, 0, &cur_key, &cur_klen, &cur_val, &cur_vlen, &eof);
    ASSERT_EQ(err, 0);
    err = hse_kvs_cursor_read(cursor, 0, &cur_key, &cur_klen, &cur_val, &cur_vlen, &eof);
    ASSERT_EQ(err, 0);
    n = snprintf(buff_1, sizeof(buff_1), "%.*s", (int)cur_klen, (char *)cur_key);
    ASSERT_LT(n, sizeof(buff_1));

    /* Slow Cursor */
    err =
        hse_kvs_cursor_read(duplicate_cursor, 0, &cur_key, &cur_klen, &cur_val, &cur_vlen, &eof);
    ASSERT_EQ(err, 0);
    n = snprintf(buff_2, sizeof(buff_2), "%.*s", (int)cur_klen, (char *)cur_key);
    ASSERT_LT(n, sizeof(buff_2));

    ASSERT_STRNE(buff_1, buff_2);

    /* TC: A KVS with multiple cursors must update each cursor explicitly */
    err = hse_kvs_cursor_create(kvs_handle, 0, NULL, NULL, 0, &yet_another_cursor);
    ASSERT_EQ(err, 0);

    err = hse_kvs_put(kvs_handle, 0, NULL, "extra_key", 9, "extra_value", 11);
    ASSERT_EQ(err, 0);

    err = hse_kvs_cursor_update_view(cursor, 0);
    ASSERT_EQ(err, 0);

    eof = false;
    while (!eof) {
        hse_kvs_cursor_read(cursor, 0, &cur_key, &cur_klen, &cur_val, &cur_vlen, &eof);

        if (!eof) {
            count_1++;
        }
    }

    eof = false;
    while (!eof) {
        hse_kvs_cursor_read(yet_another_cursor, 0, &cur_key, &cur_klen, &cur_val, &cur_vlen, &eof);

        if (!eof) {
            count_2++;
        }
    }

    ASSERT_NE(count_1, count_2);

    err = hse_kvs_cursor_destroy(cursor);
    ASSERT_EQ(err, 0);
    err = hse_kvs_cursor_destroy(duplicate_cursor);
    ASSERT_EQ(err, 0);
    err = hse_kvs_cursor_destroy(yet_another_cursor);
    ASSERT_EQ(err, 0);
}

MTF_END_UTEST_COLLECTION(cursor_api_test)

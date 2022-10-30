#!/usr/bin/env bash

# SPDX-License-Identifier: Apache-2.0
#
# Copyright (C) 2021 Micron Technology, Inc. All rights reserved.

. common.subr

output=$(cmd hse storage info -h)

echo "$output" | cmd grep -F "Usage: hse storage info [options] <kvdb_home>"
echo "$output" | cmd grep -F "Options:"
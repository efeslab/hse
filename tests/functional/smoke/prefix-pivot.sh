#!/usr/bin/env bash

. common.subr

# SPDX-License-Identifier: Apache-2.0
#
# Copyright (C) 2021 Micron Technology, Inc. All rights reserved.

# For this test to work, the value of (keys * vlen) should be about half the
# value of (node_size * fanout).  This should result in kvsets spilling to
# just below the pivot but no deeper.

node_size=100    # MiB
keys=200000
vlen=1000
fanout=4
prefix=xxxx

rparams=(
    kvdb-oparms
    log_squelch_ns=0
    log_lvl=5
    csched_debug_mask=0x60
    csched_rspill_params=0x0104
    csched_ispill_params=0x0104
    csched_leaf_pct=99

    kvs-oparms
    cn_close_wait=true
    cn_node_size_lo="$node_size"
    cn_node_size_hi="$node_size"
)

for pivot in 0 1 2 3 4; do

    if (( pivot == 2 )); then
        # default
        kvs=$(kvs_create fanout="$fanout" prefix.length=4) || err
    else
        kvs=$(kvs_create fanout="$fanout" prefix.length=4 prefix.pivot="$pivot") || err
    fi

    cmd kmt "$home" "$kvs" -s1 -j32 -f "${prefix}%023lx" "-i$keys" "-l$vlen" "${rparams[@]}"

    # Run this a few times to finish off compaction.  Calling repeatedly with
    # cn_close_wait=true is better than calling once with a longer wait b/c each
    # call will spill to the next level regardless of how long it takes.
    for (( i = 0; i < 5; i++ )); do
        cmd putbin "$home" "$kvs" -n 1000 "${rparams[@]}"
    done

    output=$(cmd cn_metrics "$home" "$kvs")

    level=$((pivot + 1))
    count=$(grep -c "^n $level," <<<"$output")
    if [[ "$count" != "$fanout" ]]; then
        err "Expected $fanout nodes at level $level, found $count nodes."
    fi

    level=$((pivot + 1))
    count=$(grep -c "^n $level," <<<"$output")
    if [[ "$count" != "$fanout" ]]; then
        err "Expected $fanout nodes at level $level, found $count nodes."
    fi

    # scan and count keys
    output=$(cmd pscan "$home" "$kvs" "-p$prefix" -c)
    cmd grep -P "^$keys\\b" <<<"$output"
done

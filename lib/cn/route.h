/* SPDX-License-Identifier: Apache-2.0 */
/*
 * Copyright (C) 2022 Micron Technology, Inc.  All rights reserved.
 */

#ifndef HSE_ROUTE_H
#define HSE_ROUTE_H

struct route_map;
struct kvs_cparams;

struct route_map *
route_map_create(const struct kvs_cparams *cp, const char *kvsname);

void
route_map_destroy(struct route_map *map);

uint
route_map_lookup(struct route_map *map, const void *pfx, uint pfxlen);

#endif

#pragma once

#include <stdint.h>
#include <stddef.h>

extern int subprocess_start(int (*subprocess_function)(void));
extern void *subprocess_create_share(size_t data_len);
extern void subprocess_delete_share(void *share, size_t data_len);


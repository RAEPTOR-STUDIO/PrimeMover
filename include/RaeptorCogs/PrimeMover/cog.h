#pragma once
#include <RaeptorContainers/dynamic_array.h>
typedef struct cog {
  void *handle;
} cog_t;

typedef darray(cog_t) cogs_t;

int cogs_free(cogs_t *cogs);
int cogs_load_cog(cogs_t *cogs, const char *path);

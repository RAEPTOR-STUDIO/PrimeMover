#include <RaeptorCogs/PrimeMover/cog.h>
#include <RaeptorCogs/PrimeMover/function_registry.h>
#include <RaeptorCogs/PrimeMover/logger.h>
#include <dlfcn.h>

int cogs_free(cogs_t *cogs) {
  for (size_t i = 0; i < cogs->count; i++) {
    dlclose(cogs->items[i].handle);
  }
  darray_free(cogs);
  return 0;
}

int cogs_load_cog(cogs_t *cogs, const char *path) {
  // Load the library
  struct cog cog;
  cog.handle = dlopen(path, RTLD_LAZY);
  if (!cog.handle) {
    logger_logf(stderr, "dlopen: %s", dlerror());
    return 1;
  }

  // Get a specific function
  const char *(*cog_name)(void);
  const char *(*cog_version)(void);
  void *(*logger_log_replace)(FILE *, const char *);

  *(void **)(&cog_name) = dlsym(cog.handle, "cog_name");
  *(void **)(&cog_version) = dlsym(cog.handle, "cog_version");

  char *error = dlerror();
  if (error) {
    logger_logf(stderr, "dlsym: %s", error);
    dlclose(cog.handle);
    return 1;
  }

  *(void **)(&logger_log_replace) = dlsym(cog.handle, "logger_log");
  if (dlerror() == NULL) {
    function_registry_register(fn_registry, 0, logger_log_replace);
  }

  // Call it
  logger_logf(stdout, "Loaded %s - %s", cog_name(), cog_version());

  darray_push(cogs, cog);
  return 0;
}

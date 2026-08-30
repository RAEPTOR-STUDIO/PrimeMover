#include <RaeptorCogs/Cog/cog.h>
#include <RaeptorCogs/Cog/function_registry.h>
#include <RaeptorCogs/PrimeMover/cog.h>
#include <RaeptorCogs/PrimeMover/logger.h>
#include <dlfcn.h>
#include <string.h>

int cogs_free(cogs_t *cogs) {
  for (size_t i = 0; i < cogs->count; i++) {
    void (*cog_on_detach)(void) = dlsym(cogs->items[i].handle, "cog_on_detach");
    if (dlerror() == NULL) {
      cog_on_detach();
    }
    dlclose(cogs->items[i].handle);
  }
  RC_darray_free(cogs);
  return 0;
}

int cogs_compute_cog_name(const char *name, char **out) {
  const char *so_ext = ".so";
  const char *cogs_dir = "./cogs/";
  const char *lib_prefix = "lib";
  size_t name_len = strlen(name);
  size_t name_with_ext_len = strlen(cogs_dir) + name_len + 1 +
                             strlen(lib_prefix) + name_len + strlen(so_ext) + 1;
  *out = malloc(name_with_ext_len);
  if (!*out) {
    logger_logf(stderr, "malloc failed");
    return 1;
  }
  strcpy(*out, cogs_dir);
  strcat(*out, name);
  strcat(*out, "/");
  strcat(*out, lib_prefix);
  strcat(*out, name);
  strcat(*out, so_ext);
  return 0;
}

int cogs_load_cog(cogs_t *cogs, const char *name) {
  // Load the library
  struct cog cog;
  char *real_name;
  if (cogs_compute_cog_name(name, &real_name)) {
    logger_logf(stderr, "cogs_compute_cog_name failed");
    return 1;
  }
  cog.handle = dlopen(real_name, RTLD_LAZY);
  if (!cog.handle) {
    free(real_name);
    logger_logf(stderr, "dlopen: %s", dlerror());
    return 1;
  }

  // Get a specific function
  const char *(*cog_name)(void);
  const char *(*cog_version)(void);
  void *(*logger_log_replace)(FILE *, const char *);

  *(void **)(&cog_name) = dlsym(cog.handle, "cog_name");
  *(void **)(&cog_version) = dlsym(cog.handle, "cog_version");
  shared_context_t **shared_context_ptr = dlsym(cog.handle, "shared_context");

  // Check if not already loaded
  for (size_t i = 0; i < cogs->count; i++) {
    const char *(*loaded_cog_name)(void);
    *(void **)(&loaded_cog_name) = dlsym(cogs->items[i].handle, "cog_name");
    if (dlerror() != NULL) {
      logger_logf(stderr, "dlsym: %s", dlerror());
      return 1;
    }
    if (strcmp(loaded_cog_name(), cog_name()) == 0) {
      logger_logf(stderr, "Cog %s already loaded", cog_name());
      free(real_name);
      dlclose(cog.handle);
      return 1;
    }
  }

  char *error = dlerror();
  if (error) {
    free(real_name);
    logger_logf(stderr, "dlsym: %s", error);
    dlclose(cog.handle);
    return 1;
  }

  *shared_context_ptr = shared_context;

  void (*cog_on_attach)(void) = dlsym(cog.handle, "cog_on_attach");
  if (dlerror() == NULL) {
    cog_on_attach();
  }

  // Call it
  logger_logf(stdout, "Loaded %s - %s", cog_name(), cog_version());

  RC_darray_push(cogs, cog);
  free(real_name);
  return 0;
}

int cogs_unload_cog(cogs_t *cogs, const char *name) {
  for (size_t i = 0; i < cogs->count; i++) {
    const char *(*cog_name)(void);
    *(void **)(&cog_name) = dlsym(cogs->items[i].handle, "cog_name");
    if (dlerror() != NULL) {
      logger_logf(stderr, "dlsym: %s", dlerror());
      return 1;
    }
    if (strcmp(cog_name(), name) == 0) {
      void (*cog_on_detach)(void) =
          dlsym(cogs->items[i].handle, "cog_on_detach");
      if (dlerror() == NULL) {
        cog_on_detach();
      }
      dlclose(cogs->items[i].handle);
      RC_darray_remove(cogs, i);
      logger_logf(stdout, "Unloaded %s", name);
      return 0;
    }
  }
  logger_logf(stderr, "Cog %s not found", name);
  return 1;
}

#include "RaeptorCogs/PrimeMover/logger.h"
#include <RaeptorCogs/Cog/cog.h>
#include <RaeptorCogs/Cog/function_registry.h>
#include <RaeptorCogs/PrimeMover/cog.h>
#include <dlfcn.h>

cogs_t cogs = {0};

void load_cog(const char *name) {
  if (cogs_load_cog(&cogs, name)) {
    logger_logf(stderr, "Failed to load cog: %s", name);
  }
}

void unload_cog(const char *name) {
  if (cogs_unload_cog(&cogs, name)) {
    logger_logf(stderr, "Failed to unload cog: %s", name);
  }
}

int cog_entry_point(int argc, const char **argv) {

  worker_init(&shared_context->workers.main_worker, pthread_self());
  shared_context->fn_registry.bind("logger_log", logger_log);
  shared_context->fn_registry.bind("logger_logf", logger_logf);
  shared_context->fn_registry.bind("load_cog", load_cog);
  shared_context->fn_registry.bind("unload_cog", unload_cog);
  if (argc > 1) {
    for (int i = 1; i < argc; i++) {
      cogs_load_cog(&cogs, argv[i]);
    }
  } else {
    logger_logf(stderr, "No cogs specified.");
  }

  worker_start(&shared_context->workers.main_worker);
  worker_free(&shared_context->workers.main_worker);
  cogs_free(&cogs);
  return 0;
}

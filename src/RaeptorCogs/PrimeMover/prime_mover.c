#include "RaeptorCogs/PrimeMover/logger.h"
#include <RaeptorCogs/Cog/function_registry.h>
#include <RaeptorCogs/PrimeMover/cog.h>
#include <dlfcn.h>

int cog_entry_point(int argc, const char **argv) {
  cogs_t cogs = {0};

  fn_registry->bind("logger_log", logger_log_fallback);
  if (argc > 1) {
    for (int i = 1; i < argc; i++) {
      cogs_load_cog(&cogs, argv[i]);
    }
  } else {
    logger_logf(stderr, "No cogs specified.");
  }
  cogs_free(&cogs);

  return 0;
}

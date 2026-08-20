#include <RaeptorCogs/PrimeMover/function_registry.h>
#include <RaeptorCogs/PrimeMover/logger.h>

function_registry_t fn_registry = {
    logger_log_fallback,
};

void function_registry_register(void(**fn_registry), size_t index, void(*f)) {
  fn_registry[index] = f;
}
void(*function_registry_get(size_t index)) { return fn_registry[index]; }

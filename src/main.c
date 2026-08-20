#include <RaeptorCogs/PrimeMover/cog.h>

int main(void) {
  cogs_t cogs = {0};

  cogs_load_cog(&cogs, "./cogs/libExampleCog.so");
  cogs_free(&cogs);

  return 0;
}

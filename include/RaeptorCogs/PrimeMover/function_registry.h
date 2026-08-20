#pragma once
#include <RaeptorContainers/dynamic_array.h>
typedef void(*function_registry_t[]);

void function_registry_register(function_registry_t fn_registry, size_t index,
                                void(*f));
void(*function_registry_get(size_t index));

extern function_registry_t fn_registry;

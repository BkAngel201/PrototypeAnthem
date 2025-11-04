#pragma once

#include "CoreMinimal.h"

/**
 * ProtoDebug.h
 * 
 * Centralized debug utilities and log definitions for the PrototypeAnthem project.
 * Place this file inside Source/PrototypeAnthem/Debug/
 */

// 🔹 Declare your main project log category (defined in ProtoDebug.cpp)
DECLARE_LOG_CATEGORY_EXTERN(LogProto, Log, All);

// 🔹 Network context helper macro
#define NET_LOG(FuncName) \
UE_LOG(LogProto, Warning, TEXT("[%s] %s executed on %s"), \
HasAuthority() ? TEXT("SERVER") : TEXT("CLIENT"), \
TEXT(FuncName), \
*GetNameSafe(this))

// 🔹 General debug log (compiled out in shipping builds)
#if !UE_BUILD_SHIPPING
#define DEBUG_LOG(Format, ...) UE_LOG(LogProto, Warning, TEXT(Format), ##__VA_ARGS__)
#else
#define DEBUG_LOG(Format, ...)
#endif

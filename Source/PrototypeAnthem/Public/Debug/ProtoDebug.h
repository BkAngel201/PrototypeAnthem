#pragma once

#include "CoreMinimal.h"

/**
 * ProtoDebug.h
 * 
 * Centralized debug utilities and log definitions for the PrototypeAnthem project.
 * 
 * PURPOSE:
 * - Standardized logging across the project
 * - Network-aware logging (Server vs Client)
 * - Category-based logging for different systems
 * - Compiled out in Shipping builds for performance
 * 
 * USAGE EXAMPLES:
 * 
 * Basic logging:
 *   PROTO_LOG("Player joined the game");
 *   PROTO_WARN("Missing objective definition!");
 *   PROTO_ERROR("Critical: Failed to spawn mission!");
 * 
 * Network-aware logging:
 *   PROTO_NET_LOG("SelectRole");  // Shows [SERVER] or [CLIENT] automatically
 *   PROTO_NET_FUNC();              // Logs function entry with network context
 * 
 * System-specific logging:
 *   PROTO_MISSION_LOG("Starting countdown: %s", *MissionName.ToString());
 *   PROTO_OBJECTIVE_LOG("Objective %d completed", ObjectiveIndex);
 * 
 * State transitions:
 *   PROTO_STATE_CHANGE("MissionState", "Countdown", "Active");
 * 
 * Authority checks:
 *   PROTO_AUTH_CHECK("SetReady");  // Logs if authority check passes/fails
 */

// ════════════════════════════════════════════════════════════════════════════
// LOG CATEGORY DECLARATION
// ════════════════════════════════════════════════════════════════════════════

DECLARE_LOG_CATEGORY_EXTERN(LogProto, Log, All);

// ════════════════════════════════════════════════════════════════════════════
// BASIC LOGGING MACROS (Always available in non-shipping builds)
// ════════════════════════════════════════════════════════════════════════════

#if !UE_BUILD_SHIPPING

// Basic severity levels
#define PROTO_LOG(Format, ...) \
    UE_LOG(LogProto, Log, TEXT(Format), ##__VA_ARGS__)

#define PROTO_WARN(Format, ...) \
    UE_LOG(LogProto, Warning, TEXT("⚠️ " Format), ##__VA_ARGS__)

#define PROTO_ERROR(Format, ...) \
    UE_LOG(LogProto, Error, TEXT("❌ " Format), ##__VA_ARGS__)

#define PROTO_SUCCESS(Format, ...) \
    UE_LOG(LogProto, Log, TEXT("✅ " Format), ##__VA_ARGS__)

#else
    #define PROTO_LOG(Format, ...)
    #define PROTO_WARN(Format, ...)
    #define PROTO_ERROR(Format, ...)
    #define PROTO_SUCCESS(Format, ...)
#endif

// ════════════════════════════════════════════════════════════════════════════
// NETWORK-AWARE LOGGING (Shows SERVER vs CLIENT context)
// ════════════════════════════════════════════════════════════════════════════

#if !UE_BUILD_SHIPPING

// Network context with custom message
#define PROTO_NET_LOG(Format, ...) \
    UE_LOG(LogProto, Warning, TEXT("[%s] " Format), \
        HasAuthority() ? TEXT("SERVER") : TEXT("CLIENT"), ##__VA_ARGS__)

// Network context for function entry (uses __FUNCTION__ automatically)
#define PROTO_NET_FUNC() \
    UE_LOG(LogProto, Warning, TEXT("[%s] %s() | Object: %s"), \
        HasAuthority() ? TEXT("SERVER") : TEXT("CLIENT"), \
        ANSI_TO_TCHAR(__FUNCTION__), \
        *GetNameSafe(this))

// Authority check logging - logs if check passes or fails
#define PROTO_AUTH_CHECK(FuncName) \
    if (!HasAuthority()) { \
        UE_LOG(LogProto, Warning, TEXT("[CLIENT] %s() early exit - no authority"), TEXT(FuncName)); \
        return; \
    } \
    UE_LOG(LogProto, Log, TEXT("[SERVER] %s() executing with authority"), TEXT(FuncName));

// Authority check with return value
#define PROTO_AUTH_CHECK_RETURN(FuncName, ReturnValue) \
    if (!HasAuthority()) { \
        UE_LOG(LogProto, Warning, TEXT("[CLIENT] %s() early exit - no authority"), TEXT(FuncName)); \
        return ReturnValue; \
    } \
    UE_LOG(LogProto, Log, TEXT("[SERVER] %s() executing with authority"), TEXT(FuncName));

#else
    #define PROTO_NET_LOG(Format, ...)
    #define PROTO_NET_FUNC()
    #define PROTO_AUTH_CHECK(FuncName) if (!HasAuthority()) return;
    #define PROTO_AUTH_CHECK_RETURN(FuncName, ReturnValue) if (!HasAuthority()) return ReturnValue;
#endif

// ════════════════════════════════════════════════════════════════════════════
// SYSTEM-SPECIFIC LOGGING (Helps filter logs by system)
// ════════════════════════════════════════════════════════════════════════════

#if !UE_BUILD_SHIPPING

#define PROTO_MISSION_LOG(Format, ...) \
    UE_LOG(LogProto, Log, TEXT("[MISSION] " Format), ##__VA_ARGS__)

#define PROTO_OBJECTIVE_LOG(Format, ...) \
    UE_LOG(LogProto, Log, TEXT("[OBJECTIVE] " Format), ##__VA_ARGS__)

#define PROTO_UI_LOG(Format, ...) \
    UE_LOG(LogProto, Log, TEXT("[UI] " Format), ##__VA_ARGS__)

#define PROTO_LOBBY_LOG(Format, ...) \
    UE_LOG(LogProto, Log, TEXT("[LOBBY] " Format), ##__VA_ARGS__)

#define PROTO_ABILITY_LOG(Format, ...) \
    UE_LOG(LogProto, Log, TEXT("[ABILITY] " Format), ##__VA_ARGS__)

#define PROTO_COMBAT_LOG(Format, ...) \
    UE_LOG(LogProto, Log, TEXT("[COMBAT] " Format), ##__VA_ARGS__)

#else
    #define PROTO_MISSION_LOG(Format, ...)
    #define PROTO_OBJECTIVE_LOG(Format, ...)
    #define PROTO_UI_LOG(Format, ...)
    #define PROTO_LOBBY_LOG(Format, ...)
    #define PROTO_ABILITY_LOG(Format, ...)
    #define PROTO_COMBAT_LOG(Format, ...)
#endif

// ════════════════════════════════════════════════════════════════════════════
// STATE TRANSITION LOGGING (Before → After)
// ════════════════════════════════════════════════════════════════════════════

#if !UE_BUILD_SHIPPING

#define PROTO_STATE_CHANGE(StateName, OldState, NewState) \
    UE_LOG(LogProto, Log, TEXT("🔄 %s: %s → %s"), TEXT(StateName), TEXT(OldState), TEXT(NewState))

#define PROTO_STATE_CHANGE_ENUM(StateName, OldState, NewState) \
    UE_LOG(LogProto, Log, TEXT("🔄 %s: %d → %d"), TEXT(StateName), (uint8)OldState, (uint8)NewState)

#else
    #define PROTO_STATE_CHANGE(StateName, OldState, NewState)
    #define PROTO_STATE_CHANGE_ENUM(StateName, OldState, NewState)
#endif

// ════════════════════════════════════════════════════════════════════════════
// REPLICATION LOGGING (Track RepNotify execution)
// ════════════════════════════════════════════════════════════════════════════

#if !UE_BUILD_SHIPPING

#define PROTO_REP_LOG(PropertyName, NewValue) \
    UE_LOG(LogProto, Log, TEXT("🔁 OnRep_%s: %s"), TEXT(PropertyName), *FString::Printf(TEXT("%s"), NewValue))

#define PROTO_REP_LOG_ENUM(PropertyName, NewValue) \
    UE_LOG(LogProto, Log, TEXT("🔁 OnRep_%s: %d"), TEXT(PropertyName), (uint8)NewValue)

#else
    #define PROTO_REP_LOG(PropertyName, NewValue)
    #define PROTO_REP_LOG_ENUM(PropertyName, NewValue)
#endif

// ════════════════════════════════════════════════════════════════════════════
// TIMER VALIDATION (Check if timers are set correctly)
// ════════════════════════════════════════════════════════════════════════════

#if !UE_BUILD_SHIPPING

#define PROTO_TIMER_LOG(TimerName, Duration, bIsActive) \
    UE_LOG(LogProto, Log, TEXT("⏱️ Timer '%s' set for %.2f seconds | Active: %s"), \
        TEXT(TimerName), Duration, bIsActive ? TEXT("YES") : TEXT("NO"))

#else
    #define PROTO_TIMER_LOG(TimerName, Duration, bIsActive)
#endif

// ════════════════════════════════════════════════════════════════════════════
// NULL CHECK LOGGING (Defensive programming)
// ════════════════════════════════════════════════════════════════════════════

#if !UE_BUILD_SHIPPING

#define PROTO_NULL_CHECK(Pointer, PointerName) \
    if (!IsValid(Pointer)) { \
        UE_LOG(LogProto, Error, TEXT("❌ NULL CHECK FAILED: %s is invalid!"), TEXT(PointerName)); \
        return; \
    }

#define PROTO_NULL_CHECK_RETURN(Pointer, PointerName, ReturnValue) \
    if (!IsValid(Pointer)) { \
        UE_LOG(LogProto, Error, TEXT("❌ NULL CHECK FAILED: %s is invalid!"), TEXT(PointerName)); \
        return ReturnValue; \
    }

#else
    #define PROTO_NULL_CHECK(Pointer, PointerName) if (!IsValid(Pointer)) return;
    #define PROTO_NULL_CHECK_RETURN(Pointer, PointerName, ReturnValue) if (!IsValid(Pointer)) return ReturnValue;
#endif

// ════════════════════════════════════════════════════════════════════════════
// DEPRECATED/LEGACY MACROS (For backwards compatibility)
// ════════════════════════════════════════════════════════════════════════════

// These are your old macros - still work but prefer the new ones above
#define NET_LOG(FuncName) PROTO_NET_LOG("%s", TEXT(FuncName))
#define DEBUG_LOG(Format, ...) PROTO_LOG(Format, ##__VA_ARGS__)
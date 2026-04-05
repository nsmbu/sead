#pragma once

#include <basis/seadTypes.h>

#if defined(SEAD_TARGET_DEBUG)
    #define SEAD_ASSERT(condition)                                                                     \
        do                                                                                             \
        {                                                                                              \
            if (!(condition))                                                                          \
            {                                                                                          \
                sead::system::HaltWithDetailNoFormat(__FILE__, __LINE__, #condition);                  \
            }                                                                                          \
        } while (0)

    #define SEAD_ASSERT_MSG(condition, format, ...)                                                    \
        do                                                                                             \
        {                                                                                              \
            if (!(condition))                                                                          \
            {                                                                                          \
                sead::system::HaltWithDetail(__FILE__, __LINE__, format, ##__VA_ARGS__);               \
            }                                                                                          \
        } while (0)
#else
    #define SEAD_ASSERT(condition)                                                                     \
        do                                                                                             \
        {                                                                                              \
            if (false)                                                                                 \
            {                                                                                          \
                static_cast<void>(condition);                                                          \
            }                                                                                          \
        } while (0)

    #define SEAD_ASSERT_MSG(condition, format, ...)                                                    \
        do                                                                                             \
        {                                                                                              \
            if (false)                                                                                 \
            {                                                                                          \
                static_cast<void>(condition);                                                          \
                sead::system::HaltWithDetail(nullptr, 0, format, ##__VA_ARGS__);                       \
            }                                                                                          \
        } while (0)
#endif // SEAD_TARGET_DEBUG

namespace sead { namespace system {

void DebugBreak();
void Halt();
void HaltWithDetail(const char* pos, s32 line, const char* format, ...);
void HaltWithDetailNoFormat(const char* pos, s32 line, const char* str);

#if defined(SEAD_PLATFORM_WINDOWS)
void SetEnableExceptionOnHalt(bool enable);
#endif // SEAD_PLATFORM_WINDOWS

} } // namespace sead::system

#ifndef SEAD_CAFE_DEBUG_CONTROLLER_CAFE_H_
#define SEAD_CAFE_DEBUG_CONTROLLER_CAFE_H_

#include <controller/seadController.h>

namespace sead {

class CafeDebugController : public Controller
{
    SEAD_RTTI_OVERRIDE(CafeDebugController, Controller)

public:
    CafeDebugController(ControllerMgr* mgr, s32 channel);

    s32 getChannel() const { return mChannel; }

    bool isConnected() const override { return mIsConnected; }

private:
#if 0 // defined(SEAD_TARGET_DEBUG)
    void calcImpl_() override;
#else
    void calcImpl_() override { }
#endif

    void clampStickValue_(Vector2f* out, s8 x, s8 y);

private:
    s32 mChannel;
    bool mIsConnected;
};
static_assert(sizeof(CafeDebugController) == 0x164, "sead::CafeDebugController size mismatch");

} // namespace sead

#endif // SEAD_CAFE_DEBUG_CONTROLLER_CAFE_H_

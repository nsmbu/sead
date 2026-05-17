#ifndef SEAD_AUDIOSYSTEM_CAFE_H_
#define SEAD_AUDIOSYSTEM_CAFE_H_

#include <audio/seadAudioSystem.h>

namespace sead {

class AudioSystemCafe : public AudioSystem
{
    SEAD_RTTI_OVERRIDE(AudioSystemCafe, AudioSystem)

public:
    AudioSystemCafe();
    virtual ~AudioSystemCafe() { }

    void initialize() override;
    void finalize() override;
    bool setOutputMode(AudioGlobal::OutputMode) override;
    AudioGlobal::OutputMode getOutputMode() const override;
    bool appendEffect(AudioGlobal::AuxBus, AudioFx*) override;
    void clearEffect(AudioGlobal::AuxBus, s32) override;
    bool isFinishedClearEffect(AudioGlobal::AuxBus) override;
    void appendSoundFrameCallback(ISoundFrameCallback&) override;
    void removeSoundFrameCallback(ISoundFrameCallback&) override;
    void clearSoundFrameCallback() override;

protected:
    void initializeSdk_() override;
    void finalizeSdk_() override;
    void initializeNw_() override;

public:
    void forceQuit();
};

} // namespace sead

#endif // SEAD_AUDIOSYSTEM_CAFE_H_

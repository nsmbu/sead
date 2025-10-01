#ifndef SEAD_AUDIO_PLAYER_CAFE_H_
#define SEAD_AUDIO_PLAYER_CAFE_H_

#include <audio/seadAudioPlayer.h>
#include <thread/seadCriticalSection.h>

#include <nw/snd/snd_SoundArchivePlayer.h>

namespace sead {

class AudioRmtSpeakerMgrCafe;
class AudioSoundDataMgrCafe;
class AudioSoundHeapCafe;
class SoundHandle;

class AudioPlayerCafe : public AudioPlayer, public nw::snd::SoundArchivePlayer
{
    SEAD_RTTI_OVERRIDE(AudioPlayerCafe, AudioPlayer)

public:
    AudioPlayerCafe();
    virtual ~AudioPlayerCafe();

    void initialize() override;
    void finalize() override;
    void calc() override;
    bool startSound(SoundHandle*, u32) override;
    bool startSound(SoundHandle*, const char*) override;
    bool holdSound(SoundHandle*, u32) override;
    bool holdSound(SoundHandle*, const char*) override;
    u32 getSoundCount() const override;
    const char* getSoundName(u32) const override;
    u32 getSoundId(const char*) const override;

protected:
    nw::snd::SoundStartable::StartResult detail_SetupSound(nw::snd::SoundHandle* handle, u32 soundId, bool holdFlag, const nw::snd::SoundStartable::StartInfo* startInfo) override;

public:
    AudioSoundDataMgrCafe* getSoundDataMgr() const
    {
        return mSoundDataMgr;
    }

    AudioSoundHeapCafe* getSoundHeap() const
    {
        return mSoundHeap;
    }

    AudioRmtSpeakerMgrCafe* getRmtSpeakerMgr() const
    {
        return mRmtSpeakerMgr;
    }

protected:
    u8* mPlayerBuffer;
    u32 mPlayerBufferSize;
    u8* mStreamBuffer;
    u32 mStreamBufferSize;
    u32 _d8;
    u8* mStreamCacheBuffer;
    u32 mStreamCacheBufferSize;
    AudioSoundDataMgrCafe* mSoundDataMgr;
    AudioSoundHeapCafe* mSoundHeap;
    AudioRmtSpeakerMgrCafe* mRmtSpeakerMgr;
    u8 _f0;
    u8 _f1;
    CriticalSection mUpdateLock;
    bool mIsValidUpdateLock;
};
#ifdef cafe
static_assert(sizeof(AudioPlayerCafe) == 0x134, "sead::AudioPlayerCafe size mismatch");
#endif // cafe

} // namespace sead

#endif // SEAD_AUDIO_PLAYER_CAFE_H_

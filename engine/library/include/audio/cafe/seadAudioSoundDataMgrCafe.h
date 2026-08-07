#pragma once

#include <prim/seadSafeString.h>

namespace sead {

class AudioSoundHeapCafe;

// TODO
class AudioSoundDataMgrCafe // : public nw::snd::SoundDataManager
{
public:
    void setContentRootPath(const char* content_root_path);
    void mountSoundArchiveFromFs(const SafeString& path, Heap* heap, bool file_access_in_function, bool load_label_string);

    bool loadData(u32 id, u32 load_flag, u32 load_block_size, AudioSoundHeapCafe* sound_heap);
    bool loadData(const char* item_name, u32 load_flag, u32 load_block_size, AudioSoundHeapCafe* sound_heap);
};

} // namespace sead

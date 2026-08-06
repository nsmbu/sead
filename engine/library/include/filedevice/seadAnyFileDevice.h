#ifndef SEAD_ANY_FILEDEVICE_H_
#define SEAD_ANY_FILEDEVICE_H_

#include <container/seadPtrArray.h>
#include <filedevice/seadFileDevice.h>

namespace sead {

class AnyFileDevice : public FileDevice
{
    SEAD_RTTI_OVERRIDE(AnyFileDevice, FileDevice)

private:
    typedef PtrArray<FileDevice> FileDeviceList;

public:
    AnyFileDevice(s32 limit_num, Heap* heap, s32 alignment = 4);
    ~AnyFileDevice() override;

    bool pushBack(FileDevice* device);
    bool pushFront(FileDevice* device);
    bool erase(FileDevice* device);

    void traceFilePath(const SafeString& path) const override;
    void traceDirectoryPath(const SafeString& path) const override;
    void resolveFilePath(BufferedSafeString* out, const SafeString& path) const override;
    void resolveDirectoryPath(BufferedSafeString* out, const SafeString& path) const override;

    //? Added
    const FileDeviceList& getFileDeviceList() const { return mFileDeviceList; }

protected:
    bool doIsAvailable_() const override;
    u8* doLoad_(LoadArg& arg) override;

    FileDevice* findFileDeviceByFile_(const SafeString& path) const;
    FileDevice* findFileDeviceByDirectory_(const SafeString& path) const;

    FileDevice* doOpen_(FileHandle* handle, const SafeString& filename, FileOpenFlag flag) override;
    bool doClose_(FileHandle* handle) override;
    bool doRead_(u32* read_size, FileHandle* handle, u8* buf, u32 size) override;
    bool doWrite_(u32* write_size, FileHandle* handle, const u8* buf, u32 size) override;
    bool doSeek_(FileHandle* handle, s32 offset, SeekOrigin origin) override;
    bool doGetCurrentSeekPos_(u32* pos, FileHandle* handle) override;
    bool doGetFileSize_(u32* size, const SafeString& path) override;
    bool doGetFileSize_(u32* size, FileHandle* handle) override;
    bool doIsExistFile_(bool* is_exist, const SafeString& path) override;
    bool doIsExistDirectory_(bool* is_exist, const SafeString& path) override;
    FileDevice* doOpenDirectory_(DirectoryHandle* handle, const SafeString& dirname) override;
    bool doCloseDirectory_(DirectoryHandle* handle) override;
    bool doReadDirectory_(u32* read_num, DirectoryHandle* handle, DirectoryEntry* entry, u32 num) override;
    bool doMakeDirectory_(const SafeString& path, u32 permission) override;
    bool isMatchDevice_(const HandleBase* handle) const override;
    RawErrorCode doGetLastRawError_() const override;

protected:
    FileDeviceList mFileDeviceList;
    FileDevice* mLastFileDevice;
};
#ifdef cafe
static_assert(sizeof(AnyFileDevice) == 0x64, "sead::AnyFileDevice size mismatch");
#endif // cafe

} // namespace sead

#endif // SEAD_ANY_FILEDEVICE_H_

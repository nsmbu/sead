#ifndef SEAD_MAIN_FILEDEVICE_H_
#define SEAD_MAIN_FILEDEVICE_H_

#include <filedevice/seadFileDevice.h>
#include <prim/seadRuntimeTypeInfo.h>
#include <prim/seadSafeString.h>

namespace sead {

class MainFileDevice : public FileDevice
{
    SEAD_RTTI_OVERRIDE(MainFileDevice, FileDevice)

public:
    MainFileDevice(Heap* heap);
    virtual ~MainFileDevice();

    void traceFilePath(const SafeString& path) const override;
    void traceDirectoryPath(const SafeString& path) const override;
    void resolveFilePath(BufferedSafeString* out, const SafeString& path) const override;
    void resolveDirectoryPath(BufferedSafeString* out, const SafeString& path) const override;

protected:
    bool doIsAvailable_() const override
    {
        return mFileDevice->isAvailable();
    }

    FileDevice* doOpen_(FileHandle* handle, const SafeString& filename, FileOpenFlag flag) override
    {
        return mFileDevice->tryOpen(handle, filename, flag, handle->getDivSize());
    }

    bool doClose_(FileHandle* handle) override
    {
        return mFileDevice->tryClose(handle);
    }

    bool doRead_(u32* read_size, FileHandle* handle, u8* buf, u32 size) override
    {
        return mFileDevice->tryRead(read_size, handle, buf, size);
    }

    bool doWrite_(u32* write_size, FileHandle* handle, const u8* buf, u32 size) override
    {
        return mFileDevice->tryWrite(write_size, handle, buf, size);
    }

    bool doSeek_(FileHandle* handle, s32 offset, SeekOrigin origin) override
    {
        return mFileDevice->trySeek(handle, offset, origin);
    }

    bool doGetCurrentSeekPos_(u32* pos, FileHandle* handle) override
    {
        return mFileDevice->tryGetCurrentSeekPos(pos, handle);
    }

    bool doGetFileSize_(u32* size, const SafeString& path) override
    {
        return mFileDevice->tryGetFileSize(size, path);
    }

    bool doGetFileSize_(u32* size, FileHandle* handle) override
    {
        return mFileDevice->tryGetFileSize(size, handle);
    }

    bool doIsExistFile_(bool* is_exist, const SafeString& path) override
    {
        return mFileDevice->tryIsExistFile(is_exist, path);
    }

    bool doIsExistDirectory_(bool* is_exist, const SafeString& path) override
    {
        return mFileDevice->tryIsExistDirectory(is_exist, path);
    }

    FileDevice* doOpenDirectory_(DirectoryHandle* handle, const SafeString& dirname) override
    {
        return mFileDevice->tryOpenDirectory(handle, dirname);
    }

    bool doCloseDirectory_(DirectoryHandle* handle) override
    {
        return mFileDevice->tryCloseDirectory(handle);
    }

    bool doReadDirectory_(u32* read_num, DirectoryHandle* handle, DirectoryEntry* entry, u32 num) override
    {
        return mFileDevice->tryReadDirectory(read_num, handle, entry, num);
    }

    bool doMakeDirectory_(const SafeString& path, u32 permission) override
    {
        return mFileDevice->tryMakeDirectory(path, permission);
    }

    bool isMatchDevice_(const HandleBase* handle) const override
    {
        return mFileDevice->isMatchDevice_(handle);
    }

    RawErrorCode doGetLastRawError_() const override
    {
        return mFileDevice->getLastRawError();
    }

protected:
    FileDevice* mFileDevice;
};
#ifdef cafe
static_assert(sizeof(MainFileDevice) == 0x58, "sead::MainFileDevice size mismatch");
#endif // cafe

} // namespace sead

#endif // SEAD_MAIN_FILEDEVICE_H_

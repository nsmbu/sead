#ifndef SEAD_SHARC_ARCHIVE_RES_H_
#define SEAD_SHARC_ARCHIVE_RES_H_

#include <container/seadBuffer.h>
#include <filedevice/seadFileDevice.h>
#include <prim/seadEndian.h>
#include <prim/seadSafeString.h>
#include <resource/seadArchiveRes.h>

namespace sead {

class SharcArchiveRes : public ArchiveRes
{
    SEAD_RTTI_OVERRIDE(SharcArchiveRes, ArchiveRes)

public:
    static const u32 cArchiveVersion = 0x100;
    static const u32 cArchiveEntryMax = 0x3fff;
    static const u32 cFileNameTableAlign = 4;

public:
    struct ArchiveBlockHeader
    {
        char signature[4];
        u16 header_size;
        u16 byte_order;
        u32 file_size;
        u32 data_block_offset;
        u16 version;
        u16 reserved;
    };
    static_assert(sizeof(ArchiveBlockHeader) == 0x14,
                  "sead::SharcArchiveRes::ArchiveBlockHeader size mismatch");

    struct FATBlockHeader
    {
        char signature[4];
        u16 header_size;
        u16 file_num;
        u32 hash_key;
    };
    static_assert(sizeof(FATBlockHeader) == 0xC,
                  "sead::SharcArchiveRes::FATBlockHeader size mismatch");

    struct FATEntry
    {
        u32 hash;
        u32 name_offset;
        u32 data_start_offset;
        u32 data_end_offset;
    };
    static_assert(sizeof(FATEntry) == 0x10,
                  "sead::SharcArchiveRes::FATEntry size mismatch");

    struct FNTBlockHeader
    {
        char signature[4];
        u16 header_size;
        u16 reserved;
    };
    static_assert(sizeof(FNTBlockHeader) == 8,
                  "sead::SharcArchiveRes::FNTBlockHeader size mismatch");

public:
    SharcArchiveRes();
    virtual ~SharcArchiveRes();

protected:
    void* getFileImpl_(const SafeString& file_path, FileInfo* file_info) override;
    void* getFileFastImpl_(s32 entry_id, FileInfo* file_info) override;
    s32 convertPathToEntryIDImpl_(const SafeString& file_path) override;
    bool setCurrentDirectoryImpl_(const SafeString& file_path) override;
    bool openDirectoryImpl_(HandleBuffer* handle_buffer, const SafeString& dir_path) override;
    bool closeDirectoryImpl_(HandleBuffer* handle_buffer) override;
    u32 readDirectoryImpl_(HandleBuffer* handle_buffer, DirectoryEntry* entry, u32 num) override;
    bool prepareArchive_(const void* archive) override;

protected:
    const ArchiveBlockHeader* mArchiveBlockHeader;
    const FATBlockHeader* mFATBlockHeader;
    const char* mFNTBlock;
    Buffer<const FATEntry> mFATEntrys;
    const u8* mDataBlock;
    Endian::Types mEndianType;
};
#ifdef cafe
static_assert(sizeof(SharcArchiveRes) == 0x44, "sead::SharcArchiveRes size mismatch");
#endif // cafe

} // namespace sead

#endif // SEAD_SHARC_ARCHIVE_RES_H_

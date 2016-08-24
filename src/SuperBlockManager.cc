#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

#include "SuperBlockManager.h"

namespace kvdb{
    bool SuperBlockManager::InitSuperBlockForCreateDB()
    {
        m_superblock = new DBSuperBlock;

        memset(m_superblock, 0, sizeof(DBSuperBlock));
        return true;
    }

    bool SuperBlockManager::LoadSuperBlockFromDevice(uint64_t offset)
    {   
        m_superblock = new DBSuperBlock;
        
        uint64_t length = sizeof(DBSuperBlock);
        if ((uint64_t)m_bdev->pRead(m_superblock, length, offset) != length)
        {
            __ERROR("Could not read superblock from device\n"); 
            return false;
        }
        return true;
    }

    bool SuperBlockManager::WriteSuperBlockToDevice(uint64_t offset)
    {
        uint64_t length = sizeof(DBSuperBlock);
        if ((uint64_t)m_bdev->pWrite(m_superblock, length, offset) != length)
        {
            __ERROR("Could not write superblock at position %ld\n", offset);
            return false;
        }
        return true;
    }
    
    void SuperBlockManager::SetSuperBlock(DBSuperBlock& sb)
    {
        m_superblock->hashtable_size        = sb.hashtable_size;
        m_superblock->number_elements       = sb.number_elements;
        m_superblock->deleted_elements      = sb.deleted_elements;
        m_superblock->segment_size          = sb.segment_size;
        m_superblock->number_segments       = sb.number_segments;
        m_superblock->current_segment       = sb.current_segment;
        m_superblock->db_sb_size            = sb.db_sb_size;
        m_superblock->db_index_size         = sb.db_index_size;
        m_superblock->db_data_size          = sb.db_data_size;
        m_superblock->device_size           = sb.device_size;
    }


    uint64_t SuperBlockManager::GetSuperBlockSizeOnDevice()
    {
        uint64_t sb_size_pages = sizeof(DBSuperBlock) / getpagesize();
        return (sb_size_pages + 1) * getpagesize();
    }

    SuperBlockManager::SuperBlockManager(BlockDevice* bdev):
        m_bdev(bdev), m_superblock(NULL)
    {
        return;
    }

    SuperBlockManager::~SuperBlockManager()
    {
        if (m_superblock)
        {
            delete m_superblock;
        }
    }

} // namespace kvdb

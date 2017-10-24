#ifndef _HLKVDS_METASTOR_H_
#define _HLKVDS_METASTOR_H_

#include <string>

#include "hlkvds/Options.h"

using namespace std;
namespace hlkvds {

class BlockDevice;
class SuperBlockManager;
class IndexManager;
class SimpleDS_Impl;

class MetaStor {
public:
    bool CreateMetaData();
    bool LoadMetaData();
    bool PersistMetaData();

    bool PersistIndexToDevice();
    bool PersistSSTsToDevice();
    bool FastRecovery();

    MetaStor(const char* paths, BlockDevice *dev, SuperBlockManager *sbm, IndexManager *im, SimpleDS_Impl *ds, Options &opt);
    ~MetaStor();

private:
    bool LoadSuperBlock(int first_create = 0);
    bool LoadIndex(uint32_t ht_size, uint64_t index_size, int first_create = 0);
    bool LoadSSTs(uint32_t segment_size, uint32_t number_segments,int first_create = 0);

    bool PersistSuperBlockToDevice();

private:
    const char* paths_;
    BlockDevice *metaDev_;
    SuperBlockManager *sbMgr_;
    IndexManager* idxMgr_;
    SimpleDS_Impl* dataStor_;
    Options &options_;

    int64_t sbOff_;
    int64_t idxOff_;
    int64_t sstOff_;
};
    
}// namespace hlkvds

#endif //#ifndef _HLKVDS_METASTOR_H_

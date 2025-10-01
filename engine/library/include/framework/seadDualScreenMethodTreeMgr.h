#ifndef SEAD_DUALSCREEN_METHODTREEMGR_H_
#define SEAD_DUALSCREEN_METHODTREEMGR_H_

#include <framework/seadMethodTree.h>
#include <framework/seadMethodTreeMgr.h>

namespace sead {

class DualScreenMethodTreeMgr : public MethodTreeMgr
{
    SEAD_RTTI_OVERRIDE(DualScreenMethodTreeMgr, MethodTreeMgr)

public:
    enum Screen
    {
        cScreen_Top = 0,
        cScreen_Btm,
        cScreen_Num
    };

public:
    DualScreenMethodTreeMgr();
    virtual ~DualScreenMethodTreeMgr();

    void attachMethod(s32, MethodTreeNode*) override;
    MethodTreeNode* getRootMethodTreeNode(s32) override;
    void pauseAll(bool) override;
    void pauseAppCalc(bool) override;

    void calc();
    void drawTop();
    void drawBtm();

protected:
    MethodTreeNode mRootCalcNode;
    MethodTreeNode mSysCalcNode;
    MethodTreeNode mAppCalcNode;
    MethodTreeNode mTopRootDrawNode;
    MethodTreeNode mTopSysDrawNode;
    MethodTreeNode mTopAppDrawNode;
    MethodTreeNode mTopAppDrawFinalNode;
    MethodTreeNode mBtmRootDrawNode;
    MethodTreeNode mBtmSysDrawNode;
    MethodTreeNode mBtmAppDrawNode;
    MethodTreeNode mBtmAppDrawFinalNode;
    Screen mSysDrawScreen;
    Screen mAppDrawScreen;
};
#ifdef cafe
static_assert(sizeof(DualScreenMethodTreeMgr) == 0x3E4, "sead::DualScreenMethodTreeMgr size mismatch");
#endif // cafe

} // namespace sead

#endif // SEAD_DUALSCREEN_METHODTREEMGR_H_

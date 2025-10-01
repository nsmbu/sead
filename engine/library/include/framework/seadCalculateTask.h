#ifndef SEAD_CALCULATE_TASK_H_
#define SEAD_CALCULATE_TASK_H_

#include <framework/seadMethodTree.h>
#include <framework/seadTaskBase.h>

namespace sead {

class CalculateTask : public TaskBase
{
    SEAD_RTTI_OVERRIDE(CalculateTask, TaskBase)

public:
    CalculateTask(const TaskConstructArg& arg);
    CalculateTask(const TaskConstructArg& arg, const char* name);

    void pauseCalc(bool b) override;
    void pauseDraw(bool b) override;
    void pauseCalcRec(bool b) override;
    void pauseDrawRec(bool b) override;
    void pauseCalcChild(bool b) override;
    void pauseDrawChild(bool b) override;

    virtual ~CalculateTask();

    void attachCalcImpl() override;
    void attachDrawImpl() override;
    void detachCalcImpl() override;
    void detachDrawImpl() override;
    const RuntimeTypeInfo::Interface* getCorrespondingMethodTreeMgrTypeInfo() const override;
    MethodTreeNode* getMethodTreeNode(s32 method_type) override;

    virtual void calc();

    MethodTreeNode mCalcNode;
};
#ifdef cafe
static_assert(sizeof(CalculateTask) == 0xC8, "sead::CalculateTask size mismatch");
#endif // cafe

} // namespace sead

#endif // SEAD_CALCULATE_TASK_H_

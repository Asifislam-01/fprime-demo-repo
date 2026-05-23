// ======================================================================
// \title  HiEchoComponent.cpp
// \author asifislam
// \brief  cpp file for HiEchoComponent component implementation class
// ======================================================================

#include "HiNamespace/Components/HiEchoComponent/HiEchoComponent.hpp"

namespace HiNamespace {

HiEchoComponent ::HiEchoComponent(const char* const compName) : HiEchoComponentComponentBase(compName) {}

HiEchoComponent ::~HiEchoComponent() {}

void HiEchoComponent ::greetingIn_handler(FwIndexType portNum, U32 code) {
    (void)portNum;
    this->log_ACTIVITY_HI_MessageReceived(code);
    this->tlmWrite_MessageCount(++this->m_messageCount);
}

}  // namespace HiNamespace

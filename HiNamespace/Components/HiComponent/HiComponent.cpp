// ======================================================================
// \title  HiComponent.cpp
// \author asifislam
// \brief  cpp file for HiComponent component implementation class
// ======================================================================

#include "HiNamespace/Components/HiComponent/HiComponent.hpp"

namespace HiNamespace {

// ----------------------------------------------------------------------
// Component construction and destruction
// ----------------------------------------------------------------------

HiComponent ::HiComponent(const char* const compName) : HiComponentComponentBase(compName) {}

HiComponent ::~HiComponent() {}

// ----------------------------------------------------------------------
// Handler implementations for commands
// ----------------------------------------------------------------------

void HiComponent ::SAY_HI_cmdHandler(FwOpcodeType opCode, U32 cmdSeq, const Fw::CmdStringArg& greeting) {
    Fw::LogStringArg eventGreeting(greeting.toChar());
    this->log_ACTIVITY_HI_SayHiEvent(eventGreeting);

    this->tlmWrite_GreetingCount(++this->m_greetingCount);

    this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
}

void HiComponent ::SEND_TO_PEER_cmdHandler(FwOpcodeType opCode, U32 cmdSeq, U32 code) {
    // Port-to-port call: topology connects greetingOut -> hiEchoInstance.greetingIn
    this->greetingOut_out(0, code);
    this->cmdResponse_out(opCode, cmdSeq, Fw::CmdResponse::OK);
}

}  // namespace HiNamespace

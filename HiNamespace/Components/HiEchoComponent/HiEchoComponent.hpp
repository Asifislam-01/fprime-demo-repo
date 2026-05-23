// ======================================================================
// \title  HiEchoComponent.hpp
// \author asifislam
// \brief  hpp file for HiEchoComponent component implementation class
// ======================================================================

#ifndef HiNamespace_HiEchoComponent_HPP
#define HiNamespace_HiEchoComponent_HPP

#include "HiNamespace/Components/HiEchoComponent/HiEchoComponentComponentAc.hpp"

namespace HiNamespace {

class HiEchoComponent final : public HiEchoComponentComponentBase {
  public:
    HiEchoComponent(const char* const compName);

    ~HiEchoComponent();

  private:
    void greetingIn_handler(FwIndexType portNum, U32 code) override;

    U32 m_messageCount = 0;
};

}  // namespace HiNamespace

#endif

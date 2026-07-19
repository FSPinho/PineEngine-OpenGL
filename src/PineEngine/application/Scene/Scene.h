#pragma once

#include <PineEngine/util/Resource/Resource.h>

namespace PineEngine {
class Scene : public Resource {
  public:
    explicit Scene(const Path &path);

  protected:
    void performLoad() override;
    void performUnload() override;

  private:
};
} // namespace PineEngine
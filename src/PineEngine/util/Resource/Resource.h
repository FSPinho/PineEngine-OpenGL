#pragma once

#include <string>
#include <PineEngine/util/Path/Path.h>

namespace PineEngine {
class Resource {
  public:
    explicit Resource(const Path &path);
    virtual ~Resource();

    virtual void load();
    virtual void unload();

    const Path &getPath();

  protected:
    virtual void performLoad() = 0;
    virtual void performUnload() = 0;

  private:
    Path path;
};
} // namespace PineEngine

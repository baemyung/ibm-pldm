#pragma once

#include "serialize.hpp"

#include <sdbusplus/bus.hpp>
#include <sdbusplus/server.hpp>
#include <sdbusplus/server/object.hpp>
#include <xyz/openbmc_project/Inventory/Item/FabricAdapter/server.hpp>

#include <string>

namespace pldm
{
namespace dbus
{
using ItemFabricAdapter = sdbusplus::server::object_t<
    sdbusplus::xyz::openbmc_project::Inventory::Item::server::FabricAdapter>;

class FabricAdapter : public ItemFabricAdapter
{
  public:
    FabricAdapter() = delete;
    ~FabricAdapter() = default;
    FabricAdapter(const FabricAdapter&) = delete;
    FabricAdapter& operator=(const FabricAdapter&) = delete;
    FabricAdapter(FabricAdapter&&) = default;
    FabricAdapter& operator=(FabricAdapter&&) = default;

    FabricAdapter(sdbusplus::bus_t& bus, const std::string& objPath) :
        ItemFabricAdapter(bus, objPath.c_str())
    {
        pldm::serialize::Serialize::getSerialize().serialize(objPath,
                                                             "FabricAdapter");
    }
};

} // namespace dbus
} // namespace pldm

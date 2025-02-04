#include "../dbus/custom_dbus.hpp"

#include <gtest/gtest.h>

using namespace pldm::dbus;
TEST(CustomDBus, LocationCode)
{
    std::string tmpPath = "/abc/def";
    std::string locationCode = "testLocationCode";

    CustomDBus::getCustomDBus().setLocationCode(tmpPath, locationCode);
    auto retLocationCode = CustomDBus::getCustomDBus().getLocationCode(tmpPath);

    EXPECT_NE(retLocationCode, std::nullopt);
    EXPECT_EQ(locationCode, retLocationCode);
}

TEST(CustomDBus, MicroCode)
{
    std::string tmpPath = "/abc/def";
    uint32_t value = 32;
    CustomDBus::getCustomDBus().setMicroCode(tmpPath, value);
    auto retMicroCode = CustomDBus::getCustomDBus().getMicroCode(tmpPath);
    EXPECT_NE(retMicroCode, std::nullopt);
    EXPECT_EQ(value, retMicroCode);
}

TEST(CustomDBus, OperationalStatus)
{
    std::string tmpPath = "/abc/def";
    bool status = false;
    bool retStatus = false;

    CustomDBus::getCustomDBus().setOperationalStatus(tmpPath, status);
    retStatus = CustomDBus::getCustomDBus().getOperationalStatus(tmpPath);

    EXPECT_EQ(status, false);
    EXPECT_EQ(retStatus, false);

    status = true;
    CustomDBus::getCustomDBus().setOperationalStatus(tmpPath, status);
    retStatus = CustomDBus::getCustomDBus().getOperationalStatus(tmpPath);

    EXPECT_EQ(status, true);
    EXPECT_EQ(retStatus, true);
}

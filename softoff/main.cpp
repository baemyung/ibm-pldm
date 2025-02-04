#include "common/instance_id.hpp"
#include "common/utils.hpp"
#include "softoff.hpp"

#include <getopt.h>

#include <phosphor-logging/lg2.hpp>
PHOSPHOR_LOG2_USING;

int main(int argc, char* argv[])
{
    bool noTimeOut = false;
    static struct option long_options[] = {{"notimeout", no_argument, 0, 't'},
                                           {0, 0, 0, 0}};

    auto argflag = getopt_long(argc, argv, "t", long_options, nullptr);
    switch (argflag)
    {
        case 't':
            noTimeOut = true;
            std::cout << "Not applying any time outs\n";
            break;
        case -1:
            break;
        default:
            exit(EXIT_FAILURE);
    }

    // Get a default event loop
    auto event = sdeventplus::Event::get_default();

    // Get a handle to system D-Bus.
    auto& bus = pldm::utils::DBusHandler::getBus();

    // Obtain the instance database
    pldm::InstanceIdDb instanceIdDb;

    // Attach the bus to sd_event to service user requests
    bus.attach_event(event.get(), SD_EVENT_PRIORITY_NORMAL);

    pldm::SoftPowerOff softPower(bus, event.get(), instanceIdDb, noTimeOut);

    if (softPower.isError())
    {
        error(
            "Failure in gracefully shutdown by remote terminus, exiting pldm-softpoweroff app");
        return -1;
    }

    if (softPower.isCompleted())
    {
        error(
            "Remote terminus current state is not Running, exiting pldm-softpoweroff app");
        return 0;
    }

    // Send the gracefully shutdown request to the host and
    // wait the host gracefully shutdown.
    if (softPower.hostSoftOff(event))
    {
        error(
            "Failure in sending soft off request to the remote terminus. Exiting pldm-softpoweroff app");
        return -1;
    }

    if (softPower.isTimerExpired() && softPower.isReceiveResponse())
    {
        pldm::utils::reportError(
            "xyz.openbmc_project.PLDM.Error.SoftPowerOff.HostSoftOffTimeOut");

        auto method = bus.new_method_call(
            "xyz.openbmc_project.Dump.Manager", "/xyz/openbmc_project/dump/bmc",
            "xyz.openbmc_project.Dump.Create", "CreateDump");
        method.append(
            std::vector<
                std::pair<std::string, std::variant<std::string, uint64_t>>>());
        try
        {
            bus.call_noreply(method);
        }
        catch (const sdbusplus::exception::exception& e)
        {
            error("SoftPowerOff:Failed to create BMC dump, ERROR={ERR_EXCEP}",
                  "ERR_EXCEP", e.what());
        }
        error(
            "ERROR! Waiting for the host soft off timeout. Exit the pldm-softpoweroff");
        return -1;
    }

    return 0;
}

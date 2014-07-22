#include <facter/facts/collection.hpp>
#include <facter/facts/posix/kernel_resolver.hpp>
#include <facter/facts/linux/operating_system_resolver.hpp>
#include <facter/facts/linux/lsb_resolver.hpp>
#include <facter/facts/linux/networking_resolver.hpp>
#include <facter/facts/linux/block_device_resolver.hpp>
#include <facter/facts/linux/dmi_resolver.hpp>
#include <facter/facts/linux/processor_resolver.hpp>
#include <facter/facts/linux/uptime_resolver.hpp>
#include <facter/facts/linux/selinux_resolver.hpp>
#include <facter/facts/linux/virtualization_resolver.hpp>
#include <facter/facts/posix/ssh_resolver.hpp>

using namespace std;

namespace facter { namespace facts {

    void collection::add_platform_facts()
    {
        add(make_shared<posix::kernel_resolver>());
        add(make_shared<linux::operating_system_resolver>());
        add(make_shared<linux::lsb_resolver>());
        add(make_shared<linux::networking_resolver>());
        add(make_shared<linux::block_device_resolver>());
        add(make_shared<linux::dmi_resolver>());
        add(make_shared<linux::processor_resolver>());
        add(make_shared<linux::uptime_resolver>());
        add(make_shared<linux::selinux_resolver>());
        add(make_shared<posix::ssh_resolver>());
        add(make_shared<linux::virtualization_resolver>());
    }

}}  // namespace facter::facts
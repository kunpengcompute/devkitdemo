import sys

from check_env_remote import CheckDemoEnvironment


def check_control_server():
    control = CheckDemoEnvironment()
    check_list = {
        control.check_local_os:(),
        control.check_status_firewalld:(),
        control.check_openstack_neutron:(),
        control.check_openstack_nova:(),
        control.check_openstack_ceph:(),
        control.check_openstack_version:()
    }
    for function, args in check_list.items():
        status, result = function(*args)
        if not status:
            break
    return status, result


if __name__ == "__main__":

    status, result = check_control_server()
    if status:
        sys.exit(0)
    sys.exit(1)

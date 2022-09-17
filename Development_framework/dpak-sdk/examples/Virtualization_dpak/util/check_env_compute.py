import os 
import sys

from configparser import ConfigParser
from check_env_remote import CheckDemoEnvironment


def check_compute_server(ip, username, port):
    compute = CheckDemoEnvironment(ip, username, port)
    check_list = {
        compute.check_remote_os:(),
        compute.check_cat_fstab:(),
        compute.check_status_remote_firewalld:(),
        compute.check_status_hinic3:(),
        compute.ovs_vsctl_show:()
    }
    for function, args in check_list.items():
        status, result = function(*args)
        if not status:
            break
    return status, result


if __name__ == "__main__":
    compute_num = sys.argv[1]
    cfg = ConfigParser()
    cfg.read(os.path.join(os.path.dirname(os.path.dirname(os.path.realpath(__file__))),"conf/demo_conf.cfg"))
    # compute server 
    if compute_num == 'compute_first':
        port_compute = cfg.get('compute_first', 'port_compute_first')
        username_compute = cfg.get('compute_first', 'username_compute_first')
        ip_compute = cfg.get('compute_first', 'ip_compute_first')
    else:
        port_compute = cfg.get('compute_second', 'port_compute_second')
        username_compute = cfg.get('compute_second', 'username_compute_second')
        ip_compute = cfg.get('compute_second', 'ip_compute_second')
    status, result = check_compute_server(ip_compute, username_compute, port_compute)
    if status:
        sys.exit(0)
    sys.exit(1)

import os 
import sys

from configparser import ConfigParser
from check_env_remote import CheckDemoEnvironment


def check_compute_server(ip, username, port):
    compute = CheckDemoEnvironment(ip, username, port, 'remote')
    status, result = compute.check_vm_ping(eth0_ip, show_message)

    return status, result


if __name__ == "__main__":
    compute_num = sys.argv[1]
    cfg = ConfigParser()
    cfg.read(os.path.join(os.path.dirname(os.path.dirname(os.path.realpath(__file__))),"conf/demo_conf.cfg"))
    # compute 
    if compute_num == 'compute_first':
        port_compute = cfg['compute_first'].get('port_compute_first')
        username_compute = cfg['compute_first'].get('username_compute_first')
        ip_compute = cfg['compute_first'].get('ip_compute_first')
        eth0_ip = cfg.get('compute_second','eth0_ip_second')
        show_message = "VM1 can ping vm2"
    else:
        port_compute = cfg['compute_second'].get('port_compute_second')
        username_compute = cfg['compute_second'].get('username_compute_second')
        ip_compute = cfg['compute_second'].get('ip_compute_second')
        eth0_ip = cfg.get('compute_first','eth0_ip_first')
        show_message = "VM1 can ping vm2"
    status, result = check_compute_server(ip_compute,username_compute, port_compute)
    if status:
        sys.exit(0)
    sys.exit(1)

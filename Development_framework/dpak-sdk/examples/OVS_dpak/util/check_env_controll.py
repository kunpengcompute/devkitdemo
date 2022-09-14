import sys
import os
from configparser import ConfigParser
from check_env_remote import CheckDemoEnvironment


def check_control_server(ip,usermae, port,flag):
    control = CheckDemoEnvironment(ip,usermae, port,flag)
    check_list = {
        control.check_os:(),
        control.check_status_firewalld:(),
        control.check_vf_status:(),
        control.ovs_vsctl_show:(),
        control.check_status_hinic3:(),
        control.check_openv_switch_status:()
    }
    for function, args in check_list.items():
        status, result = function(*args)
        if not status:
            break
    return status, result


if __name__ == "__main__":
    flag = sys.argv[1]
    cfg = ConfigParser()
    cfg.read(os.path.join(os.path.dirname(os.path.dirname(os.path.realpath(__file__))),"conf/demo_conf.cfg"))
    ip_compute = None
    username_compute = None
    port_compute = None
    if flag == "remote":
        port_compute = cfg['server_host_second'].get('port_host_second')
        username_compute = cfg['server_host_second'].get('username_host_first')
        ip_compute = cfg['server_host_second'].get('ip_host_first')
    status, result = check_control_server(ip_compute,username_compute,port_compute,flag)
    if status:
        sys.exit(0)
    sys.exit(1)

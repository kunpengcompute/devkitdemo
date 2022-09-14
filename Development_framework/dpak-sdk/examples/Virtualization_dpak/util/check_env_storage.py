import os 
import sys

from configparser import ConfigParser
from check_env_remote import CheckDemoEnvironment


def check_storage_server(ip, username, port):
    storage = CheckDemoEnvironment(ip, username, port)

    check_list = {
        storage.check_remote_os:(),
        storage.check_status_remote_firewalld:(),
    }
    for function, args in check_list.items():
        status, result = function(*args)
        if not status:
            break

    return status, result


if __name__ == "__main__":
    cfg = ConfigParser()
    cfg.read(os.path.join(os.path.dirname(os.path.dirname(os.path.realpath(__file__))),"conf/demo_conf.cfg"))
    port_storage = cfg.get('storage', 'port_storage')
    username_storage = cfg.get('storage', 'username_storage')
    ip_storage = cfg.get('storage', 'ip_storage')

    status, result = check_storage_server(ip_storage, username_storage, port_storage)
    if status:
        sys.exit(0)
    sys.exit(1)

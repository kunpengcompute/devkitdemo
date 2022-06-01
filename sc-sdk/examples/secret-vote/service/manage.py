#!/usr/bin/env python
"""Django's command-line utility for administrative tasks."""
import os
import sys
from ctypes import cdll


def check_python_version():
    if sys.version_info.major != 3 or sys.version_info.minor < 7:
        print("Python 3.7.0 or later is required.")
        return False
    return True


def check_openssl_version():
    try:
        crypto = cdll.LoadLibrary("libcrypto.so")
    except OSError:
        print("The openssl library cannot be found.")
        return
    try:
        version_num = crypto.OpenSSL_version_num()
    except AttributeError:
        print("openssl 1.1.1 or later is required.")
        return False
    # openssl 1.1.1 version number: 0x1010100f
    if version_num < 0x1010100f:
        print("openssl 1.1.1 or later is required.")
        return False
    return True


def main():
    if not check_python_version():
        return
    if not check_openssl_version():
        return
    os.environ.setdefault('DJANGO_SETTINGS_MODULE', 'service.settings')
    try:
        from django.core.management import execute_from_command_line
    except ImportError as exc:
        raise ImportError(
            "Couldn't import Django. Are you sure it's installed and "
            "available on your PYTHONPATH environment variable? Did you "
            "forget to activate a virtual environment?"
        )
    execute_from_command_line(sys.argv)


if __name__ == '__main__':
    main()

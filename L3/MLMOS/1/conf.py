#! /usr/bin/env python3

# Networking doc: https://www.serverlab.ca/tutorials/linux/administration-linux/how-to-configure-centos-7-network-settings/


import configparser
import os
import re
import sys


NET_MAP = [
    "IPADDR",
    "NETMASK",
    "GATEWAY",
]


def configure_network(conf):
    with open(conf["path"]) as stream:
        data = stream.read()
    for entry in NET_MAP:
        value = conf[entry]
        row = f"{entry}={value}"
        if entry in data:
            data = re.sub(fr"{entry}=.+", row, data)
        else:
            data += row + "\n"
    print(f"[*] Saving the following into network config:\n{data}")
    with open(conf["path"], "w") as stream:
        stream.write(data)


def setup_iface(conf):
    print(conf["name"])


def main():
    if len(sys.argv) != 2:
        print(f"[i] Usage: {sys.argv[0]} <CONFIG_FILE>")
        return

    file_path = sys.argv[1]
    if not os.path.isfile(file_path):
        print(f"[x] {file_path!r} doesn't exist.")
        return

    conf = configparser.ConfigParser()
    conf.read(sys.argv[1])

    configure_network(conf["net"])
    setup_iface(conf["iface"])


if __name__ == "__main__":
    main()

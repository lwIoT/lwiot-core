#
# lwIoT - Continuous integration runner
#
# Author: Michel Megens
# Date:   12/01/2018
# Email:  dev@bietje.net
#

import sys
import os
import yaml


def execute(cmds):
    for cmd in cmds:
        os.system(cmd)


def parse(config, target):
    with open(config) as f:
        data = f.read()

    data = yaml.load(data)
    execute(data[target]['script'])


def check_args():
    if len(sys.argv) != 3:
        print("Usage: %s <ci-config-file> <target>" % sys.argv[0])
        sys.exit(-1)


def main():
    check_args()
    configfile = sys.argv[1]
    target = sys.argv[2]
    parse(configfile, target)


if __name__ == "__main__":
    main()

#
# CMake build script
#
# @author Michel Megens
# @email  dev@bietje.net
# @date   04/05/2018
#

import sys
import argparse
import os
import StringIO

from yaml import load
try:
	from yaml import CLoader as Loader
except:
	from yaml import Loader

class ArgumentParser(object):
    def __init__(self):
        self.config = None
        self.target = None
        self.define = []
        self.buildtype = 'Debug'

class ConfigParser(object):
    def __init__(self, config):
        try:
            with open(config.config) as f:
                data = load(f, Loader)
                target = data[config.target]
                self.defs = target['defs']
                self.config = config
                self.build_arguments(config)
        except KeyError, e:
            print 'Unable to parse build configuration. Definition not found: %s' % e.message
        except IOError, e:
            print 'Unable to open configuration file %s' % config.config


    def build_arguments(self, config):
        argbuilder = StringIO.StringIO()
        for definition in self.defs:
            argbuilder.write('-D%s ' % definition)

        for definition in self.config.define:
            argbuilder.write('-D%s ' % definition)

        argbuilder.write('-DCMAKE_BUILD_TYPE=%s ' % config.buildtype)
        argbuilder.write('../..')
        self.args = argbuilder.getvalue().strip()


def build(parser, config):
    relpath = 'build/%s' % config.target
    if not os.path.exists(relpath):
        os.mkdir(relpath)

    os.chdir(relpath)
    os.system('cmake %s' % parser.args)


def main():
    args = ArgumentParser()
    parser = argparse.ArgumentParser(description='lwIoT build configurator')
    parser.add_argument('-D', '--define', action='append', metavar=('def'))
    parser.add_argument('-b', '--buildtype', metavar='BUILD TYPE', help='build type')
    parser.add_argument('-t', '--target', metavar='TARGET', help='build target', required=True)
    parser.add_argument('-c', '--config', metavar='PATH', help='build configuration file', required=True)
    parser.add_argument('-v', '--version', action='version', version='%(prog)s 0.0.1')
    parser.parse_args(namespace=args)

    conf = ConfigParser(args)
    build(conf, args)

if __name__ == "__main__":
	main()

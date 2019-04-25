#
# Run unit tests based on a configuration file
#
# Author: Michel Megens
# Email:  dev@bietje.net
# Date:   26/01/2018
#

# pylint: disable=C0330,W0312,C0111,C0103,W0702

import os
import sys
import argparse
import xml.etree.ElementTree as ET
from yaml import load

try:
    from yaml import CLoader as Loader
except:
    from yaml import Loader


class TestFailedError(Exception):
    def __init__(self, message):
        super(TestFailedError, self).__init__()
        self.message = message


class ArgumentParser(object):
    def __init__(self):
        self.all = False
        self.conf = None
        self.parser = None
        self.config = None
        self.tests = None
        self.platform = None
        self.xml = None


class UnitTest(object):
    def __init__(self, title, info, valgrind=False, helgrind=False, xml='../vg.xml'):
        self.command = info['command']
        self.args = info['args']
        self.valgrind = valgrind
        self.helgrind = helgrind
        self.title = title
        self.result = "FAILED"
        self.vgxml = xml

    def evaluate_vg_results(self):
        try:
            tree = ET.parse(self.vgxml)
            root = tree.getroot()
            for child in root:
                if child.tag == 'error':
                    return False
        except ET.ParseError:
            print 'Unable to parse valgrind output!'
            sys.exit(-1)
        return True

    def execute(self):
        prefix = ""
        valgrindOk = True
        if self.valgrind:
            prefix = "valgrind --xml=yes --xml-file=vg.xml"
        elif self.helgrind:
            prefix = "valgrind --xml=yes --xml-file=vg.xml --tool=helgrind"

        x = "%s %s %s" % (prefix, self.command, self.args)
        x = x.strip()
        print "Running '%s'\n" % self.title
        rv = os.system(x)

        if self.valgrind or self.helgrind:
            valgrindOk = self.evaluate_vg_results()

        if rv is 0 and valgrindOk:
            self.result = "OK"
        else:
            return -1

        return rv


class ConfigParser(object):
    def __init__(self, args):
        self.tests = []
        with open(args.config) as fname:
            data = load(fname, Loader=Loader)
            if args.all:
                args.tests = data[args.platform].keys()

            for t in args.tests:
                try:
                    test = data[args.platform][t]
                    self.tests.append(UnitTest(t, test, args.valgrind, args.helgrind, args.xml))
                except KeyError:
                    print 'Test \'%s\' does not exist in %s!' % (t, args.config)
                    exit(1)


    def print_results(self):
        print "Unit test results:"
        for t in self.tests:
            string = "Test '%s' result:" % t.title
            print string, t.result


def main():
    args = ArgumentParser()
    parser = argparse.ArgumentParser(description='Unit test runner')
    parser.add_argument('-a', '--all', action='store_true',
                        help='run all tests for the given platform')
    parser.add_argument('-c', '--config', metavar='PATH', help='unit test configuration file path',
                        required=True)
    parser.add_argument('-p', '--platform', metavar='PLATFORM', help='system environment to run in',
                        required=True)
    parser.add_argument('-V', '--valgrind', action='store_true', help='run unit tests with valgrind')
    parser.add_argument('-H', '--helgrind', action='store_true', help='run unit tests with helgrind')
    parser.add_argument('-x', '--xml', metavar='XML FILE', help='Valgrind output XML file')
    parser.add_argument('tests', metavar='NAMES', nargs='*')
    parser.add_argument('-v', '--version', action='version', version='%(prog)s 0.0.1')
    parser.parse_args(namespace=args)
    conf = ConfigParser(args)

    failed = False
    for t in conf.tests:
        if t.execute() is not 0:
            failed = True
    conf.print_results()

    if failed:
        raise TestFailedError("A unit test has failed!")


if __name__ == "__main__":
    try:
        main()
    except:
        sys.exit(1)

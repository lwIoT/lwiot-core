#
# File header generation script.
#
# @author Michel Megens
# @email  dev@bietje.net
#

import os, sys
import argparse

from yaml import load
try:
	from yaml import CLoader as Loader
except:
	from yaml import Loader

class CliModel(object):
    def __init__(self):
        super(CliModel, self).__init__()
        self.config = os.getcwd() + '/.header.yaml'
        self.description = None
        self.target = None
        self.force = False


class Application(object):
    def __init__(self):
        super(Application, self).__init__()

        self.model = CliModel()
        self.parser = argparse.ArgumentParser(description='lwIoT build configurator')
        self.parser.add_argument('-c', '--config', metavar='PATH', help='build configuration file', required=False)
        self.parser.add_argument('-f', '--force', help='overwrite contents if FILE already exists', action='store_true')
        self.parser.add_argument('-d', '--description', metavar='DESCRIPTION', help='build configuration file', required=True)
        self.parser.add_argument('-v', '--version', action='version', version='%(prog)s 0.0.1')
        self.parser.add_argument('target', metavar='FILE', nargs='+')


    def run(self):
        self.parse_cli()
        data = """
/*
 * %s
 *
 * @author %s
 * @email  %s
 */"""
        data = data.lstrip()

        for file in self.model.target:
            if os.path.exists(file) and not self.model.force:
                print 'Output file %s already exists. Use -f to overwrite. Skipping..' % file
                continue

            sys.stdout.write('Writing to %s...' % file)
            with open(file, 'w+') as f:
                f.write(data % (self.model.description, self.model.author, self.model.email))

            print ' [DONE]'

    def parse_cli(self):
        self.parser.parse_args(namespace=self.model)

        # Make sure the configuration file exists
        if not os.path.exists(self.model.config):
            print 'Configuration file does not exist!'
            sys.exit(-1)

        with open(self.model.config) as f:
            data = load(f, Loader)
            try:
                self.model.author = data['author']
                self.model.email = data['email']
            except KeyError, e:
                print 'Missing configuration entry: %s' % e.message
                sys.exit(-1)


def main():
    app = Application()
    app.run()


if __name__ == "__main__":
    main()

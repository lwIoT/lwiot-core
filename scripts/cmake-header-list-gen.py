#
# Header requirement list generator for CMake.
#
# @author Michel Megens
# @email dev@bietje.net
#

import os
import fnmatch
import sys


class Generator(object):
    def __init__(self, path, pattern='*'):
        self.path = path
        self.pattern = pattern

    def run(self):
        results = self.find()
        return list(results)

    def find(self):
        for dirpath, dirnames, filenames in os.walk(self.path):
            for file_name in fnmatch.filter(filenames, self.pattern):
                yield os.path.join(dirpath, file_name)


def main():
    gen = Generator(sys.argv[1], '*.h')
    directories = gen.run()
    print('SET(GENERIC_HEADERS')
    for header in directories:
        print '\t' + header[len('include/'):len(header)]
    print(')')


if __name__ == '__main__':
    main()

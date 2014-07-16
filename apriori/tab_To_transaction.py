#!/bin/python

import sys

def main(filename):
    f = open(filename, 'r')
    count = 0
    if not(f):
        print "error on file"
    else:
        for line in f:
            count += 1
            for el in line.split('\t'):
                if(el == '\n'):
                    continue
                print "TR" + str(count) + " " + el

if __name__ == "__main__":
    if(len(sys.argv) > 1):
        main(sys.argv[1])
    else:
        print "insert file name"

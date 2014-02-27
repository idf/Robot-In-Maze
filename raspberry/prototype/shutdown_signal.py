import sys
from signal import *


def bye(*args):
  print "bye"
  sys.exit(1)

for sig in (SIGABRT, SIGILL, SIGINT, SIGSEGV, SIGTERM):
  signal(sig, bye)

while True: 
  pass
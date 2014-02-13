import os
import subprocess
def execute_lzz():
  """
  This function executes lzz (lazy C++) on the files in current directories and subdirectories
  to generate .cpp and .h files as specified by the .lzz file accordingly.
  """
  for root, dirs, files in os.walk("."):
    for name in files:
      if name.endswith((".lzz")):
        relative_path = os.path.join(root, name) # root is the dir of the file
        cmd = "lzz %s"%relative_path
        print "Executing: %s"%cmd
        subprocess.call(cmd)

if __name__=="__main__":
  execute_lzz()
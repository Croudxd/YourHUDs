import os
import sys
import shutil

def copyfiles(src, dst):
    shutil.copytree(src, dst)

if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("Usage: python script.py <zipInstallPath>")
        sys.exit(1)

    src = sys.argv[1]
    dst = sys.argv[2]
    print(f"Received src: {src} and recieved dst: {dst}" )
    copyfiles(src, dst)
import zipfile
import os
import sys

def unzip(zipInstallPath):
    destination_dir = os.path.dirname(zipInstallPath)
    handle = zipfile.ZipFile(zipInstallPath)
    handle.extractall(destination_dir)
    handle.close()


if __name__ == "_ _ main_ _":
    if len(sys.argv) !=2:
        print("Usage: python script.py <zipInstallPath>")
        sys.exit(1)

    zipInstallPath = sys.argv[1]
    unzip(zipInstallPath)
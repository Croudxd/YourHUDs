import zipfile
import os
import sys

def unzip(zipInstallPath):
    destination_dir = os.path.dirname(zipInstallPath)
    print(f"Working directory: {os.getcwd()}")
    print(f"Destination directory: {destination_dir}")
    handle = zipfile.ZipFile(zipInstallPath)
    handle.extractall(destination_dir)
    handle.close()

if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Usage: python script.py <zipInstallPath>")
        sys.exit(1)

    zipInstallPath = sys.argv[1]
    print(f"Received zipInstallPath: {zipInstallPath}")
    unzip(zipInstallPath)

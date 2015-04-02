import os

env = Environment()
Export("env")

for (path, dirs, files) in os.walk("."):
    if "SConscript" in files:
        print os.path.join(path, "SConscript")
        SConscript(os.path.join(path, "SConscript"))

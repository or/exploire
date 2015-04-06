import os


def DockerBuilder(env, source):
    directory = Dir(".").path
    docker_name = 'exploire/' + os.path.dirname(directory).replace('/', '_')
    env.Command(target=".docker-built",
                source=source,
                action="docker build -t {docker_name} {directory} > $TARGET || rm $TARGET"
                .format(docker_name=docker_name, directory=directory))

env = Environment()
env.AddMethod(DockerBuilder, "DockerBuilder")
Export("env")

for (path, dirs, files) in os.walk("."):
    if "SConscript" in files:
        SConscript(os.path.join(path, "SConscript"))

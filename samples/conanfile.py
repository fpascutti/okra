
# Copyright Franck Pascutti 2016.
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

import conans
import os.path

class OkraSamples(conans.ConanFile):
    name = "okra-samples"
    version = "0.0.1"
    url = "https://github.com/fpascutti/okra.git"
    license = "Boost Software License 1.0"
    author = "Franck Pascutti"

    requires = (
        ("gtest/1.8.0@eliaskousk/stable", "private"),
    )
    default_options = (
        "gtest:shared=False", # mock objects do not properly return errors with the shared version of googletest
    )
    settings = {"os", "arch", "compiler", "build_type"}
    generators = ["cmake"]

    def imports(self):
        self.copy("*.dll", dst="bin", src="bin")
        self.copy("*.dylib*", dst="bin", src="lib")

    def build(self):
        cmake = conans.CMake(self.settings)

        args = ["-DOKRA_BUILD_SAMPLES=ON"]
        args.append("-DOKRA_BUILD_SAMPLES=ON")
        if cmake.is_multi_configuration:
            args.append("-DCMAKE_CONFIGURATION_TYPES=\"%s\"" % (self.settings.build_type))

        self.run("cmake \"%s\" %s %s" % (os.path.join(self.conanfile_directory, os.path.pardir), cmake.command_line, " ".join(args)))
        self.run("cmake --build . %s" % (cmake.build_config))

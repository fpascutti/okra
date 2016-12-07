
# Copyright Franck Pascutti 2016.
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

import conans
import os.path

class Okra(conans.ConanFile):
    name = "okra"
    version = "0.0.1"
    url = "https://github.com/fpascutti/okra.git"
    license = "Boost Software License 1.0"
    author = "Franck Pascutti"

    settings = {"os", "arch", "compiler", "build_type"}
    generators = ["cmake"]

    exports = (
        "CMakeLists.txt",
        "LICENSE_1_0.txt",
        "README.md",
        "includes/*",
        "samples/*",
        "tests/*",
    )

    def config_options(self):
        self.options["gtest"].shared = False # mock objects do not properly return errors with the shared version of googletest
        if not self._needs_build:
            self.settings.clear()

    def requirements(self):
        if self.scope.build_tests or self.scope.build_samples:
            self.requires("gtest/1.8.0@eliaskousk/stable")

    def imports(self):
        self.copy("*.dll", dst="bin", src="bin")
        self.copy("*.dylib*", dst="bin", src="lib")

    def build(self):
        if not self._needs_build:
            return

        cmake = conans.CMake(self.settings)

        args = []
        args.append("-DOKRA_BUILD_TESTS=%s" % ("ON" if self.scope.build_tests else "OFF"))
        args.append("-DOKRA_BUILD_SAMPLES=%s" % ("ON" if self.scope.build_samples else "OFF"))

        if cmake.is_multi_configuration:
            args.append("-DCMAKE_CONFIGURATION_TYPES=\"%s\"" % (self.settings.build_type))

        self.run("cmake \"%s\" %s %s" % (self.conanfile_directory, cmake.command_line, " ".join(args)))
        self.run("cmake --build . %s" % (cmake.build_config))

        if self.scope.build_tests:
            if cmake.is_multi_configuration:
                self.run("ctest --verbose --extra-verbose --build-config \"%s\"" % (self.settings.build_type))
            else:
                self.run("ctest --verbose --extra-verbose")

    def package(self):
        self.copy("LICENSE_1_0.txt", keep_path=False)
        self.copy("*.h", dst="include", src="includes", keep_path=True)

    @property
    def _needs_build(self):
        return (self.scope.build_tests or self.scope.build_samples)

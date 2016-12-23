
# Copyright Franck Pascutti 2016.
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

import conans
import os.path

class OkraTests(conans.ConanFile):
    name = "okra-tests"
    version = "0.0.1"
    url = "https://github.com/fpascutti/okra.git"
    license = "Boost Software License 1.0"
    author = "Franck Pascutti"

    settings = {"os", "arch", "compiler", "build_type"}
    generators = ["cmake"]

    options = {
        "use_boost_regex": [True, False],
        "use_boost_wstringconvert": [True, False],
    }
    default_options = (
        "use_boost_regex=False",
        "use_boost_wstringconvert=False",
        "gtest:shared=False", # mock objects do not properly return errors with the shared version of googletest
    )

    requires = (
        ("gtest/1.8.0@eliaskousk/stable", "private"),
    )

    def requirements(self):
        if self.options.use_boost_regex or self.options.use_boost_wstringconvert:
            self.requires("Boost/1.61.0@eliaskousk/stable")

    def imports(self):
        self.copy("*.dll", dst="bin", src="bin")
        self.copy("*.dylib*", dst="bin", src="lib")

    def build(self):
        cmake = conans.CMake(self.settings)

        args = ["-DOKRA_BUILD_TESTS=ON"]
        args.append("-DOKRA_CONFIG_USEBOOSTREGEX=%s" % ("ON" if self.options.use_boost_regex else "OFF"))
        args.append("-DOKRA_CONFIG_USEBOOSTWSTRINGCONVERT=%s" % ("ON" if self.options.use_boost_wstringconvert else "OFF"))
        if cmake.is_multi_configuration:
            args.append("-DCMAKE_CONFIGURATION_TYPES=\"%s\"" % (self.settings.build_type))

        self.run("cmake \"%s\" %s %s" % (os.path.join(self.conanfile_directory, os.path.pardir), cmake.command_line, " ".join(args)))
        self.run("cmake --build . %s" % (cmake.build_config))

        if cmake.is_multi_configuration:
            self.run("ctest --verbose --extra-verbose --build-config \"%s\"" % (self.settings.build_type))
        else:
            self.run("ctest --verbose --extra-verbose")

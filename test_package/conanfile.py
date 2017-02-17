
# Copyright Franck Pascutti 2016-2017.
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

import conans
import os
import os.path

USERNAME = os.getenv("CONAN_USERNAME", "demo")
CHANNEL = os.getenv("CONAN_CHANNEL", "testing")

class OkraTestPackage(conans.ConanFile):
    requires = ("okra/0.0.1@%s/%s" % (USERNAME, CHANNEL))
    settings = {"os", "arch", "compiler", "build_type"}
    generators = ["cmake"]

    def build(self):
        cmake = conans.CMake(self.settings)
        self.run("cmake \"%s\" %s" % (self.conanfile_directory, cmake.command_line))
        self.run("cmake --build . %s" % (cmake.build_config))

    def imports(self):
        if self.settings.os == "Windows":
            self.copy("*.dll", dst="bin", src="bin") # copy requirements DLL if needed

    def test(self):
        self.run(os.path.join(os.curdir, "bin", "test_package"))

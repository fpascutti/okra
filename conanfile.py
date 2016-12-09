
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

    settings = None

    exports = (
        "includes/*",
        "CMakeLists.txt",
        "LICENSE_1_0.txt",
    )

    def build(self):
        # only run CMake to generate the `compiler_detection.h` file
        # no need for a specific generator as the
        # `write_compiler_detection_header` function does not use it
        args = ["-DOKRA_BUILD_TESTS=OFF", "-DOKRA_BUILD_SAMPLES=OFF"]
        self.run("cmake \"%s\" %s" % (self.conanfile_directory, " ".join(args)))

    def package(self):
        self.copy("LICENSE_1_0.txt", keep_path=False)
        self.copy("*.h", dst="include", src="includes", keep_path=True)

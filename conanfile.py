
# Copyright Franck Pascutti 2016-2017.
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
    options = {
        "use_boost_regex": [True, False],
        "use_boost_wstringconvert": [True, False],
    }
    default_options = (
        "use_boost_regex=False",
        "use_boost_wstringconvert=False",
    )

    exports = (
        "includes/*",
        "CMakeLists.txt",
        "LICENSE_1_0.txt",
    )

    def config_options(self):
        if not self.options.use_boost_regex and not self.options.use_boost_wstringconvert:
            self.settings.clear()

    def requirements(self):
        if self.options.use_boost_regex or self.options.use_boost_wstringconvert:
            self.requires("Boost/1.61.0@eliaskousk/stable")

    def build(self):
        # only run CMake to generate the `compiler_detection.h` file
        # no need for a specific generator as the
        # `write_compiler_detection_header` function does not use it
        args = ["-DOKRA_BUILD_TESTS=OFF", "-DOKRA_BUILD_SAMPLES=OFF"]
        args.append("-DOKRA_CONFIG_USEBOOSTREGEX=%s" % ("ON" if self.options.use_boost_regex else "OFF"))
        args.append("-DOKRA_CONFIG_USEBOOSTWSTRINGCONVERT=%s" % ("ON" if self.options.use_boost_wstringconvert else "OFF"))
        self.run("cmake \"%s\" %s" % (self.conanfile_directory, " ".join(args)))

    def package(self):
        self.copy("LICENSE_1_0.txt", keep_path=False)
        self.copy("*.h", dst="include", src="includes", keep_path=True)

    def package_info(self):
        if self.options.use_boost_regex:
            self.cpp_info.defines.append("OKRA_CONFIG_USEBOOSTREGEX=1")
        if self.options.use_boost_wstringconvert:
            self.cpp_info.defines.append("OKRA_CONFIG_USEBOOSTWSTRINGCONVERT=1")

# ------------------------------------------------------------
# This file is part of mem of headcode.space
#
# The 'LICENSE.txt' file in the project root holds the software license.
# Copyright (C) 2020-2021 headcode.space e.U.
# Oliver Maurhart <info@headcode.space>, https://www.headcode.space
# ------------------------------------------------------------

from conans import ConanFile, CMake, tools


class HcsLoggerConan(ConanFile):
    """Conan package manager file. See https://conan.io"""
    name = "@CMAKE_PROJECT_NAME@"
    version = "@VERSION@"
    license = "MIT"
    author = "Oliver Maurhart (oliver.maurhart@headcode.space)"
    url = "https://gitlab.com/headcode.space/logger"
    homepage = "https://gitlab.com/headcode.space/logger"
    description = "Very much like log4c++, yet minimal and concise with no further dependencies."
    topics = ("logging", "logger", "log4j", "log4c++", "small", "self-contained", "C++17")
    no_copy_source = True
    settings = "os", "compiler", "build_type", "arch"

    def source(self):
        git_source = "@CMAKE_SOURCE_DIR@" or "https://gitlab.com/headcode.space/benchmark.git"
        self.run(f"git clone --recurse-submodules {git_source} {self.name}")

    def build(self):
        cmake = CMake(self)
        cmake.configure(source_folder="hcs-logger")
        cmake.build()

    def package(self):
        self.copy("*.hpp", src=f"{self.name}/include", dst="include")
        self.copy("*.h", dst="include", src="hello")
        self.copy("*hello.lib", dst="lib", keep_path=False)
        self.copy("*.dll", dst="bin", keep_path=False)
        self.copy("*.so", dst="lib", keep_path=False)
        self.copy("*.dylib", dst="lib", keep_path=False)
        self.copy("*.a", dst="lib", keep_path=False)

    def package_info(self):
        self.cpp_info.libs = ["hello"]

    def package_id(self):
        # check this
        self.info.header_only()

from conans import ConanFile, CMake, tools, AutoToolsBuildEnvironment
import platform


class GHermeneusConan(ConanFile):
    name = "GHermeneus"
    version = "0.1"
    license = "LGPL-3.0"
    author = "Jelle Spijker <j.spijker@ultimaker.com>"
    url = "https://github.com/Ultimaker/GHermeneus"
    description = "Fast GCode interpreter"
    topics = ("GCode", "3D printing", "Path generator", "Ultimaker", "Cura")
    settings = "os", "compiler", "build_type", "arch"
    requires = "eigen/3.3.7@conan/stable", "range-v3/0.10.0@ericniebler/stable"
    build_requires = "gtest/1.10.0"
    options = {"shared": [True, False], "build_tests": [True, False]}
    default_options = {"shared": False, "build_tests": True}
    generators = "cmake"

    def configure_cmake(self):
        cmake = CMake(self)
        cmake.definitions["BUILD_TESTS"] = self.options.build_tests
        cmake.configure()
        return cmake

    def build(self):
        cmake = self.configure_cmake()
        cmake.build()
        if self.options.build_tests:
            cmake.test()

    def package(self):
        self.copy("*.h", dst="include", src="GHermeneus")
        self.copy("*GHermeneus.lib", dst="lib", keep_path=False)
        self.copy("*.dll", dst="bin", keep_path=False)
        self.copy("*.so", dst="lib", keep_path=False)
        self.copy("*.dylib", dst="lib", keep_path=False)
        self.copy("*.a", dst="lib", keep_path=False)

    def package_info(self):
        self.cpp_info.name = "GCodeHermeneus"
        self.cpp_info.libs = tools.collect_libs(self)


from conans import ConanFile, CMake
import os


class MiniCheetahActuatorLibConan(ConanFile):
    name = "mini_cheetah_actuator_lib"
    version = "1.0.0"
    license = "MIT"
    author = "Sebastian Bergt sebastian@bergt.org"
    url = "<Package recipe repository url here, for issues about the package>"
    description = "Library for Motor Interaction via CAN"
    topics = ("MiniCheetahActuator", "CAN")
    settings = "os", "compiler", "build_type", "arch"
    generators = ["cmake", "cmake_find_package"]

    exports_sources = "src/**"

    options = {"with_tests": [True, False], "with_coverage": [True, False]}
    default_options = {"with_tests": True, "with_coverage": False, "catch2:with_main": True}

    def build_requirements(self):
        self.build_requires("catch2/2.13.7")
        self.build_requires("trompeloeil/41")
        self.build_requires("can_interface_lib/1.0.0")
        self.build_requires("strong_types_lib/1.0.0")

    def _report_coverage(self):
        self.run("gcovr -r .".split())
        self.run("gcovr -r . --html --html-details -o coverage.html".split())
        cwd = os.getcwd()
        self.output.highlight("coverage report was created here: " + cwd + "/coverage.html")
        self.run("google-chrome " + cwd + "/coverage.html")

    def build(self):
        cmake = CMake(self)
        cmake.definitions["WITH_TESTS"] = self.options.with_tests
        cmake.definitions["CONAN_CXX_FLAGS"] = "-Wall -Wextra -Wpedantic -Werror "
        if self.options.with_coverage:
            cmake.definitions["CONAN_CXX_FLAGS"] += "--coverage -g -O0 "
        cmake.configure(source_folder="src")
        cmake.build()
        if self.options.with_tests:
            cmake.test()
            if self.options.with_coverage:
                self._report_coverage()
        cmake.install()

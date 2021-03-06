from conans import CMake, ConanFile, tools
import os


class MiniCheetahActuatorLibConan(ConanFile):
    name = "mini_cheetah_actuator_lib"
    version = "1.0.0"
    license = "MIT"
    author = "Sebastian Bergt sebastian@bergt.org"
    url = "https://github.com/sebastianbergt/MiniCheetahActuatorGui"
    description = "Library for Motor Interaction via CAN"
    topics = ("MiniCheetahActuator", "CAN")
    settings = "os", "compiler", "build_type", "arch"
    generators = ["cmake", "cmake_find_package"]

    exports_sources = "src/**"

    options = {"with_tests": [True, False], "with_coverage": [True, False]}
    default_options = {
        "with_tests": True,
        "with_coverage": False,
        "catch2:with_main": True,
    }

    def build_requirements(self):
        if self.options.with_tests:
            self.build_requires("catch2/2.13.7", force_host_context=True)
            self.build_requires("trompeloeil/41", force_host_context=True)

    def requirements(self):
        self.requires("can_interface_lib/1.0.0")
        self.requires("strong_types_lib/1.0.0")

    def _report_coverage(self):
        self.run("gcovr -r .".split())
        self.run("gcovr -r . --html --html-details -o coverage.html".split())
        cwd = os.getcwd()
        self.output.highlight(
            "coverage report was created here: " + cwd + "/coverage.html"
        )

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

    def package_info(self):
        self.cpp_info.names["cmake_find_package"] = self.name
        self.cpp_info.names["cmake_find_package_multi"] = self.name
        self.cpp_info.libs = tools.collect_libs(self)
        self.cpp_info.includedirs = ["include"]

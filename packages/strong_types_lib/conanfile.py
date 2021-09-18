from conans import ConanFile, CMake
import os


class StrongTypesLibConan(ConanFile):
    name = "strong_types_lib"
    version = "1.0.0"
    license = "MIT"
    author = "Sebastian Bergt sebastian@bergt.org"
    url = "<Package recipe repository url here, for issues about the package>"
    description = "Tiny helper library to to enhance type safety and let the depending code be more expressive"
    topics = ("strong types", "utility", "type safety")
    settings = "os", "compiler", "build_type", "arch"
    generators = ["cmake", "cmake_find_package"]

    exports_sources = "src/**"

    options = {"with_tests": [True, False], "with_coverage": [True, False]}
    default_options = {"with_tests": True, "with_coverage": False, "catch2:with_main": True}

    def build_requirements(self):
        self.build_requires("catch2/2.13.7")

    def _report_coverage(self):
        self.run("gcovr -r . --html --html-details -o coverage.html".split())
        cwd = os.getcwd()
        self.output.highlight("coverage report was created here: " + cwd + "/coverage.html")

    def build(self):
        cmake = CMake(self)
        cmake.definitions["WITH_TESTS"] = self.options.with_tests
        if self.options.with_coverage:
            cmake.definitions["CONAN_CXX_FLAGS"] = "--coverage -g -O0"
        cmake.configure(source_folder="src")
        cmake.build()
        if self.options.with_tests:
            cmake.test()
            if self.options.with_coverage:
                self._report_coverage()
        cmake.install()

    def package_id(self):
        self.info.header_only()

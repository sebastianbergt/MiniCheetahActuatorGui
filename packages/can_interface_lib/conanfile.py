from conans import ConanFile, CMake, tools


class CanInterfaceLibConan(ConanFile):
    name = "can_interface_lib"
    version = "1.0.0"
    license = "MIT"
    author = "Sebastian Bergt sebastian@bergt.org"
    url = "<Package recipe repository url here, for issues about the package>"
    description = "Helper to platform abstract CAN interface"
    topics = ("CAN", "communication")
    settings = "os", "compiler", "build_type", "arch"
    generators = ["cmake", "cmake_find_package"]

    exports_sources = "src/**"

    options = {"with_tests": [True, False]}
    default_options = {"with_tests": True, "catch2:with_main": True}

    def build_requirements(self):
        self.build_requires("catch2/2.13.7")
        self.build_requires("trompeloeil/41")
        self.build_requires("strong_types_lib/1.0.0")

    def build(self):
        cmake = CMake(self)
        cmake.definitions["WITH_TESTS"] = self.options.with_tests
        cmake.configure(source_folder="src")
        cmake.build()
        if self.options.with_tests:
            cmake.test()
        cmake.install()

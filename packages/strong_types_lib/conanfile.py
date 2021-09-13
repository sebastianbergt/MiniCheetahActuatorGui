from conans import ConanFile, CMake, tools


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

    options = {"with_tests": [True, False]}
    default_options = {"with_tests": True, "catch2:with_main": True}

    def build_requirements(self):
        self.build_requires("catch2/2.13.7")

    def build(self):
        cmake = CMake(self)
        cmake.definitions["WITH_TESTS"] = self.options.with_tests
        cmake.configure(source_folder="src")
        cmake.build()
        if self.options.with_tests:
            cmake.test()
        cmake.install()

    def package_id(self):
        self.info.header_only()

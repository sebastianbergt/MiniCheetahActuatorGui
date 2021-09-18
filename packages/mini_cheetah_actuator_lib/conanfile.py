from conans import ConanFile, CMake, tools


class MiniCheetahActuatorLibConan(ConanFile):
    name = "mini_cheetah_actuator_lib"
    version = "0.0.1"
    license = "<Put the package license here>"
    author = "<Put your name here> <And your email here>"
    url = "<Package recipe repository url here, for issues about the package>"
    description = "<Description of MiniCheetahActuatorLib here>"
    topics = ("<Put some tag here>", "<here>", "<and here>")
    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False], "fPIC": [True, False], "with_tests": [True, False]}
    default_options = {"shared": False, "fPIC": True, "with_tests": True}
    generators = ["cmake", "cmake_find_package"]

    def config_options(self):
        if self.settings.os == "Windows":
            del self.options.fPIC

    def build_requirements(self):
        self.build_requires("catch2/2.13.7#5ab84d6acfe1f23c4fae0ab88f26e3a396351ac9")
        self.build_requires("trompeloeil/41#5ab84d6acfe1f23c4fae0ab88f26e3a396351ac9")

    def requirements(self):
        self.requires("strong_types_lib/1.0.0")

    def build(self):
        cmake = CMake(self)
        cmake.definitions["WITH_TESTS"] = self.options.with_tests
        cmake.configure(source_folder="src")
        cmake.build()
        if self.options.with_tests:
            cmake.test()

    def package(self):
        self.copy("*.h", dst="include", src="hello")
        self.copy("*hello.lib", dst="lib", keep_path=False)
        self.copy("*.dll", dst="bin", keep_path=False)
        self.copy("*.so", dst="lib", keep_path=False)
        self.copy("*.dylib", dst="lib", keep_path=False)
        self.copy("*.a", dst="lib", keep_path=False)

    def package_info(self):
        self.cpp_info.libs = ["mini_cheetah_actuator_lib"]

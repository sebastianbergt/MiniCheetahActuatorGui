from conans import ConanFile, CMake


class MiniCheetahActuatorGuiConan(ConanFile):
    name = "mini_cheetah_actuator_gui"
    version = "1.0.0"
    license = "MIT"
    author = "Sebastian Bergt sebastian@bergt.org"
    url = "https://github.com/sebastianbergt/MiniCheetahActuatorGui"
    description = (
        "GUI application to simplify work with Actuators based on MiniCheetahActuator"
    )
    topics = ("motor control", "ui", "debug")
    settings = "os", "compiler", "build_type", "arch"
    generators = ["cmake", "cmake_find_package"]

    exports_sources = "src/**"

    options = {}
    default_options = {}

    _cmake = None

    def requirements(self):
        self.requires("glew/2.2.0")
        self.requires("glfw/3.3.4")
        self.requires("imgui/1.85")
        self.requires("mini_cheetah_actuator_lib/1.0.0")

    def _configure_cmake(self):
        if self._cmake:
            return self._cmake
        self._cmake = CMake(self)
        self._cmake.definitions["CONAN_CXX_FLAGS"] = "-Wall -Wextra -Wpedantic -Werror "
        self._cmake.configure(source_folder="src")
        return self._cmake

    def build(self):
        cmake = self._configure_cmake()
        cmake.build()

    def package(self):
        self.copy(pattern="LICENSE", dst="licenses", src="src")
        cmake = self._configure_cmake()
        cmake.install()

    def imports(self):
        self.copy("license*", src=".", dst="licenses", folder=True, ignore_case=True)
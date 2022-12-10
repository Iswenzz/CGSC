from conans import ConanFile, CMake


class CGSC(ConanFile):
    name = "CGSC"
    version = "1.2.0"
    license = "LICENCE"
    url = "https://github.com/Iswenzz/CGSC"
    description = "Call of Duty 4X source extension that adds new utilities to extend the server and the creation of new plugins."

    generators = "cmake"
    exports_sources = "LICENSE", "README.md", "CMakeLists.txt", ".conan/*"
    requires = ["libuv/1.44.2"]

    settings = "os", "arch", "compiler", "build_type"
    options = {"enable_testing": [True, False], "plugin": [True, False]}
    default_options = {"enable_testing": False, "plugin": True}
    no_copy_source = True

    def build_requirements(self):
        if self.options.enable_testing:
            self.build_requires("greatest/1.5.0", force_host_context=True)

    def build(self):
        cmake_release = CMake(self, build_type="Release")
        cmake_release.definitions["CONAN_DISABLE_CHECK_COMPILER"] = True
        if self.options.plugin:
            cmake_release.definitions["PLUGIN"] = True
        else:
            cmake_release.definitions["COD4X18UPDATE"] = True
        cmake_release.configure()
        cmake_release.build()

    def export_sources(self):
        self.copy("*.h", dst="CGSC", keep_path=True)

    def package(self):
        self.copy("LICENSE, README.md")
        self.copy("*.h", dst="include")

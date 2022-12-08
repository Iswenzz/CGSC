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
    options = {"enable_testing": [True, False]}
    default_options = {"enable_testing": False}
    no_copy_source = True

    def build_requirements(self):
        if self.options.enable_testing:
            self.build_requires("greatest/1.5.0", force_host_context=True)

    def export_sources(self):
        self.copy("cgsc.h", dst="CGSC")
        self.copy("plugin.h", dst="CGSC")
        self.copy("*.h", src="extensions", dst="CGSC/extensions")
        self.copy("*.h", src="sys", dst="CGSC/sys")
        self.copy("*.h", src="utils", dst="CGSC/utils")
        self.copy("*.h", src="versions", dst="CGSC/versions")

    def build(self):
        cmake_release = CMake(self, build_type="Release")
        cmake_release.definitions["CONAN_DISABLE_CHECK_COMPILER"] = True
        cmake_release.definitions["PLUGIN"] = True
        cmake_release.configure()
        cmake_release.build()

    def package(self):
        self.copy("LICENSE, README.md")
        self.copy("*.h", dst="include", excludes="cod4x")
        self.copy("*.a", dst="lib", keep_path=False)
        self.copy("*.lib", dst="lib", keep_path=False)
        self.copy("*.dll", dst="bin", keep_path=False)
        self.copy("*.so", dst="bin", keep_path=False)

from conans import ConanFile, CMake, tools

class CGSC(ConanFile):
	name = "CGSC"
	version = "1.1.1"
	license = "LICENCE"
	url = "https://github.com/Iswenzz/CGSC"
	description = "Call of Duty 4X source extension that adds new features for creating plugins not offered in the original source."

	generators = "cmake"
	exports_sources = "LICENSE", "README.md", "CMakeLists.txt", "cgsc.h"

	settings = "os", "arch", "compiler", "build_type"
	options = {"enable_testing": [True, False]}
	default_options  = {"enable_testing": False}

	def build_requirements(self):
		if self.options.enable_testing:
			self.build_requires("greatest/1.5.0", force_host_context=True)

	def export_plugin_api(self):
		self.copy("pinc.h", src="../../plugins", dst="cod4x/plugins")
		self.copy("declarations.h", src="../../plugins", dst="cod4x/plugins")
		self.copy("plugin_declarations.h", src="../../plugins", dst="cod4x/plugins")
		self.copy("callback_declarations.h", src="../../plugins", dst="cod4x/plugins")
		self.copy("function_declarations.h", src="../../plugins", dst="cod4x/plugins")

	def export_sources(self):
		self.copy(".conan/*")
		self.copy("api/*")
		self.copy("*.h", src="extensions", dst="extensions")
		self.export_plugin_api()

	def build(self):
		tools.patch(patch_file=".conan/patches/CMakeLists.txt.patch")
		cmake_release = CMake(self, build_type="Release")
		cmake_release.configure()
		cmake_release.build()

	def package(self):
		self.copy("LICENSE, README.md")
		self.copy("*.h", dst="include", excludes="cod4x")
		self.copy("*.a", dst="lib", keep_path=False)
		self.copy("*.lib", dst="lib", keep_path=False)
		self.copy("*.dll", dst="bin", keep_path=False)
		self.copy("*.so", dst="bin", keep_path=False)

	def package_info(self):
		self.cpp_info.libs = ["CGSC"]

from conans import ConanFile, CMake

class CGSC(ConanFile):
	name = "CGSC"
	version = "1.0.0"
	license = "LICENCE"
	url = "https://github.com/Iswenzz/CGSC"
	description = "Call of Duty 4X source extension that adds new features for creating plugins not offered in the original source."
	exports_sources = "*"
	no_copy_source = True

	def package(self):
		self.copy("*.h", dst="include", src=".")

	def package_id(self):
		self.info.header_only()

import os
from conan import ConanFile
from conan.tools.files import copy
from conan.tools.cmake import CMakeDeps
from conan.tools.cmake import CMake, CMakeToolchain, cmake_layout


class SgltkRecipe(ConanFile):
	name = "sgltk"
	version = "0.6.1"
	license = "Zlib"
	url = "https://github.com/pyth/sgltk"
	description = "A collection of easy to use OpenGL tools."
	settings = "os", "compiler", "build_type", "arch"
	options = {
		"shared": [True, False],
		"fPIC": [True, False],
		"with_sdl_ttf": [True, False],
		"with_assimp": [True, False],
	}
	default_options = {
		"shared": True,
		"fPIC": True,
		"with_sdl_ttf": True,
		"with_assimp": True,
	}
	exports_sources = (
		"CMakeLists.txt",
		"src/*",
		"include/*",
		"doc/*",
		"config.h.in",
		"cmake_uninstall.cmake.in",
		"sgltkConfig.cmake.in",
		"sgltkConfigVersion.cmake.in",
		"LICENCE",
	)

	def requirements(self):
		self.requires("glm/1.0.1")
		self.requires("glew/2.2.0")
		self.requires("sdl/2.28.3")
		self.requires("sdl_image/2.8.2")
		if self.options.with_sdl_ttf:
			self.requires("sdl_ttf/2.24.0")
		if self.options.with_assimp:
			self.requires("assimp/5.4.3")

	def layout(self):
		cmake_layout(self)

	def generate(self):
		deps = CMakeDeps(self)
		deps.generate()

		tc = CMakeToolchain(self)
		tc.generate()

	def build(self):
		cm = CMake(self)
		cm.configure()
		cm.build()

	def package(self):
		cm = CMake(self)
		cm.install()
		copy(self, "LICENCE", src=self.source_folder, dst=os.path.join(self.package_folder, "licenses"))

	def package_info(self):
		self.cpp_info.libs = ["sgltk" if self.options.shared else "sgltk_static"]
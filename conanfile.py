import os
from conans import ConanFile, CMake, tools


def get_version():
    with open(os.path.join(os.path.dirname(__file__), 'version'), 'r') as f:
        content = f.read()
        try:
            content = content.decode()
        except AttributeError:
            pass
        return content.strip()


class HttbConan(ConanFile):
    name = "httb"
    version = get_version()
    license = "MIT"
    author = "Eduard Maximovich edward.vstock@gmail.com"
    url = "https://github.com/edwardstock/httb"
    description = "Lightweight C++ HTTP Client based on Boost.Beast"
    topics = ("boost", "http", "cpp-http", "cpp-http-client", "http-client", "boost-http", "boost-beast")
    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False]}
    default_options = {
        "shared": False,
        "OpenSSL:shared": False,
        "boost_system:shared": False,
        "boost_filesystem:shared": False,
        "boost_regex:shared": False
    }
    exports = "version"
    exports_sources = (
        "modules/*",
        "include/*",
        "tests/*",
        "src/*",
        "CMakeLists.txt",
        "conanfile.py",
        "conanfile.txt",
        "LICENSE",
        "README.md"
    )
    generators = "cmake"
    default_user = "scatter"
    default_channel = "latest"

    requires = (
        "OpenSSL/1.1.1a@conan/stable",
        "toolboxpp/2.2.0@scatter/latest",
        "boost/1.70.0@conan/stable",
        ("gtest/1.8.1@bincrafters/stable", "private"),
    )

    def source(self):
        if "CONAN_LOCAL" not in os.environ:
            self.run("rm -rf *")
            self.run("git clone https://github.com/edwardstock/httb.git .")

    def build(self):
        cmake = CMake(self)
        cmake.configure(defs={'HTTB_TEST': 'Off', 'CMAKE_BUILD_TYPE': 'Release'})
        cmake.build()

    def package(self):
        self.copy("*.h", dst="include", src="include", keep_path=True)
        self.copy("*httb.lib", dst="lib", keep_path=False)
        self.copy("*.dll", dst="bin", keep_path=False)
        self.copy("*.so", dst="lib", keep_path=False)
        self.copy("*.dylib", dst="lib", keep_path=False)
        self.copy("*.a", dst="lib", keep_path=False)

    def package_info(self):
        self.cpp_info.libs = self.collect_libs()

    def test(self):
        cmake = CMake(self)
        cmake.configure(defs={'HTTB_TEST': 'On'})
        cmake.build(target="httb-test")
        self.run("bin/httb-test")

    def package_info(self):
        self.cpp_info.libs = ["httb"]

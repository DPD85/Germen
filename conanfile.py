from conan import ConanFile
from conan.tools.cmake import cmake_layout, CMakeDeps
from conan.tools.system.package_manager import Apt

class ConanApplication(ConanFile):
    package_type = "application"
    settings = "os", "compiler", "build_type", "arch"

    def layout(self):
        cmake_layout(self)
        self.folders.generators = "Conan"

    def generate(self):
        cmake = CMakeDeps(self)
        cmake.generate()

    def system_requirements(self):
        packages = [
            # SDL3
            "libsdl3-dev", # Su Linux uso SDL3 dai pacchetti di sistema poiché il pacchetto Conan non funziona con Wayland.
            # Vulkan
            "libvulkan-dev",
            "vulkan-utility-libraries-dev",
            # Graphviz
            "bison",
            "flex",
            "python3"
        ]
        apt = Apt(self)
        result = apt.install(packages)
        if result is not None and result != 0:
            apt.install(packages, update=True)

    def requirements(self):
        # ----- Lista dipendenze indirette: devono sostituire quelle usate dalle altre dipendenze.

        # Sostituisce la versione 2.13.2 usata da PlutoSVG.
        self.requires("freetype/2.14.1", force=True)

        # ----- Lista dipendenze dirette

        self.requires("boost/1.88.0")
        self.requires("nlohmann_json/3.12.0")
        self.requires("platformfolders/4.3.0")
        self.requires("plutosvg/0.0.7")
        # Il pacchetto conan di SDL3 è rotto e non funziona con Wayland su Linux per tanto lo utilizzo solamente su Windows.
        if self.settings.os == "Windows":
            self.requires("sdl/3.2.20")


import os
import subprocess
import platform

from setup_python import PythonConfiguration as PythonRequirements

# Make sure everything we need for the setup is installed
PythonRequirements.Validate()

from setup_premake import PremakeConfiguration as PremakeRequirements
from setup_vulkan import VulkanConfiguration as VulkanRequirements
os.chdir('./../') # Change from devtools/scripts directory to root

premakeInstalled = PremakeRequirements.Validate()
VulkanRequirements.Validate()

print("\nUpdating submodules...")
subprocess.call(["git", "submodule", "update", "--init", "--recursive"])

if (premakeInstalled):
    if platform.system() == "Windows":
        print("\nRunning premake...")
        subprocess.call([os.path.abspath("./scripts/WinGenProject.bat"), "nopause"])

    print("\nSetup completed!")
else:
    print("GitGud requires Premake to generate project files.")
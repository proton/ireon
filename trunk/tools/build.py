from distutils.core import setup
import py2exe

setup(
    console=[
        {
            "script": "sysinfo/sysinfo.py",
            "icon_resources": [(1, "common\\images\\applications-system-48.ico")]
        }
    ],
    windows=[
        {
            "script": "launcher/launcher.pyw",
            "icon_resources": [(1, "common\\images\\system-software-update-48.ico")]
        }
    ]
)
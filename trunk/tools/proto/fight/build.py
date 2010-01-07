from distutils.core import setup
import py2exe
import sys
sys.path += ['..\\..\\common\\py-modules\\PHPSerialize\\','..\\..\\common\\py-modules\\config-0.3.6\\']

class Target:
    def __init__(self, **kw):
        self.__dict__.update(kw)
        # for the versioninfo resources
        self.version = "0.5.0"
        self.company_name = "ireon.org"
        self.copyright = "2006 ireon.org developers council"
        self.name = "combat system prototype"

################################################################
# A program using wxPython

# The manifest will be inserted as resource into test_wx.exe.  This
# gives the controls the Windows XP appearance (if run on XP ;-)
#
# Another option would be to store it in a file named
# test_wx.exe.manifest, and copy it with the data_files option into
# the dist-dir.
#
manifest_template = '''
<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<assembly xmlns="urn:schemas-microsoft-com:asm.v1" manifestVersion="1.0">
<assemblyIdentity
    version="5.0.0.0"
    processorArchitecture="x86"
    name="%(prog)s"
    type="win32"
/>
<description>%(prog)s Program</description>
<dependency>
    <dependentAssembly>
        <assemblyIdentity
            type="win32"
            name="Microsoft.Windows.Common-Controls"
            version="6.0.0.0"
            processorArchitecture="X86"
            publicKeyToken="6595b64144ccf1df"
            language="*"
        />
    </dependentAssembly>
</dependency>
</assembly>
'''

RT_MANIFEST = 24

fight_proto_client = Target(
    # used for the versioninfo resource
    description = "combat system prototype",

    # what to build
    script = "fight_proto_client.pyw",
    other_resources = [(RT_MANIFEST, 1, manifest_template % dict(prog="fight_proto_client"))],
##    icon_resources = [(1, "icon.ico")],
    dest_base = "fight_proto_client")

setup(
    windows = [fight_proto_client],
    data_files=[("",["combat_prototype.xrc", "dist/fight_proto_client.cfg", "../../common/images/applications-prototype-16.ico"])]
)
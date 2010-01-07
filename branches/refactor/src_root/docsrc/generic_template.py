# ireon.org
# this is template file that is called from client sconscript files
#
# sets variable and cleanup targets

docBuildEnv['SRCDIR_'] = docBuildEnv.Dir('').srcnode().get_abspath()

docBuildEnv.Alias('docs-cleanup', \
docBuildEnv.Alias('docs-'+documentCategory_+'-cleanup', \
docBuildEnv.Alias('docs-'+documentCategory_+'-'+thisDocument_+'-cleanup', \
docBuildEnv.Command('delete.dir', '', docBuildEnv.Action(docBuildEnv.cleanupDir,docBuildEnv.cleanupDirStr)) )))

docBuildEnv.Alias('install-docs-cleanup', \
docBuildEnv.Alias('install-docs-'+documentCategory_+'-cleanup', \
docBuildEnv.Alias('install-docs-'+documentCategory_+'-'+thisDocument_+'-cleanup', \
docBuildEnv.Command(docBuildEnv['installPath_']+'/doc/'+lang_+'/'+documentCategory_+'/'+thisDocument_+'/delete.dir', '', docBuildEnv.Action(docBuildEnv.cleanupDir,docBuildEnv.cleanupDirStr)) )))

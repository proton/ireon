import string
import os
from copy import copy
import stat

execfile('data_stream.py')
execfile('log_manager.py')
execfile('colour_value.py')
execfile('material_serializer.py')
execfile('parsers.py')
execfile('constants.py')

materialRoot = '..\..\data\materials\scripts'

filesInRoot = os.listdir(materialRoot)
files = []
for i in filesInRoot:
    fullName = materialRoot +'\\' + i
    st_mode = os.lstat(fullName)[stat.ST_MODE]
    isFile = stat.S_ISREG(st_mode)
    if  isFile == True:
        files.append(fullName)    


for source in files:
    print source,
    pFile = open( source, "r" )
    if pFile == None:
        print "Don't exist file or don't open him ", source
    dataStream = FileHandleDataStream(source, pFile)
    #print dataStream.mSize
    serializer = MaterialSerializer()
    serializer.parseScript(dataStream, 'test')
    dataStream.close()
    print '  [  OK  ]'


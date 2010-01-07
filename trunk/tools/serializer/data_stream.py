'''
/** General purpose class used for encapsulating the reading of data.
@remarks
    This class performs basically the same tasks as std::basic_istream, 
    except that it does not have any formatting capabilities, and is
    designed to be subclassed to receive data from multiple sources,
    including libraries which have no compatiblity with the STLs
    stream interfaces. As such, this is an abstraction of a set of 
    wrapper classes which pretend to be standard stream classes but 
    can actually be implemented quite differently. 
@par
    Generally, if a plugin or application provides an ArchiveFactory, 
    it should also provide a DataStream subclass which will be used
    to stream data out of that Archive implementation, unless it can 
    use one of the common implementations included.
@note
    Ogre makes no guarantees about thread safety, for performance reasons.
    If you wish to access stream data asynchronously then you should
    organise your own mutexes to avoid race conditions. 
*/'''
OGRE_STREAM_TEMP_SIZE = 128

class DataStream:
    ## The name (e.g. resource name) that can be used to identify the source fot his data (optional)
    mName = ''  
    ## Size of the data in the stream (may be 0 if size cannot be determined)
    mSize = 0
    ## Temporary buffer area used for formatted read
    #mTmpArea = [] #OGRE_STREAM_TEMP_SIZE

    ## Constructor for creating unnamed streams
    def __init__(self):
        mSize = 0
    ## Constructor for creating named streams
    def __init__(self, name):
       mSize = 0
       mName = name
    
    ## Returns the name of the stream, if it has one.
    def getName(self):
        return self.mName
      
    #/** Read the requisite number of bytes from the stream, 
    #    stopping at the end of the file.
    #@param buf Reference to a buffer pointer
    #@param count Number of bytes to read
    #@returns The number of bytes read
    def read(self, count):
        pass 
    
    '''/** Get a single line from the stream.
    @remarks
        The delimiter character is not included in the data
        returned, and it is skipped over so the next read will occur
        after it. The buffer contents will include a
        terminating character.
    @param buf Reference to a buffer pointer
    @param maxCount The maximum length of data to be read, excluding the terminating character
    @param delim The delimiter to stop at
    @returns The number of bytes read, excluding the terminating character'''
    def readLine(self, maxCount ):
        pass
    
    '''/** Returns a String containing the next line of data, optionally 
        trimmed for whitespace. 
    @remarks
        This is a convenience method for text streams only, allowing you to 
        retrieve a String object containing the next line of data. The data
        is read up to the next newline character and the result trimmed if
        required.
    @param 
        trimAfter If true, the line is trimmed for whitespace (as in 
        String.trim(true,true)) '''
    def getLine(self, trimAfter = True ):
        str = ''
        c = OGRE_STREAM_TEMP_SIZE-1
        #// Keep looping while not hitting delimiter
        while (c == OGRE_STREAM_TEMP_SIZE-1):
            c, mTmpArea = self.readLine( OGRE_STREAM_TEMP_SIZE-1)
            str += mTmpArea
        if (trimAfter == True):
            str = str.strip()
        return str
    '''/** Returns a String containing the entire stream. 
    @remarks
        This is a convenience method for text streams only, allowing you to 
        retrieve a String object containing all the data in the stream.'''
    def getAsString(self):
        #// Read the entire buffer
        c, pBuf = read(self.mSize)
        return pBuf
    '''/** Skip a single line from the stream.
    @param delim The delimiter(s) to stop at
    @returns The number of bytes skipped'''
    def skipLine(self):
        pass

    '''/** Skip a defined number of bytes. This can also be a negative value, in which case
    the file pointer rewinds a defined number of bytes. */'''
    def skip(self, count):
        pass

    #/** Repositions the read point to a specified byte.
    def seek(self, pos ):
        pass
    
    #/** Returns the current byte offset from beginning */
    def tell(self):
        pass

    #/** Returns true if the stream has reached the end.
    def eof(self):
        pass

    '''/** Returns the total size of the data to be read from the stream, 
        or 0 if this is indeterminate for this stream. '''
    def size(self):
        return self.mSize

    #/** Close the stream; this makes further operations invalid. */
    def close(self):
        pass
 

################################################################################

SEEK_SET = 0
SEEK_CUR = 1
SEEK_END = 2
'''/** Common subclass of DataStream for handling data from C-style file 
    handles.
@remarks
    Use of this class is generally discouraged; if you want to wrap file
    access in a DataStream, you should definitely be using the C++ friendly
    FileStreamDataStream. However, since there are quite a few applications
    and libraries still wedded to the old FILE handle access, this stream
    wrapper provides some backwards compatibility.
*/'''
class FileHandleDataStream( DataStream ):
    mFileHandle = None
    
    ## Create stream from a C file handle
    def __init__(self, handle):
        DataStream.__init__(self)
        self.mFileHandle = handle
        #// Determine size
        self.mSize = 0
        self.mFileHandle.seek(0, SEEK_END)
        self.mSize = self.tell()
        self.mFileHandle.seek(0, SEEK_SET)
        
        
    ## Create named stream from a C file handle
    def __init__(self, name, handle):
        DataStream.__init__(self,name)
        self.mFileHandle = handle
        # Determine size
        self.mFileHandle.seek(0, SEEK_END)
        self.mSize = self.tell()
        self.mFileHandle.seek(0, SEEK_SET)
    def __del__(self):
        self.close()
    
    
    #/** @copydoc DataStream::read
    def read(self, count):
        buf = self.mFileHandle.read( count )
        return len(buf), buf
        
    #/** @copydoc DataStream::readLine
    def readLine(self, maxCount):
        buf = self.mFileHandle.readline(maxCount)
        return len(buf), buf
        
    #/** @copydoc DataStream::skipLine
    def skipLine(self):
        #// Re-use readLine, but don't copy data
        buf = self.mFileHandle.readline()
        
    #/** @copydoc DataStream::skip
    def skip(self, count):
        self.mFileHandle.seek(count, SEEK_CUR)
        
    #/** @copydoc DataStream::seek
    def seek( self, pos ):
        self.mFileHandle.seek( pos, SEEK_SET)
        
    #/** @copydoc DataStream::tell
    def tell( self ):
        return self.mFileHandle.tell()
        
    #/** @copydoc DataStream::eof
    def eof(self):
        return (self.tell() == self.mSize)
       
    #/** @copydoc DataStream::close
    def close(self):
        if self.mFileHandle != 0:
            self.mFileHandle.close()
            self.mFileHandle = 0

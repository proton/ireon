



class MaterialScriptProgramDefinition:
    name = ''
    progType = 0 #GPT
    language = ''
    source = ''
    syntax = ''
    supportsSkeletalAnimation = False # bool
    customParameters = {}
    
#    /** Struct for holding the script context while parsing. */
class MaterialScriptContext: 
    section = None
    groupName = ''
    material = None
    technique = None
    _pass = None
    textureUnit = None
    program = [] #// used when referencing a program, not when defining it
    isProgramShadowCaster = False #// when referencing, are we in context of shadow caster
    isProgramShadowReceiver = False #// when referencing, are we in context of shadow caster
    programParams = []
    programDef = None #// this is used while defining a program
    techLev = 0 #//Keep track of what tech, pass, and state level we are in
    passLev = 0
    stateLev = 0
    defaultParamLines = []
    #// Error reporting state
    lineNo = 0
    filename = ''






    #/// Function def for material attribute parser; return value determines if the next line should be {
    #typedef bool (*ATTRIBUTE_PARSER)(String& params, MaterialScriptContext& context);

#    /** Class for serializing Materials to / from a .material script.*/
class MaterialSerializer:
    #/// Keyword-mapped attribute parsers.
    #typedef std::map<String, ATTRIBUTE_PARSER> AttribParserList;

    
    mScriptContext = MaterialScriptContext()
    
    def __init__(self):
        #// Set up root attribute parsers
        self.mRootAttribParsers["material"] = parseMaterial
        self.mRootAttribParsers["vertex_program"] = parseVertexProgram
        self.mRootAttribParsers["fragment_program"] = parseFragmentProgram
        #// Set up material attribute parsers
        self.mMaterialAttribParsers["lod_distances"] = parseLodDistances
        self.mMaterialAttribParsers["receive_shadows"] = parseReceiveShadows
        self.mMaterialAttribParsers["transparency_casts_shadows"] = parseTransparencyCastsShadows
        self.mMaterialAttribParsers["technique"] = parseTechnique
        #// Set up technique attribute parsers
        self.mTechniqueAttribParsers["lod_index"] = parseLodIndex
        self.mTechniqueAttribParsers["pass"] = parsePass
        #// Set up pass attribute parsers
        self.mPassAttribParsers["ambient"] = parseAmbient
        self.mPassAttribParsers["diffuse"] = parseDiffuse
        self.mPassAttribParsers["specular"] = parseSpecular
        self.mPassAttribParsers["emissive"] = parseEmissive
        self.mPassAttribParsers["scene_blend"] = parseSceneBlend
        self.mPassAttribParsers["depth_check"] = parseDepthCheck
        self.mPassAttribParsers["depth_write"] = parseDepthWrite
        self.mPassAttribParsers["depth_func"] = parseDepthFunc
        self.mPassAttribParsers["alpha_rejection"] = parseAlphaRejection
        self.mPassAttribParsers["colour_write"] = parseColourWrite
        self.mPassAttribParsers["cull_hardware"] = parseCullHardware
        self.mPassAttribParsers["cull_software"] = parseCullSoftware
        self.mPassAttribParsers["lighting"] = parseLighting
        self.mPassAttribParsers["fog_override"] = parseFogging
        self.mPassAttribParsers["shading"] = parseShading
        self.mPassAttribParsers["depth_bias"] = parseDepthBias
        self.mPassAttribParsers["texture_unit"] = parseTextureUnit
        self.mPassAttribParsers["vertex_program_ref"] = parseVertexProgramRef
        self.mPassAttribParsers["shadow_caster_vertex_program_ref"] = parseShadowCasterVertexProgramRef
        self.mPassAttribParsers["shadow_receiver_vertex_program_ref"] = parseShadowReceiverVertexProgramRef     
        self.mPassAttribParsers["fragment_program_ref"] = parseFragmentProgramRef
        self.mPassAttribParsers["max_lights"] = parseMaxLights
        self.mPassAttribParsers["iteration"] = parseIteration
        #// Set up texture unit attribute parsers
        self.mTextureUnitAttribParsers["texture_source"] = parseTextureSource
        self.mTextureUnitAttribParsers["texture"] = parseTexture
        self.mTextureUnitAttribParsers["anim_texture"] = parseAnimTexture
        self.mTextureUnitAttribParsers["cubic_texture"] = parseCubicTexture
        self.mTextureUnitAttribParsers["tex_coord_set"] = parseTexCoord
        self.mTextureUnitAttribParsers["tex_address_mode"] = parseTexAddressMode
        self.mTextureUnitAttribParsers["colour_op"] = parseColourOp
        self.mTextureUnitAttribParsers["colour_op_ex"] = parseColourOpEx
        self.mTextureUnitAttribParsers["colour_op_multipass_fallback"] = parseColourOpFallback
        
        self.mTextureUnitAttribParsers["alpha_op_ex"] = parseAlphaOpEx
        self.mTextureUnitAttribParsers["env_map"] = parseEnvMap
        self.mTextureUnitAttribParsers["scroll"] = parseScroll
        self.mTextureUnitAttribParsers["scroll_anim"] = parseScrollAnim
        self.mTextureUnitAttribParsers["rotate"] = parseRotate
        self.mTextureUnitAttribParsers["rotate_anim"] = parseRotateAnim
        self.mTextureUnitAttribParsers["scale"] = parseScale
        
        self.mTextureUnitAttribParsers["wave_xform"] = parseWaveXform
        self.mTextureUnitAttribParsers["filtering"] = parseFiltering
        self.mTextureUnitAttribParsers["max_anisotropy"] = parseAnisotropy
        
        #// Set up program reference attribute parsers
        self.mProgramRefAttribParsers["param_indexed"] = parseParamIndexed
        self.mProgramRefAttribParsers["param_indexed_auto"] = parseParamIndexedAuto
        self.mProgramRefAttribParsers["param_named"] = parseParamNamed
        self.mProgramRefAttribParsers["param_named_auto"] = parseParamNamedAuto
              
        #// Set up program definition attribute parsers
        self.mProgramAttribParsers["source"] = parseProgramSource
        self.mProgramAttribParsers["syntax"] = parseProgramSyntax
        self.mProgramAttribParsers["includes_skeletal_animation"] = parseProgramSkeletalAnimation
        self.mProgramAttribParsers["default_params"] = parseDefaultParams
       
        #// Set up program default param attribute parsers
        self.mProgramDefaultParamAttribParsers["param_indexed"] = parseParamIndexed
        self.mProgramDefaultParamAttribParsers["param_indexed_auto"] = parseParamIndexedAuto
        self.mProgramDefaultParamAttribParsers["param_named"] = parseParamNamed
        self.mProgramDefaultParamAttribParsers["param_named_auto"] = parseParamNamedAuto
       
        self.mScriptContext.section = MSS_NONE;
        self.mScriptContext.material = None
        self.mScriptContext.technique = 0
        self.mScriptContext._pass = 0
        self.mScriptContext.textureUnit = 0
        self.mScriptContext.program = []
        self.mScriptContext.lineNo = 0
        self.mScriptContext.filename = ""
        self.mScriptContext.techLev = -1
        self.mScriptContext.passLev = -1
        self.mScriptContext.stateLev = -1

        self.mBuffer = ""
    
    
    #        /** Parses a Material script file passed as a stream.
    #        */
     
    def parseScript(self, stream, groupName):
        line = ''
        nextIsOpenBrace = False

        self.mScriptContext.section = MSS_NONE
        self.mScriptContext.material = None
        self.mScriptContext.technique = 0
        self.mScriptContext._pass = 0
        self.mScriptContext.textureUnit = 0
        self.mScriptContext.program = []
        self.mScriptContext.lineNo = 0
        self.mScriptContext.techLev = -1
        self.mScriptContext.passLev = -1
        self.mScriptContext.stateLev = -1
        self.mScriptContext.filename = stream.getName()
        self.mScriptContext.groupName = groupName
        
        while stream.eof() == False:
            line = stream.getLine()
            self.mScriptContext.lineNo += 1
            
            #// DEBUG LINE
            #// LogManager.logMessage("About to attempt line(#" + 
            #//    StringConverter::toString(mScriptContext.lineNo) + "): " + line);

            #// Ignore comments & blanks
            if (len(line) == 0 or line[0:2] == "//") == False:
                if nextIsOpenBrace == True:
                    #// NB, parser will have changed context already
                    if line != "{":
                        logParseError("Expecting '{' but got '" +    \
                            line + "' instead.", self.mScriptContext)
                    nextIsOpenBrace = False
                else:
                    nextIsOpenBrace = self.parseScriptLine(line)
   
        #// Check all braces were closed
        if (self.mScriptContext.section != MSS_NONE):
           logParseError("Unexpected end of file.", self.mScriptContext)
        
        #// Make sure we invalidate our context shared pointer (don't wanna hold on)
        self.mScriptContext.material = None
    
    '''    /** internal method for parsing a material
        @returns True if it expects the next line to be a { '''
    def parseScriptLine(self, line):
        if self.mScriptContext.section == MSS_NONE:
            if line == "}":
                logParseError("Unexpected terminating brace.", self.mScriptContext)
                return False
            else:
                #// find & invoke a parser
                return self.invokeParser(line, self.mRootAttribParsers); 
            
        elif self.mScriptContext.section == MSS_MATERIAL:
            if line == "}":
                #// End of material
                self.mScriptContext.section = MSS_NONE
                self.mScriptContext.material = None
                #//Reset all levels for next material
                self.mScriptContext.passLev = -1
                self.mScriptContext.stateLev= -1
                self.mScriptContext.techLev = -1
            else:
                #// find & invoke a parser
                return self.invokeParser(line, self.mMaterialAttribParsers) 
        
        elif self.mScriptContext.section == MSS_TECHNIQUE:
            if line == "}":
                #// End of technique
                self.mScriptContext.section = MSS_MATERIAL
                self.mScriptContext.technique = 0
                self.mScriptContext.passLev = -1    #//Reset pass level (yes, the pass level)
            else:
                #// find & invoke a parser
                return self.invokeParser(line, self.mTechniqueAttribParsers) 
           
        elif self.mScriptContext.section == MSS_PASS:
            if line == "}":
                #// End of pass
                self.mScriptContext.section = MSS_TECHNIQUE
                self.mScriptContext._pass = 0
                self.mScriptContext.stateLev = -1;   #//Reset state level (yes, the state level)
            else:
                #// find & invoke a parser
                return self.invokeParser(line, self.mPassAttribParsers) 
           
        elif self.mScriptContext.section == MSS_TEXTUREUNIT:
            if line == "}":
                #// End of texture unit
                self.mScriptContext.section = MSS_PASS
                self.mScriptContext.textureUnit = 0
            else:
                #// find & invoke a parser
                return self.invokeParser(line, self.mTextureUnitAttribParsers) 
          
        elif self.mScriptContext.section == MSS_TEXTURESOURCE:
            if line == "}":
                #//End texture source section
                #//Finish creating texture here
                sMaterialName = self.mScriptContext.material.getName()
                #if ExternalTextureSourceManager.getCurrentPlugIn() != 0 :
                #   ExternalTextureSourceManager::getSingleton().getCurrentPlugIn().
                #   createDefinedTexture( sMaterialName, mScriptContext.groupName );
                #//Revert back to texture unit
                self.mScriptContext.section = MSS_TEXTUREUNIT
            else:
                #// custom texture parameter, use original line
                self.parseTextureCustomParameter(line, self.mScriptContext)
          
        elif self.mScriptContext.section == MSS_PROGRAM_REF:
            if line == "}":
                #// End of program
                self.mScriptContext.section = MSS_PASS
                self.mScriptContext.program = []
            else:
                #// find & invoke a parser
                return self.invokeParser(line, self.mProgramRefAttribParsers) 
          
        elif self.mScriptContext.section == MSS_PROGRAM:
            #// Program definitions are slightly different, they are deferred
            #// until all the information required is known
            if line == "}" :
                #// End of program
                self.finishProgramDefinition()
                self.mScriptContext.section = MSS_NONE
                self.mScriptContext.defaultParamLines = []
                self.mScriptContext.programDef = None
            else:
                #// find & invoke a parser
                #// do this manually because we want to call a custom
                #// routine when the parser is not found
                #// First, split line on first divisor only
                splitCmd = line.split( None, 1)
                #// Find attribute parser
                iparser = self.mProgramAttribParsers.has_key(splitCmd[0])
                if iparser == 0:
                    #// custom parameter, use original line
                    parseProgramCustomParameter(line, self.mScriptContext);
                else:
                    if len(splitCmd) >= 2:
                        cmdOne = splitCmd[1]
                    else:
                        cmdOne = ''
                    #// Use parser with remainder
                    return self.mProgramAttribParsers[splitCmd[0]](cmdOne, self.mScriptContext )
           
        elif self.mScriptContext.section == MSS_DEFAULT_PARAMETERS:
            if line == "}":
                #// End of default parameters
                self.mScriptContext.section = MSS_PROGRAM
            else:
                #// Save default parameter lines up until we finalise the program
                self.mScriptContext.defaultParamLines.append(line)
            
        
        return False
    
    #/** internal method for finding & invoking an attribute parser. */
    def invokeParser(self, line, parsers):
        #// First, split line on first divisor only
        splitCmd = line.split(None, 1)

        #// Find attribute parser
        #print 'aaaaa = ', splitCmd, '|',splitCmd[0]
        iparser = parsers.has_key(splitCmd[0])
        if (iparser == 0):
            #// BAD command. BAD!
            logParseError("Unrecognised command: " + splitCmd[0], self.mScriptContext)
            return False
        else:
            cmdOne = ''
            if len(splitCmd) >= 2:
                cmdOne = splitCmd[1]
            #// Use parser, make sure we have 2 params before using splitCmd[1]
            return parsers[splitCmd[0]]( cmdOne, self.mScriptContext )
    #/** Internal method for saving a program definition which has been
    # built up.
    def finishProgramDefinition(self):
        #// Now it is time to create the program and propagate the parameters
        pdef = self.mScriptContext.programDef
        #GpuProgramPtr gp;
        if pdef.language == "asm":
            #// Native assembler
            #// Validate
            if pdef.source == []:
                logParseError("Invalid program definition for " + pdef.name + \
                    ", you must specify a source file.", self.mScriptContext)
            if pdef.syntax == []:
                logParseError("Invalid program definition for " + pdef.name + \
                    ", you must specify a syntax code.", self.mScriptContext)
            #// Create
            #gp = GpuProgramManager::getSingleton().
            #   createProgram(def->name, mScriptContext.groupName, def->source, 
            #       def->progType, def->syntax);
        else:
            #// High-level program
            #// Validate
            if pdef.source == []:
                logParseError("Invalid program definition for " + pdef.name + \
                    ", you must specify a source file.", self.mScriptContext)
            #// Create
            '''try 
                HighLevelGpuProgramPtr hgp = HighLevelGpuProgramManager::getSingleton().
                    createProgram(def->name, mScriptContext.groupName, 
                        def->language, def->progType);
                // Assign to generalised version
                gp = hgp;
                // Set source file
                hgp->setSourceFile(def->source);

                // Set custom parameters
                std::map<String, String>::const_iterator i, iend;
                iend = def->customParameters.end();
                for (i = def->customParameters.begin(); i != iend; ++i)
                {
                    if (!hgp->setParameter(i->first, i->second))
                    {
                        logParseError("Error in program " + def->name + 
                            " parameter " + i->first + " is not valid.", mScriptContext);
                    }
                }
            }
            catch (Exception& e)
            {
                logParseError("Could not create GPU program '"
                    + def->name + "', error reported was: " + e.getFullDescription(), mScriptContext);
                mScriptContext.program.setNull();
                mScriptContext.programParams.setNull();
                return;
            }'''
        #// Set skeletal animation option
        '''gp->setSkeletalAnimationIncluded(def->supportsSkeletalAnimation);
        // set origin
        gp->_notifyOrigin(mScriptContext.filename);

        // Set up to receive default parameters
        if (gp->isSupported() 
            && !mScriptContext.defaultParamLines.empty())
        {
            mScriptContext.programParams = gp->getDefaultParameters();
            mScriptContext.program = gp;
            StringVector::iterator i, iend;
            iend = mScriptContext.defaultParamLines.end();
            for (i = mScriptContext.defaultParamLines.begin();
                i != iend; ++i)
            {
                // find & invoke a parser
                // do this manually because we want to call a custom
                // routine when the parser is not found
                // First, split line on first divisor only
                StringVector splitCmd = StringUtil::split(*i, None, 1);
                // Find attribute parser
                AttribParserList::iterator iparser 
                    = mProgramDefaultParamAttribParsers.find(splitCmd[0]);
                if (iparser != mProgramDefaultParamAttribParsers.end())
                {
                    String cmd = splitCmd.size() >= 2? splitCmd[1]:StringUtil::BLANK;
                    // Use parser with remainder
                    iparser->second(cmd, mScriptContext );
                }

            }
            // Reset
            mScriptContext.program.setNull();
            mScriptContext.programParams.setNull();
        }'''
    
    #/// Parsers for the root of the material script
    mRootAttribParsers = {}
    #/// Parsers for the material section of a script
    mMaterialAttribParsers = {}
    #/// Parsers for the technique section of a script
    mTechniqueAttribParsers = {}
    #/// Parsers for the pass section of a script
    mPassAttribParsers = {}
    #/// Parsers for the texture unit section of a script
    mTextureUnitAttribParsers = {}
    #/// Parsers for the program reference section of a script
    mProgramRefAttribParsers = {}
    #/// Parsers for the program definition section of a script
    mProgramAttribParsers = {}
    #/// Parsers for the program definition section of a script
    mProgramDefaultParamAttribParsers = {}
    '''
        
    #typedef std::multimap<TextureUnitState::TextureEffectType, TextureUnitState::TextureEffect> EffectMap;

    '''
    # convertFiltering(FilterOptions fo);

  
    #/** Queue an in-memory Material to the internal buffer for export.*/
    #void queueForExport(const MaterialPtr& pMat, bool clearQueued = false, bool exportDefaults = false);
    #/** Exports queued material(s) to a named material script file. */
    #void exportQueued(const String& filename);
    #/** Exports an in-memory Material to the named material script file. */
    #void exportMaterial(const MaterialPtr& pMat, const String& filename, bool exportDefaults = false);
    #/** Returns a string representing the parsed material(s) */
    #const String &getQueuedAsString() const;
    #/** Clears the internal buffer */
    #void clearQueue();


    mBuffer = ''
    mDefaults = ''

    def beginSection(self, level):
        self.mBuffer += "\n"
        self.mBuffer += "\t" * level
        mBuffer += "{"

    def endSection(self, level):
        mBuffer += "\n"
        mBuffer += "\t" * level
        mBuffer += "}"
                        
    def writeAttribute(self, level, att):
        mBuffer += "\n"
        mBuffer += "\t" * level
        mBuffer += str(att)

    def writeValue(self, val):
        mBuffer += (" " + str(val) )

    def writeComment(self, level, comment):
        mBuffer += "\n"
        mBuffer += "\t" * level
        mBuffer += "// " + str(comment)




   #//-----------------------------------------------------------------------
    def getQueuedAsString(self ): 
        return self.mBuffer
    
    #//-----------------------------------------------------------------------
    def convertFiltering(self, fo):
        if fo == FO_NONE:
            return "none"
        if fo == FO_POINT:
            return "point"
        if fo == FO_LINEAR:
            return "linear"
        if fo == FO_ANISOTROPIC:
            return "anisotropic"
        
        return "point"
    

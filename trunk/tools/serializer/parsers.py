#    //-----------------------------------------------------------------------
#    // Internal parser methods
#    //-----------------------------------------------------------------------
def logParseError( error, context):
    #// log material name only if filename not specified
    if context.filename == '' and context.material != None:
        LogManager.logMessage("Error in material " + context.material.getName() + \
                               " : " + error)

    elif (context.material == ''):
        LogManager.logMessage(
                    "Error in material " + context.material.getName() +        \
                    " at line " + str(context.lineNo) +                         \
                    " of " + context.filename + ": " + error)
    else:
        LogManager.logMessage(
                    "Error at line " + str(context.lineNo) +                    \
                    " of " + context.filename + ": " + error)

def _parseColourValue( vecparams ):
    return ColourValue(                                                 \
            float(vecparams[0]) ,                                       \
            float(vecparams[1]) ,                                       \
            float(vecparams[2]) ,                                       \
            (len(vecparams) == 4) and float(vecparams[3]) or 1.0 )

def convertFiltering( s ):
        if s == "none":
            return FO_NONE
        elif s == "point":
            return FO_POINT
        elif s == "linear":
            return FO_LINEAR
        elif s == "anisotropic":
            return FO_ANISOTROPIC
        return FO_POINT


def parseAmbient(params, context):
    vecparams = params.split(None)
    #// Must be 1, 3 or 4 parameters 
    if len(vecparams) == 1:
        if vecparams[0] == "vertexcolour":
            pass ##context._pass.setVertexColourTracking(#context._pass.getVertexColourTracking() | TVC_AMBIENT)
        else:
            logParseError(                                                              \
                "Bad ambient attribute, single parameter flag must be 'vertexcolour'",  \
                context)
    elif len(vecparams) == 3 or len(vecparams) == 4:
        pass
        ##context._pass.setAmbient( _parseColourValue(vecparams) );
        ##context._pass.setVertexColourTracking(#context._pass.getVertexColourTracking() & ~TVC_AMBIENT)
    else: 
        logParseError(                                                                  \
            "Bad ambient attribute, wrong number of parameters (expected 1, 3 or 4)",   \
            context)
    return False


def parseDiffuse(params, context):
    vecparams = params.split(None)
    #// Must be 1, 3 or 4 parameters 
    if len(vecparams) == 1:
        if vecparams[0] == "vertexcolour":
           pass
           ##context._pass.setVertexColourTracking(#context._pass.getVertexColourTracking() | TVC_DIFFUSE)
        else:
            logParseError(                                                               \
                "Bad diffuse attribute, single parameter flag must be 'vertexcolour'",   \
                context)

    elif len(vecparams) == 3 or len(vecparams) == 4:
        pass
        ##context._pass.setDiffuse( _parseColourValue(vecparams) )
        ##context._pass.setVertexColourTracking(#context._pass.getVertexColourTracking() & ~TVC_DIFFUSE)
    else:
        logParseError(                                                                 \
            "Bad diffuse attribute, wrong number of parameters (expected 1, 3 or 4)",  \
            context)
    return False
    
def parseSpecular(params, context):
    vecparams =params.split(None)
    #// Must be 2, 4 or 5 parameters 
    if len(vecparams) == 2: 
        if vecparams[0] == "vertexcolour":
            pass
            #context._pass.setVertexColourTracking(#context._pass.getVertexColourTracking() | TVC_SPECULAR)
            #context._pass.setShininess(float(vecparams[1]) )
        else:
            logParseError(                                                                                  \
                "Bad specular attribute, double parameter statement must be 'vertexcolour <shininess>'",    \
                context)
        
    elif len(vecparams) == 4 or len(vecparams) == 5: 
        pass
        ##context._pass.setSpecular(                                  \
        #    float(vecparams[0]),                                   \
        #    float(vecparams[1]),                                  \
        #    float(vecparams[2]),                                 \
        #    (len(vecparams) == 5 and float(vecparams[3])) or 1.0)
        ##context._pass.setVertexColourTracking(#context._pass.getVertexColourTracking() & ~TVC_SPECULAR)
        ##context._pass.setShininess(float(vecparams[-1]) )
    else: 
        logParseError(                                                                  \
            "Bad specular attribute, wrong number of parameters (expected 2, 4 or 5)",  \
            context)
    return False

def parseEmissive(params, context):
    vecparams = params.split(None)
    #// Must be 1, 3 or 4 parameters 
    if len(vecparams) == 1:
        if vecparams[0] == "vertexcolour":
           pass #context._pass.setVertexColourTracking(#context._pass.getVertexColourTracking() | TVC_EMISSIVE)
        else:
            logParseError( \
                "Bad emissive attribute, single parameter flag must be 'vertexcolour'", \
                context);
    elif len(vecparams) == 3 or len(vecparams) == 4:
        pass
        ##context._pass.setSelfIllumination( _parseColourValue(vecparams) )
        ##context._pass.setVertexColourTracking(#context._pass.getVertexColourTracking() & ~TVC_EMISSIVE)
    else:
        logParseError(  \
            "Bad emissive attribute, wrong number of parameters (expected 1, 3 or 4)", \
            context);
    return False

def convertBlendFactor(param):
    if (param == "one"):
        return SBF_ONE
    elif (param == "zero"):
        return SBF_ZERO
    elif (param == "dest_colour"):
        return SBF_DEST_COLOUR
    elif (param == "src_colour"):
        return SBF_SOURCE_COLOUR
    elif (param == "one_minus_dest_colour"):
        return SBF_ONE_MINUS_DEST_COLOUR
    elif (param == "one_minus_src_colour"):
        return SBF_ONE_MINUS_SOURCE_COLOUR
    elif (param == "dest_alpha"):
        return SBF_DEST_ALPHA
    elif (param == "src_alpha"):
        return SBF_SOURCE_ALPHA
    elif (param == "one_minus_dest_alpha"):
        return SBF_ONE_MINUS_DEST_ALPHA
    elif (param == "one_minus_src_alpha"):
        return SBF_ONE_MINUS_SOURCE_ALPHA
    else:
        LogManager.logMessage("Invalid blend factor. convertBlendFactor")

#//-----------------------------------------------------------------------
def parseSceneBlend( params, context):
    params = params.lower()
    vecparams = params.split(None)
    #// Should be 1 or 2 params 
    if len(vecparams) == 1:
        #//simple
        stype = 0
        if (vecparams[0] == "add"):
            stype = SBT_ADD
        elif (vecparams[0] == "modulate"):
            stype = SBT_MODULATE
        elif (vecparams[0] == "colour_blend"):
            stype = SBT_TRANSPARENT_COLOUR
        elif (vecparams[0] == "alpha_blend"):
            stype = SBT_TRANSPARENT_ALPHA
        else:
            logParseError( \
                "Bad scene_blend attribute, unrecognised parameter '" + vecparams[0] + "'", \
                context)
            return False
        ##context._pass.setSceneBlending(stype);

    elif len(vecparams) == 2 :
        #//src/dest
        src, dest = 0, 0

        try: 
            src = convertBlendFactor(vecparams[0]);
            dest = convertBlendFactor(vecparams[1]);
            ##context._pass.setSceneBlending(src,dest);
        except: 
            logParseError("Bad scene_blend attribute, ", context);
        
    else:
        logParseError( \
            "Bad scene_blend attribute, wrong number of parameters (expected 1 or 2)", \
            context)
    return False

#//-----------------------------------------------------------------------
def convertCompareFunction( param ):
    if (param == "always_fail"):
        return CMPF_ALWAYS_FAIL
    elif (param == "always_pass"):
        return CMPF_ALWAYS_PASS
    elif (param == "less"):
        return CMPF_LESS
    elif (param == "less_equal"):
        return CMPF_LESS_EQUAL
    elif (param == "equal"):
        return CMPF_EQUAL
    elif (param == "not_equal"):
        return CMPF_NOT_EQUAL
    elif (param == "greater_equal"):
        return CMPF_GREATER_EQUAL
    elif (param == "greater"):
        return CMPF_GREATER
    else:
        LogManager.logMessage("Invalid compare function  convertCompareFunction")

#//-----------------------------------------------------------------------
def parseDepthCheck( params, context):
    params = params.lower()
    if (params == "on"):
        pass #context._pass.setDepthCheckEnabled(True)
    elif (params == "off"):
        pass #context._pass.setDepthCheckEnabled(False)
    else:
        logParseError( \
        "Bad depth_check attribute, valid parameters are 'on' or 'off'.", \
        context)

    return False

#//-----------------------------------------------------------------------
def parseDepthWrite( params, context):
    params = params.lower()
    if (params == "on"):
        pass ##context._pass.setDepthWriteEnabled(True)
    elif (params == "off"):
        pass ##context._pass.setDepthWriteEnabled(False)
    else:
        logParseError( \
            "Bad depth_write attribute, valid parameters are 'on' or 'off'.", \
            context)
    return False

#//-----------------------------------------------------------------------
def parseDepthFunc(params, context):
    params = params.lower()
    try: 
        func = convertCompareFunction(params)
        #context._pass.setDepthFunction(func)
    except :
        logParseError("Bad depth_func attribute, invalid function parameter.", context)
    
    return False

#//-----------------------------------------------------------------------
def parseColourWrite( params,  context):
    params = params.lower()
    if (params == "on"):
        pass #context._pass.setColourWriteEnabled(True)
    elif (params == "off"):
        pass #context._pass.setColourWriteEnabled(False)
    else:
        logParseError(  \
            "Bad colour_write attribute, valid parameters are 'on' or 'off'.", \
            context)
    return False

#//-----------------------------------------------------------------------
def parseCullHardware(params, context):
    params = params.lower()
    if (params=="none"):
        pass ##context._pass.setCullingMode(CULL_NONE)
    elif (params=="anticlockwise"):
        pass ##context._pass.setCullingMode(CULL_ANTICLOCKWISE)
    elif (params=="clockwise"):
        pass ##context._pass.setCullingMode(CULL_CLOCKWISE)
    else:
        logParseError( \
            "Bad cull_hardware attribute, valid parameters are " \
            "'none', 'clockwise' or 'anticlockwise'.", context)
    return False

#//-----------------------------------------------------------------------
def parseCullSoftware( params,  context):
    params = params.lower()
    if (params=="none"):
        pass ##context._pass.setManualCullingMode(MANUAL_CULL_NONE)
    elif (params=="back"):
        pass ##context._pass.setManualCullingMode(MANUAL_CULL_BACK)
    elif (params=="front"):
        pass ##context._pass.setManualCullingMode(MANUAL_CULL_FRONT)
    else:
        logParseError( \
            "Bad cull_software attribute, valid parameters are 'none', " \
            "'front' or 'back'.", context);
    return False

#//-----------------------------------------------------------------------
def parseLighting( params,  context):
    params = params.lower()
    if (params=="on"):
        pass ##context._pass.setLightingEnabled(True)
    elif (params=="off"):
        pass ##context._pass.setLightingEnabled(False)
    else:
        logParseError( \
            "Bad lighting attribute, valid parameters are 'on' or 'off'.", context)
    return False

#//-----------------------------------------------------------------------
def parseMaxLights( params,  context):
    #context._pass.setMaxSimultaneousLights(atoi(params))
    return False
#//-----------------------------------------------------------------------
def parseIteration( params, context):
    params = params.lower()
    vecparams = params.split( None)
    if len(vecparams) != 1 and len(vecparams) != 2:
        logParseError("Bad iteration attribute, expected 1 or 2 parameters.", context)
        return False
    
    if (vecparams[0]=="once"):
        pass #context._pass.setRunOncePerLight(False)
    elif (vecparams[0]=="once_per_light"):
        if len(vecparams) == 2:
            #// Parse light type
            if (vecparams[1] == "directional"):
               pass #context._pass.setRunOncePerLight(True, True, LT_DIRECTIONAL)
            elif (vecparams[1] == "point"):
                pass #context._pass.setRunOncePerLight(True, True, LT_POINT)
            elif (vecparams[1] == "spot"):
                pass #context._pass.setRunOncePerLight(True, True, LT_SPOTLIGHT)
            else:
                logParseError("Bad iteration attribute, valid values for second parameter "
                    "are 'point' or 'directional' or 'spot'.", context)
        else:
            pass #context._pass.setRunOncePerLight(True, False)
    else:
        logParseError(  \
            "Bad iteration attribute, valid parameters are 'once' or 'once_per_light'.", context)
    return False
#//-----------------------------------------------------------------------
def parseFogging( params,  context):
    params.lower()
    vecparams = params.split( None)
    if (vecparams[0]=="true"):
        #// if True, we need to see if they supplied all arguments, or just the 1... if just the one,
        #// Assume they want to disable the default fog from effecting this material.
        if  len(vecparams) == 8 :
            mFogtype = 0
            if( vecparams[1] == "none" ):
                mFogtype = FOG_NONE
            elif( vecparams[1] == "linear" ):
                mFogtype = FOG_LINEAR
            elif( vecparams[1] == "exp" ):
                mFogtype = FOG_EXP
            elif( vecparams[1] == "exp2" ):
                mFogtype = FOG_EXP2
            else:
                logParseError( \
                    "Bad fogging attribute, valid parameters are " \
                    "'none', 'linear', 'exp', or 'exp2'.", context)
                return False
            '''
            #context._pass.setFog( \
                True, \
                mFogtype, \
                ColourValue( \
                atof(vecparams[2]), \
                atof(vecparams[3]), \
                atof(vecparams[4])), \
                atof(vecparams[5]), \
                atof(vecparams[6]), \
                atof(vecparams[7])
                )'''
        else:
            pass ##context._pass.setFog(True)
        
    elif (vecparams[0]=="false"):
        pass ##context._pass.setFog(False)
    else:
        logParseError( \
            "Bad fog_override attribute, valid parameters are 'true' or 'false'.", \
            context)

    return False

#//-----------------------------------------------------------------------
def parseShading( params, context):
    params = params.lower()
    if (params=="flat"):
        pass #context._pass.setShadingMode(SO_FLAT)
    elif (params=="gouraud"):
        pass #context._pass.setShadingMode(SO_GOURAUD)
    elif (params=="phong"):
        pass #context._pass.setShadingMode(SO_PHONG)
    else:
        logParseError("Bad shading attribute, valid parameters are 'flat', " \
            "'gouraud' or 'phong'.", context)

    return False

#//-----------------------------------------------------------------------
def parseFiltering( params, context):
    params = params.lower()
    vecparams = params.split(None)
    #// Must be 1 or 3 parameters 
    if len(vecparams) == 1:
        #// Simple format
        if (vecparams[0]=="none"):
            context.textureUnit.setTextureFiltering(TFO_NONE)
        elif (vecparams[0]=="bilinear"):
            context.textureUnit.setTextureFiltering(TFO_BILINEAR)
        elif (vecparams[0]=="trilinear"):
            context.textureUnit.setTextureFiltering(TFO_TRILINEAR)
        elif (vecparams[0]=="anisotropic"):
            context.textureUnit.setTextureFiltering(TFO_ANISOTROPIC)
        else:
            logParseError("Bad filtering attribute, valid parameters for simple format are " \
                "'none', 'bilinear', 'trilinear' or 'anisotropic'.", context);
            return False
        
    elif len(vecparams) == 3:
        #// Complex format
        context.textureUnit.setTextureFiltering( \
            convertFiltering(vecparams[0]),  \
            convertFiltering(vecparams[1]),  \
            convertFiltering(vecparams[2]));

    else:
        logParseError( \
            "Bad filtering attribute, wrong number of parameters (expected 1 or 3)", \
            context)
    
    return False
#//-----------------------------------------------------------------------
#// Texture layer attributes
def parseTexture(params, context):
    vecparams = params.split(None)
    if len(vecparams) > 3:
        logParseError("Invalid texture attribute - expected only 1, 2 or 3 parameters.", \
            context)
    tt = TEX_TYPE_2D
    mips = -1 #// When passed to TextureManager::load, this means default to default number of mipmaps
    if len(vecparams) >= 2:
        vecparams[1] = vecparams[1].lower()
        if (vecparams[1] == "1d"):
            tt = TEX_TYPE_1D
        elif (vecparams[1] == "2d"):
            tt = TEX_TYPE_2D
        elif (vecparams[1] == "3d"):
            tt = TEX_TYPE_3D
        elif (vecparams[1] == "cubic"):
            tt = TEX_TYPE_CUBE_MAP
        else:
            logParseError("Invalid texture type - "+vecparams[1]+".", \
            context)
        
    if len(vecparams) >= 3:
        vecparams[2] = vecparams[2].lower()
        if (vecparams[2] == "unlimited"):
            mips = MIP_UNLIMITED
        else:
            mips = atoi(vecparams[2])
        
    #context.textureUnit.setTextureName(vecparams[0], tt, mips)
    return False
#//-----------------------------------------------------------------------
def parseAnimTexture(params, context):
    vecparams = params.split(None)
    numParams = len(vecparams)
    #// Determine which form it is
    #// Must have at least 3 params though
    if (numParams < 3):
        logParseError("Bad anim_texture attribute, wrong number of parameters " \
            "(expected at least 3)", context);
        return False
    if (numParams == 3 and atoi(vecparams[1]) != 0 ):
        #// First form using base name & number of frames
        context.textureUnit.setAnimatedTextureName( \
            vecparams[0], \
            atoi(vecparams[1]), \
            atoi(vecparams[2]))
    else:
        #// Second form using individual names
        context.textureUnit.setAnimatedTextureName( \
            vecparams[0],  \
            numParams-1, \
            atof(vecparams[numParams-1]))
    return False

#//-----------------------------------------------------------------------
def parseCubicTexture( params,  context):

    vecparams = params.split( None)
    numParams = len(vecparams)

    #// Get final param
    useUVW = False
    uvOpt = vecparams[numParams-1]
    uvOpt = uvOpt.lower() 
    if (uvOpt == "combineduvw"):
        useUVW = True
    elif (uvOpt == "separateuv"):
        useUVW = False
    else:
        logParseError("Bad cubic_texture attribute, final parameter must be " \
            "'combinedUVW' or 'separateUV'.", context)
        return False
    #// Determine which form it is
    if (numParams == 2):
        #// First form using base name
        pass
        #context.textureUnit.setCubicTextureName(vecparams[0], useUVW)
    elif (numParams == 7):
        #// Second form using individual names
        #// Can use vecparams[0] as array start point
        pass
        #context.textureUnit.setCubicTextureName(vecparams[0], useUVW)
    else:
        logParseError( \
            "Bad cubic_texture attribute, wrong number of parameters (expected 2 or 7)", \
            context)
        return False
    
    return False
#//-----------------------------------------------------------------------
def parseTexCoord( params, context):
    context.textureUnit.setTextureCoordSet(atoi(params))
    return False

#//-----------------------------------------------------------------------
def parseTexAddressMode( params, context):
    params = params.lower()
    if (params=="wrap"):
        pass #context.textureUnit.setTextureAddressingMode(TAM_WRAP)
    elif (params=="mirror"):
        pass #context.textureUnit.setTextureAddressingMode(TAM_MIRROR)
    elif (params=="clamp"):
        pass #context.textureUnit.setTextureAddressingMode(TAM_CLAMP)
    else:
        logParseError("Bad tex_address_mode attribute, valid parameters are " \
            "'wrap', 'clamp' or 'mirror'.", context)

    return False
#//-----------------------------------------------------------------------
def parseColourOp( params, context):
    params = params.lower()
    if (params=="replace"):
        context.textureUnit.setColourOperation(LBO_REPLACE)
    elif (params=="add"):
        context.textureUnit.setColourOperation(LBO_ADD)
    elif (params=="modulate"):
        context.textureUnit.setColourOperation(LBO_MODULATE)
    elif (params=="alpha_blend"):
        context.textureUnit.setColourOperation(LBO_ALPHA_BLEND)
    else:
        logParseError("Bad colour_op attribute, valid parameters are " \
            "'replace', 'add', 'modulate' or 'alpha_blend'.", context)

    return False
#//-----------------------------------------------------------------------
def parseAlphaRejection( params, context):
    params = params.lower()
    vecparams = params.split( None)
    if len(vecparams) != 2:
        logParseError( \
            "Bad alpha_rejection attribute, wrong number of parameters (expected 2)", \
            context)
        return False
    
    cmpE = 0
    try: 
        cmpE = convertCompareFunction(vecparams[0])
    except :
        logParseError("Bad alpha_rejection attribute, invalid compare function.", context)
        return False
    
    ##context._pass.setAlphaRejectSettings(cmpE, atoi(vecparams[1]))

    return False

#//-----------------------------------------------------------------------
def convertBlendOpEx(param):
    if (param == "source1"):
        return LBX_SOURCE1
    elif (param == "source2"):
        return LBX_SOURCE2
    elif (param == "modulate"):
        return LBX_MODULATE
    elif (param == "modulate_x2"):
        return LBX_MODULATE_X2
    elif (param == "modulate_x4"):
        return LBX_MODULATE_X4
    elif (param == "add"):
        return LBX_ADD
    elif (param == "add_signed"):
        return LBX_ADD_SIGNED
    elif (param == "add_smooth"):
        return LBX_ADD_SMOOTH
    elif (param == "subtract"):
        return LBX_SUBTRACT
    elif (param == "blend_diffuse_alpha"):
        return LBX_BLEND_DIFFUSE_ALPHA
    elif (param == "blend_texture_alpha"):
        return LBX_BLEND_TEXTURE_ALPHA
    elif (param == "blend_current_alpha"):
        return LBX_BLEND_CURRENT_ALPHA
    elif (param == "blend_manual"):
        return LBX_BLEND_MANUAL
    elif (param == "dotproduct"):
        return LBX_DOTPRODUCT
    else:
        LogManager.logmessage("Invalid blend function  convertBlendOpEx")

#//-----------------------------------------------------------------------
def convertBlendSource(param):
    if (param == "src_current"):
        return LBS_CURRENT
    elif (param == "src_texture"):
        return LBS_TEXTURE
    elif (param == "src_diffuse"):
        return LBS_DIFFUSE
    elif (param == "src_specular"):
        return LBS_SPECULAR
    elif (param == "src_manual"):
        return LBS_MANUAL
    else:
        LogManager.logmessage("Invalid blend source  convertBlendSource")

#//-----------------------------------------------------------------------
def parseColourOpEx( params,  context):
    params = params.lower()
    vecparams = params.split( None)
    numParams = len(vecparams)
    
    if (numParams < 3 or numParams > 10):
        logParseError( \
            "Bad colour_op_ex attribute, wrong number of parameters (expected 3 to 10)", \
            context)
        return False
    op = 0
    src1, src2 = 0, 0
    manual = 0.0
    colSrc1 = ColourValue.White
    colSrc2 = ColourValue.White

    try: 
        op = convertBlendOpEx(vecparams[0])
        src1 = convertBlendSource(vecparams[1])
        src2 = convertBlendSource(vecparams[2])

        if (op == LBX_BLEND_MANUAL):
            if (numParams < 4):
                logParseError("Bad colour_op_ex attribute, wrong number of parameters " \
                    "(expected 4 for manual blend)", context)
                return False
            manual = atof(vecparams[3])
        
        if (src1 == LBS_MANUAL):
            parIndex = 3
            if (op == LBX_BLEND_MANUAL):
                parIndex += 1

            if (numParams < parIndex + 3):
                logParseError("Bad colour_op_ex attribute, wrong number of parameters " \
                    "(expected " + str(parIndex + 3) + ")", context)
                return False
            
            #colSrc1.r = StringConverter::parseReal(vecparams[parIndex++]);
            #colSrc1.g = StringConverter::parseReal(vecparams[parIndex++]);
            #colSrc1.b = StringConverter::parseReal(vecparams[parIndex++]);
            if (numParams > parIndex):
                pass #colSrc1.a = StringConverter::parseReal(vecparams[parIndex])
            else:
                pass #colSrc1.a = 1.0f:
            

        if (src2 == LBS_MANUAL):
            parIndex = 3
            if (op == LBX_BLEND_MANUAL):
                parIndex += 1
            if (src1 == LBS_MANUAL):
                parIndex += 3

            if (numParams < parIndex + 3):
                logParseError("Bad colour_op_ex attribute, wrong number of parameters " \
                    "(expected " + str(parIndex + 3) + ")", context)
                return False
            
            #colSrc2.r = StringConverter::parseReal(vecparams[parIndex++]);
            #colSrc2.g = StringConverter::parseReal(vecparams[parIndex++]);
            #colSrc2.b = StringConverter::parseReal(vecparams[parIndex++]);
            if (numParams > parIndex):
                pass #colSrc2.a = StringConverter::parseReal(vecparams[parIndex]);
            else:
                pass #colSrc2.a = 1.0f;
            
    except :
        logParseError("Bad colour_op_ex attribute, ", context)
        return False
    
    #context.textureUnit.setColourOperationEx(op, src1, src2, colSrc1, colSrc2, manual)
    return False

#//-----------------------------------------------------------------------
def parseColourOpFallback( params, context):
    params = params.lower()
    vecparams = params.split( None)
    if len(vecparams) != 2 :
        logParseError("Bad colour_op_multipass_fallback attribute, wrong number " \
            "of parameters (expected 2)", context)
        return False
    
    #//src/dest
    src, dest = 0 , 0

    try :
        src = convertBlendFactor(vecparams[0])
        dest = convertBlendFactor(vecparams[1])
        #context.textureUnit.setColourOpMultipassFallback(src,dest)
        
    except :
        logParseError("Bad colour_op_multipass_fallback attribute, " \
            , context)
    return False

#//-----------------------------------------------------------------------
def parseAlphaOpEx( params,  context):
    params = params.lower()
    vecparams = params.split( None)
    numParams = len(vecparams)
    if (numParams < 3 or numParams > 6):
        logParseError("Bad alpha_op_ex attribute, wrong number of parameters " \
            "(expected 3 to 6)", context)
        return False
    op = 0
    src1, src2 = 0 , 0
    manual = 0.0
    arg1 = 1.0
    arg2 = 1.0

    try :
        op = convertBlendOpEx(vecparams[0])
        src1 = convertBlendSource(vecparams[1])
        src2 = convertBlendSource(vecparams[2])
        if (op == LBX_BLEND_MANUAL):
            if (numParams != 4):
                logParseError("Bad alpha_op_ex attribute, wrong number of parameters " \
                    "(expected 4 for manual blend)", context)
                return False
            manual = atof(vecparams[3])
        if (src1 == LBS_MANUAL):
            parIndex = 3
            if (op == LBX_BLEND_MANUAL):
                parIndex += 1

            if (numParams < parIndex):
                logParseError( \
                    "Bad alpha_op_ex attribute, wrong number of parameters (expected " +  \
                    str(parIndex - 1) + ")", context)
                return False
            
            arg1 = atof(vecparams[parIndex])
        
        if (src2 == LBS_MANUAL):
            parIndex = 3
            if (op == LBX_BLEND_MANUAL):
                parIndex += 1
            if (src1 == LBS_MANUAL):
                parIndex += 1

            if (numParams < parIndex):
                logParseError( \
                    "Bad alpha_op_ex attribute, wrong number of parameters " \
                    "(expected " + str(parIndex - 1) + ")", context)
                return False
            
            arg2 = atof(vecparams[parIndex])
    except :
        logParseError("Bad alpha_op_ex attribute, " , context)
        return False
    
    context.textureUnit.setAlphaOperation(op, src1, src2, arg1, arg2, manual)
    return False

#//-----------------------------------------------------------------------
def parseEnvMap( params, context):
    params = params.lower()
    if (params=="off"):
        pass #context.textureUnit.setEnvironmentMap(False)
    elif (params=="spherical"):
        pass #context.textureUnit.setEnvironmentMap(True, ENV_CURVED)
    elif (params=="planar"):
        pass #context.textureUnit.setEnvironmentMap(True, ENV_PLANAR)
    elif (params=="cubic_reflection"):
        pass #context.textureUnit.setEnvironmentMap(True, ENV_REFLECTION)
    elif (params=="cubic_normal"):
        pass #context.textureUnit.setEnvironmentMap(True, ENV_NORMAL)
    else:
        logParseError("Bad env_map attribute, valid parameters are 'off', " \
            "'spherical', 'planar', 'cubic_reflection' and 'cubic_normal'.", context)

    return False

#//-----------------------------------------------------------------------
def parseScroll( params, context):
    vecparams = params.split( None)
    if len(vecparams) != 2 :
        logParseError("Bad scroll attribute, wrong number of parameters (expected 2)", context)
        return False
    context.textureUnit.setTextureScroll( \
        atof(vecparams[0]), \
        atof(vecparams[1]))


    return False
#//-----------------------------------------------------------------------
def parseScrollAnim( params, context):
    vecparams = params.split( None)
    if len(vecparams) != 2 :
        logParseError("Bad scroll_anim attribute, wrong number of " \
            "parameters (expected 2)", context)
        return False
    #context.textureUnit.setScrollAnimation( \
    #    atof(vecparams[0]), \
    #    atof(vecparams[1]))

    return False
#//-----------------------------------------------------------------------
def parseRotate(params, context):
    #context.textureUnit.setTextureRotate(
    #    StringConverter::parseAngle(params));

    return False

#//-----------------------------------------------------------------------
def parseRotateAnim( params, context):
   # context.textureUnit.setRotateAnimation( \
   #     atof(params))

    return False

#//-----------------------------------------------------------------------
def parseScale(params, context):
    
    vecparams = params.split( None)
    if len(vecparams) != 2 :
        logParseError("Bad scale attribute, wrong number of parameters (expected 2)", context)
        return False
    context.textureUnit.setTextureScale( \
        atof(vecparams[0]), \
        atof(vecparams[1]))

    return False
#//-----------------------------------------------------------------------
def parseWaveXform(params, context):
    params = params.lower()
    vecparams = params.split( None)
    
    if len(vecparams) != 6 :
        logParseError("Bad wave_xform attribute, wrong number of parameters " \
            "(expected 6)", context)
        return False
    ttype = 0
    waveType = 0
    #// Check transform type
    if (vecparams[0]=="scroll_x"):
        ttype = TT_TRANSLATE_U
    elif (vecparams[0]=="scroll_y"):
        ttype = TT_TRANSLATE_V
    elif (vecparams[0]=="rotate"):
        ttype = TT_ROTATE
    elif (vecparams[0]=="scale_x"):
        ttype = TT_SCALE_U
    elif (vecparams[0]=="scale_y"):
        ttype = TT_SCALE_V
    else:
        logParseError("Bad wave_xform attribute, parameter 1 must be 'scroll_x', " \
            "'scroll_y', 'rotate', 'scale_x' or 'scale_y'", context)
        return False
    #// Check wave type
    if (vecparams[1]=="sine"):
        waveType = WFT_SINE
    elif (vecparams[1]=="triangle"):
        waveType = WFT_TRIANGLE
    elif (vecparams[1]=="square"):
        waveType = WFT_SQUARE
    elif (vecparams[1]=="sawtooth"):
        waveType = WFT_SAWTOOTH
    elif (vecparams[1]=="inverse_sawtooth"):
        waveType = WFT_INVERSE_SAWTOOTH
    else:
        logParseError("Bad wave_xform attribute, parameter 2 must be 'sine', " \
            "'triangle', 'square', 'sawtooth' or 'inverse_sawtooth'", context)
        return False
    
    #context.textureUnit.setTransformAnimation( \
    #    ttype, \
    #    waveType, \
    #    atof(vecparams[2]), \
    #    atof(vecparams[3]),\
    #    atof(vecparams[4]), \
    #    atof(vecparams[5]) )

    return False

#//-----------------------------------------------------------------------
def parseDepthBias( params,  context):
    #context._pass.setDepthBias(atof(params))

    return False

#//-----------------------------------------------------------------------
def parseAnisotropy( params, context):
    context.textureUnit.setTextureAnisotropy( \
        atoi(params))

    return False

#//-----------------------------------------------------------------------
def parseLodDistances(params, context):
    vecparams = params.split( None)
    #// iterate over the parameters and parse distances out of them
    lodList = []
    
    for i in vecparams:
        lodList.pushback(atof(i))
    
    context.material.setLodLevels(lodList)

    return False

#//-----------------------------------------------------------------------
def parseLodIndex( params, context):
    context.technique.setLodIndex(atoi(params))
    return False
'''
#//-----------------------------------------------------------------------
def processManualProgramParam( index,  commandname, vecparams, context):
    #// NB we assume that the first element of vecparams is taken up with either 
    #// the index or the parameter name, which we ignore

    #// Determine type
    start, dims, roundedDims, i = 0, 0, 0, 0
    isReal = False

    vecparams[1] = vecparams[1].lower()

    if (vecparams[1] == "matrix4x4"):
        dims = 16
        isReal = True
    elif ((start = vecparams[1].find("float")) != String::npos)
        #// find the dimensionality
        start = vecparams[1].find_first_not_of("float");
        #// Assume 1 if not specified
        if (start == String::npos)
        {
            dims = 1;
        }
        else
        {
            dims = StringConverter::parseInt(vecparams[1].substr(start));
        }
        isReal = True;
    }
    elif ((start = vecparams[1].find("int")) != String::npos)
    {
        // find the dimensionality
        start = vecparams[1].find_first_not_of("int");
        // Assume 1 if not specified
        if (start == String::npos)
        {
            dims = 1;
        }
        else
        {
            dims = StringConverter::parseInt(vecparams[1].substr(start));
        }
        isReal = false;
    }
    else
    {
        logParseError("Invalid " + commandname + " attribute - unrecognised "
            "parameter type " + vecparams[1], context);
        return;
    }

    if (vecparams.size() != 2 + dims)
    {
        logParseError("Invalid " + commandname + " attribute - you need " +
            StringConverter::toString(2 + dims) + " parameters for a parameter of "
            "type " + vecparams[1], context);
    }

    // Round dims to multiple of 4
    if (dims %4 != 0)
    {
        roundedDims = dims + 4 - (dims % 4);
    }
    else
    {
        roundedDims = dims;
    }

    // Now parse all the values
    if (isReal)
    {
        Real* realBuffer = new Real[roundedDims];
        // Do specified values
        for (i = 0; i < dims; ++i)
        {
            realBuffer[i] = StringConverter::parseReal(vecparams[i+2]);
        }
        // Fill up to multiple of 4 with zero
        for (; i < roundedDims; ++i)
        {
            realBuffer[i] = 0.0f;
        }
        // Set
        context.programParams->setConstant(index, realBuffer, roundedDims * 0.25);
        delete [] realBuffer;
    }
    else
    {
        int* intBuffer = new int[roundedDims];
        // Do specified values
        for (i = 0; i < dims; ++i)
        {
            intBuffer[i] = StringConverter::parseInt(vecparams[i+2]);
        }
        // Fill to multiple of 4 with 0
        for (; i < roundedDims; ++i)
        {
            intBuffer[i] = 0;
        }
        // Set
        context.programParams->setConstant(index, intBuffer, roundedDims * 0.25);
        delete [] intBuffer;
    }
}
//-----------------------------------------------------------------------
void processAutoProgramParam(size_t index, const String& commandname, 
    StringVector& vecparams, MaterialScriptContext& context)
{
    // NB we assume that the first element of vecparams is taken up with either 
    // the index or the parameter name, which we ignore

    bool extras = false;
    bool float_extras = false;
    GpuProgramParameters::AutoConstantType acType;

    StringUtil::toLowerCase(vecparams[1]);

    if (vecparams[1] == "world_matrix")
    {
        acType = GpuProgramParameters::ACT_WORLD_MATRIX;
    }
    elif (vecparams[1] == "world_matrix_array")
    {
        acType = GpuProgramParameters::ACT_WORLD_MATRIX_ARRAY;
    }
    elif (vecparams[1] == "world_matrix_array_3x4")
    {
        acType = GpuProgramParameters::ACT_WORLD_MATRIX_ARRAY_3x4;
    }
    elif (vecparams[1] == "view_matrix")
    {
        acType = GpuProgramParameters::ACT_VIEW_MATRIX;
    }
    elif (vecparams[1] == "projection_matrix")
    {
        acType = GpuProgramParameters::ACT_PROJECTION_MATRIX;
    }
    elif (vecparams[1] == "viewproj_matrix")
    {
        acType = GpuProgramParameters::ACT_VIEWPROJ_MATRIX;
    }
    elif (vecparams[1] == "worldview_matrix")
    {
        acType = GpuProgramParameters::ACT_WORLDVIEW_MATRIX;
    }
    elif (vecparams[1] == "worldviewproj_matrix")
    {
        acType = GpuProgramParameters::ACT_WORLDVIEWPROJ_MATRIX;
    }
    elif (vecparams[1] == "inverse_world_matrix")
    {
        acType = GpuProgramParameters::ACT_INVERSE_WORLD_MATRIX;
    }
    elif (vecparams[1] == "inverse_view_matrix")
    {
        acType = GpuProgramParameters::ACT_INVERSE_VIEW_MATRIX;
    }
    elif (vecparams[1] == "inverse_worldview_matrix")
    {
        acType = GpuProgramParameters::ACT_INVERSE_WORLDVIEW_MATRIX;
    }
    elif (vecparams[1] == "inverse_transpose_world_matrix")
    {
        acType = GpuProgramParameters::ACT_INVERSETRANSPOSE_WORLD_MATRIX;
    }
    elif (vecparams[1] == "inverse_transpose_worldview_matrix")
    {
        acType = GpuProgramParameters::ACT_INVERSETRANSPOSE_WORLDVIEW_MATRIX;
    }
    elif (vecparams[1] == "time_0_x") 
    {
        acType = GpuProgramParameters::ACT_TIME_0_X;
        float_extras = True;
    }
    elif (vecparams[1] == "costime_0_x") 
    {
        acType = GpuProgramParameters::ACT_COSTIME_0_X;
        float_extras = True;
    }
    elif (vecparams[1] == "sintime_0_x") 
    {
        acType = GpuProgramParameters::ACT_SINTIME_0_X;
        float_extras = True;
    }
    elif (vecparams[1] == "tantime_0_x") 
    {
        acType = GpuProgramParameters::ACT_TANTIME_0_X;
        float_extras = True;
    }
    elif (vecparams[1] == "time_0_x_packed") 
    {
        acType = GpuProgramParameters::ACT_TIME_0_X_PACKED;
        float_extras = True;
    }
    elif (vecparams[1] == "time_0_1") 
    {
        acType = GpuProgramParameters::ACT_TIME_0_1;
        float_extras = True;
    }
    elif (vecparams[1] == "costime_0_1") 
    {
        acType = GpuProgramParameters::ACT_COSTIME_0_1;
        float_extras = True;
    }
    elif (vecparams[1] == "sintime_0_1") 
    {
        acType = GpuProgramParameters::ACT_SINTIME_0_1;
        float_extras = True;
    }
    elif (vecparams[1] == "tantime_0_1") 
    {
        acType = GpuProgramParameters::ACT_TANTIME_0_1;
        float_extras = True;
    }
    elif (vecparams[1] == "time_0_1_packed") 
    {
        acType = GpuProgramParameters::ACT_TIME_0_1_PACKED;
        float_extras = True;
    }
    elif (vecparams[1] == "time_0_2pi") 
    {
        acType = GpuProgramParameters::ACT_TIME_0_2PI;
        float_extras = True;
    }
    elif (vecparams[1] == "costime_0_2pi") 
    {
        acType = GpuProgramParameters::ACT_COSTIME_0_2PI;
        float_extras = True;
    }
    elif (vecparams[1] == "sintime_0_2pi") 
    {
        acType = GpuProgramParameters::ACT_SINTIME_0_2PI;
        float_extras = True;
    }
    elif (vecparams[1] == "tantime_0_2pi") 
    {
        acType = GpuProgramParameters::ACT_TANTIME_0_2PI;
        float_extras = True;
    }
    elif (vecparams[1] == "time_0_2pi_packed") 
    {
        acType = GpuProgramParameters::ACT_TIME_0_2PI_PACKED;
        float_extras = True;
    }
    elif (vecparams[1] == "fps") 
    {
        acType = GpuProgramParameters::ACT_FPS;
    }
    elif (vecparams[1] == "viewport_width") 
    {
        acType = GpuProgramParameters::ACT_VIEWPORT_WIDTH;
    }
    elif (vecparams[1] == "viewport_height") 
    {
        acType = GpuProgramParameters::ACT_VIEWPORT_HEIGHT;
    }
    elif (vecparams[1] == "inverse_viewport_width") 
    {
        acType = GpuProgramParameters::ACT_INVERSE_VIEWPORT_WIDTH;
    }
    elif (vecparams[1] == "inverse_viewport_height") 
    {
        acType = GpuProgramParameters::ACT_INVERSE_VIEWPORT_HEIGHT;
    }
    elif (vecparams[1] == "view_direction") 
    {
        acType = GpuProgramParameters::ACT_VIEW_DIRECTION;
    }
    elif (vecparams[1] == "view_side_vector") 
    {
        acType = GpuProgramParameters::ACT_VIEW_SIDE_VECTOR;
    }
    elif (vecparams[1] == "view_up_vector") 
    {
        acType = GpuProgramParameters::ACT_VIEW_UP_VECTOR;
    }
    elif (vecparams[1] == "fov") 
    {
        acType = GpuProgramParameters::ACT_FOV;
    }
    elif (vecparams[1] == "near_clip_distance") 
    {
        acType = GpuProgramParameters::ACT_NEAR_CLIP_DISTANCE;
    }
    elif (vecparams[1] == "far_clip_distance") 
    {
        acType = GpuProgramParameters::ACT_FAR_CLIP_DISTANCE;
    }
    elif (vecparams[1] == "inverse_viewproj_matrix") 
    {
        acType = GpuProgramParameters::ACT_INVERSE_VIEWPROJ_MATRIX;
    }
    elif (vecparams[1] == "inverse_transpose_viewproj_matrix") 
    {
        acType = GpuProgramParameters::ACT_INVERSETRANSPOSE_VIEWPROJ_MATRIX;
    }
    elif (vecparams[1] == "transpose_viewproj_matrix") 
    {
        acType = GpuProgramParameters::ACT_TRANSPOSE_VIEWPROJ_MATRIX;
    }
    elif (vecparams[1] == "transpose_view_matrix") 
    {
        acType = GpuProgramParameters::ACT_TRANSPOSE_VIEW_MATRIX;
    }
    elif (vecparams[1] == "inverse_transpose_view_matrix") 
    {
        acType = GpuProgramParameters::ACT_INVERSETRANSPOSE_VIEW_MATRIX;
    }
    elif (vecparams[1] == "projection_matrix") 
    {
        acType = GpuProgramParameters::ACT_PROJECTION_MATRIX;
    }
    elif (vecparams[1] == "transpose_projection_matrix") 
    {
        acType = GpuProgramParameters::ACT_TRANSPOSE_PROJECTION_MATRIX;
    }
    elif (vecparams[1] == "inverse_projection_matrix") 
    {
        acType = GpuProgramParameters::ACT_INVERSE_PROJECTION_MATRIX;
    }
    elif (vecparams[1] == "inverse_transpose_projection_matrix") 
    {
        acType = GpuProgramParameters::ACT_INVERSETRANSPOSE_PROJECTION_MATRIX;
    }
    elif (vecparams[1] == "transpose_worldviewproj_matrix") 
    {
        acType = GpuProgramParameters::ACT_TRANSPOSE_WORLDVIEWPROJ_MATRIX;
    }
    elif (vecparams[1] == "inverse_worldviewproj_matrix") 
    {
        acType = GpuProgramParameters::ACT_INVERSE_WORLDVIEWPROJ_MATRIX;
    }
    elif (vecparams[1] == "inverse_transpose_worldviewproj_matrix") 
    {
        acType = GpuProgramParameters::ACT_INVERSETRANSPOSE_WORLDVIEWPROJ_MATRIX;
    }
    elif (vecparams[1] == "transpose_worldview_matrix") 
    {
        acType = GpuProgramParameters::ACT_TRANSPOSE_WORLDVIEW_MATRIX;
    }
    elif (vecparams[1] == "inverse_transpose_worldview_matrix") 
    {
        acType = GpuProgramParameters::ACT_INVERSE_TRANSPOSE_WORLDVIEW_MATRIX;
    }
    elif (vecparams[1] == "transpose_world_matrix") 
    {
        acType = GpuProgramParameters::ACT_TRANSPOSE_WORLD_MATRIX;
    }
    elif (vecparams[1] == "inverse_transpose_world_matrix") 
    {
        acType = GpuProgramParameters::ACT_INVERSE_TRANSPOSE_WORLD_MATRIX;
    }
    elif (vecparams[1] == "light_diffuse_colour")
    {
        acType = GpuProgramParameters::ACT_LIGHT_DIFFUSE_COLOUR;
        extras = True;
    }
    elif (vecparams[1] == "light_specular_colour")
    {
        acType = GpuProgramParameters::ACT_LIGHT_SPECULAR_COLOUR;
        extras = True;
    }
    elif (vecparams[1] == "light_attenuation")
    {
        acType = GpuProgramParameters::ACT_LIGHT_ATTENUATION;
        extras = True;
    }
    elif (vecparams[1] == "light_position")
    {
        acType = GpuProgramParameters::ACT_LIGHT_POSITION;
        extras = True;
    }
    elif (vecparams[1] == "light_direction")
    {
        acType = GpuProgramParameters::ACT_LIGHT_DIRECTION;
        extras = True;
    }
    elif (vecparams[1] == "light_position_object_space")
    {
        acType = GpuProgramParameters::ACT_LIGHT_POSITION_OBJECT_SPACE;
        extras = True;
    }
    elif (vecparams[1] == "light_direction_object_space")
    {
        acType = GpuProgramParameters::ACT_LIGHT_DIRECTION_OBJECT_SPACE;
        extras = True;
    }
    elif (vecparams[1] == "ambient_light_colour")
    {
        acType = GpuProgramParameters::ACT_AMBIENT_LIGHT_COLOUR;
    }
    elif (vecparams[1] == "camera_position")
    {
        acType = GpuProgramParameters::ACT_CAMERA_POSITION;
    }
    elif (vecparams[1] == "camera_position_object_space")
    {
        acType = GpuProgramParameters::ACT_CAMERA_POSITION_OBJECT_SPACE;
    }
    elif (vecparams[1] == "texture_viewproj_matrix")
    {
        acType = GpuProgramParameters::ACT_TEXTURE_VIEWPROJ_MATRIX;
    }
    elif (vecparams[1] == "time")
    {
        // Special case!
        Real factor = 1.0f;
        if (vecparams.size() == 3)
        {
            factor = StringConverter::parseReal(vecparams[2]);
        }
        
        context.programParams->setConstantFromTime(index, factor);
        return;
    }
    elif (vecparams[1] == "custom")
    {
        acType = GpuProgramParameters::ACT_CUSTOM;
        extras = True;
    }
    else
    {
        logParseError("Invalid " + commandname + " attribute - "
            + vecparams[1], context);
        return;

    }

    // Do we need any extra parameters?
    size_t extraParam = 0;
    if (extras)
    {
        if (vecparams.size() != 3)
        {
            logParseError("Invalid " + commandname + " attribute - "
                "expected 3 parameters.", context);
            return;
        }
        extraParam = StringConverter::parseInt(vecparams[2]);
    }
    
    
    if (float_extras) {
        Real rData = StringConverter::parseReal(vecparams[2]);
        context.programParams->setAutoConstantReal(index, acType, rData);
    }
    else {
        context.programParams->setAutoConstant(index, acType, extraParam);
    }

}'''

#//-----------------------------------------------------------------------
def parseParamIndexed(params, context):
    #// NB skip this if the program is not supported or could not be found
    if context.program == [] or context.program.isSupported() == False:
        return False

    params = params.lower()
    vecparams = params.split( None)
    if len(vecparams) < 3:
        logParseError("Invalid param_indexed attribute - expected at least 3 parameters.", \
            context)
        return False
    
    #// Get start index
    index = atoi(vecparams[0])
    
    processManualProgramParam(index, "param_indexed", vecparams, context)
    
    return False


#//-----------------------------------------------------------------------
def parseParamIndexedAuto( params,  context):
    #// NB skip this if the program is not supported or could not be found
    if context.program == [] or context.program.isSupported() == False :
        return False
    
    params = params.lower()
    vecparams = params.split (None)
    if len(vecparams) != 2 and len(vecparams) != 3 :
        logParseError("Invalid param_indexed_auto attribute - expected 2 or 3 parameters.", \
            context)
        return False
    
    #// Get start index
    index = atoi(vecparams[0])
    
    processAutoProgramParam(index, "param_indexed_auto", vecparams, context)
    
    return False
    
#//-----------------------------------------------------------------------
def parseParamNamed( params,  context):
    #// NB skip this if the program is not supported or could not be found
    if context.program == [] or context.program.isSupported() == False :
        return False
    
    
    vecparams = params.split (None)
    if len(vecparams) < 3 :
        logParseError("Invalid param_named attribute - expected at least 3 parameters.", \
            context)
        return False
    
    #// Get start index from name
    index = 0
    try :
        index = context.programParams.getParamIndex(vecparams[0])
    except :
        logParseError("Invalid param_named attribute - " , context)
        return False
    
    #// TEST
    '''/*
    LogManager::getSingleton().logMessage("SETTING PARAMETER " + vecparams[0] + " as index " +
        StringConverter::toString(index));
    */'''
    processManualProgramParam(index, "param_named", vecparams, context)
    
    return False
#//-----------------------------------------------------------------------
def parseParamNamedAuto( params,  context):

    #// NB skip this if the program is not supported or could not be found
    if context.program == [] or context.program.isSupported() == False:
        return False
    
    vecparams = params.split ( None)
    if len(vecparams) != 2 and len(vecparams) != 3 :
        logParseError("Invalid param_indexed_auto attribute - expected 2 or 3 parameters.", \
            context)
        return False
    
    #// Get start index from name
    index = 0
    try :
        index = context.programParams.getParamIndex(vecparams[0])
    except :
        logParseError("Invalid param_named_auto attribute - ", context)
        return False
    
    processAutoProgramParam(index, "param_named_auto", vecparams, context)
    
    return False
#//-----------------------------------------------------------------------
def parseMaterial( params,  context):
    #// Create a brand new material
    #context.material =  \
    #   MaterialManager::getSingleton().create(params, context.groupName)
    #context.material._notifyOrigin(context.filename)
    #// Remove pre-created technique from defaults
    #context.material.removeAllTechniques();
    
    #// update section
    context.section = MSS_MATERIAL
    
    #// Return TRUE because this must be followed by a {
    return True

#//-----------------------------------------------------------------------
def parseTechnique( params,  context):
    #// Create a new technique
    #context.technique = context.material.createTechnique()
    
    #// update section
    context.section = MSS_TECHNIQUE
    
    #//Increase technique level depth
    context.techLev += 1
    
    #// Return TRUE because this must be followed by a {
    return True

#//-----------------------------------------------------------------------
def parsePass( params,  context):
    #// Create a new pass
    ##context._pass = context.technique.createPass()
    
    #// update section
    context.section = MSS_PASS
    
    #//Increase pass level depth
    context.passLev += 1
    
    #// Return TRUE because this must be followed by a {
    return True

#//-----------------------------------------------------------------------
def parseTextureUnit( params, context):
    #// Create a new texture unit
    #context.textureUnit = #context._pass.createTextureUnitState()

    #// update section
    context.section = MSS_TEXTUREUNIT

    #// Increase texture unit depth
    context.stateLev += 1

    #// Return TRUE because this must be followed by a {
    return True

#//-----------------------------------------------------------------------
def parseVertexProgramRef( params,  context):
    #// update section
    context.section = MSS_PROGRAM_REF
    '''
    context.program = GpuProgramManager::getSingleton().getByName(params);
    if (context.program.isNull())
    {
        // Unknown program
        logParseError("Invalid vertex_program_ref entry - vertex program " 
            + params + " has not been defined.", context);
        return True;
    }

    context.isProgramShadowCaster = false;
    context.isProgramShadowReceiver = false;
    
    // Set the vertex program for this pass
    context.pass->setVertexProgram(params);

    // Create params? Skip this if program is not supported
    if (context.program->isSupported())
    {
        context.programParams = context.pass->getVertexProgramParameters();
    }

    // Return TRUE because this must be followed by a {'''
    return True
#//-----------------------------------------------------------------------
def parseShadowCasterVertexProgramRef(params,  context):
    #// update section
    context.section = MSS_PROGRAM_REF
    '''
    context.program = GpuProgramManager::getSingleton().getByName(params);
    if (context.program.isNull())
    {
        // Unknown program
        logParseError("Invalid vertex_program_ref entry - vertex program " 
            + params + " has not been defined.", context);
        return True;
    }

    context.isProgramShadowCaster = True;
    context.isProgramShadowReceiver = false;

    // Set the vertex program for this pass
    context.pass->setShadowCasterVertexProgram(params);

    // Create params? Skip this if program is not supported
    if (context.program->isSupported())
    {
        context.programParams = context.pass->getShadowCasterVertexProgramParameters();
    }

    // Return TRUE because this must be followed by a {'''
    return True

#//-----------------------------------------------------------------------
def parseShadowReceiverVertexProgramRef( params,  context):
   #// update section
    context.section = MSS_PROGRAM_REF
    '''
    context.program = GpuProgramManager::getSingleton().getByName(params);
    if (context.program.isNull())
    {
        // Unknown program
        logParseError("Invalid vertex_program_ref entry - vertex program " 
            + params + " has not been defined.", context);
        return True;
    }

    context.isProgramShadowCaster = false;
    context.isProgramShadowReceiver = True;

    // Set the vertex program for this pass
    context.pass->setShadowReceiverVertexProgram(params);

    // Create params? Skip this if program is not supported
    if (context.program->isSupported())
    {
        context.programParams = context.pass->getShadowReceiverVertexProgramParameters();
    }

    // Return TRUE because this must be followed by a { '''
    return True

#//-----------------------------------------------------------------------
def parseFragmentProgramRef( params, context):
    #// update section
    context.section = MSS_PROGRAM_REF
    '''
    context.program = GpuProgramManager::getSingleton().getByName(params);
    if (context.program.isNull())
    {
        // Unknown program
        logParseError("Invalid fragment_program_ref entry - fragment program " 
            + params + " has not been defined.", context);
        return True;
    }
    
    // Set the vertex program for this pass
    context.pass->setFragmentProgram(params);

    // Create params? Skip this if program is not supported
    if (context.program->isSupported())
    {
        context.programParams = context.pass->getFragmentProgramParameters();
    }

    // Return TRUE because this must be followed by a {'''
    return True

#//-----------------------------------------------------------------------
def parseVertexProgram(params,  context):
    #// update section
    context.section = MSS_PROGRAM
    
    #// Create new program definition-in-progress
    context.programDef = MaterialScriptProgramDefinition()
    context.programDef.progType = GPT_VERTEX_PROGRAM
    context.programDef.supportsSkeletalAnimation = False

    #// Get name and language code
    vecparams = params.split( None)
    if len(vecparams) != 2:
        logParseError("Invalid vertex_program entry - expected " \
            "2 parameters.", context)
        return True
    #// Name, preserve case
    context.programDef.name = vecparams[0]
    #// language code, make lower case
    context.programDef.language = vecparams[1]
    context.programDef.language = context.programDef.language.lower()

    #// Return TRUE because this must be followed by a { 
    return True

#//-----------------------------------------------------------------------
def parseFragmentProgram( params, context):
    #// update section
    context.section = MSS_PROGRAM
    
    #// Create new program definition-in-progress
    context.programDef = MaterialScriptProgramDefinition()
    context.programDef.progType = GPT_FRAGMENT_PROGRAM
    context.programDef.supportsSkeletalAnimation = False

    #// Get name and language code
    vecparams = params.split( None)
    if len(vecparams) != 2:
        logParseError("Invalid fragment_program entry - expected "
            "2 parameters.", context)
        return True
    #// Name, preserve case
    context.programDef.name = vecparams[0]
    #// language code, make lower case
    context.programDef.language = vecparams[1].lower()
    
   
    #// Return TRUE because this must be followed by a {
    return True

#//-----------------------------------------------------------------------
def parseProgramSource(params, context):
    #// Source filename, preserve case
    context.programDef.source = params

    return False
#//-----------------------------------------------------------------------
def parseProgramSkeletalAnimation( params, context):
    #// Source filename, preserve case
    context.programDef.supportsSkeletalAnimation =  abs(int(params)) > 0 

    return False
#//-----------------------------------------------------------------------
def parseProgramSyntax( params, context):
    #// Syntax code, make lower case
    params = params.lower()
    context.programDef.syntax = params

    return False
#//-----------------------------------------------------------------------
def parseProgramCustomParameter( params, context):
    #// This params object does not have the command stripped
    #// Lower case the command, but not the value incase its relevant
    #// Split only up to first delimiter, program deals with the rest
    vecparams = params.split(None, 1)
    if len(vecparams) != 2 :
        logParseError("Invalid custom program parameter entry; " \
            "there must be a parameter name and at least one value.", \
            context)
        return False
    
    context.programDef.customParameters[vecparams[0]] = vecparams[1]

    return False

#//-----------------------------------------------------------------------
def parseTextureSource(params, context):
    params = params.lower()
    vecparams = params.split( None)
    #//The only param should identify which ExternalTextureSource is needed
    
    if len(vecparams.size) != 1 :
        logParseError("Invalid texture source attribute - expected 1 parameter." , context);
    #ExternalTextureSourceManager::getSingleton().setCurrentPlugIn( vecparams[0] )
    '''
    if( ExternalTextureSourceManager::getSingleton().getCurrentPlugIn() != 0 )
    {
        String tps;
        tps = StringConverter::toString( context.techLev ) + " "
            + StringConverter::toString( context.passLev ) + " "
            + StringConverter::toString( context.stateLev);

        ExternalTextureSourceManager::getSingleton().getCurrentPlugIn()->setParameter( "set_T_P_S", tps );
    }
    ''' 
    #// update section
    context.section = MSS_TEXTURESOURCE
    #// Return TRUE because this must be followed by a {
    return True

#//-----------------------------------------------------------------------
def parseTextureCustomParameter(params, context):
    #// This params object does not have the command stripped
    #// Split only up to first delimiter, program deals with the rest
    
    vecparams = params.split( None)
    if  len(vecparams) != 2:
        logParseError("Invalid texture parameter entry; " \
            "there must be a parameter name and at least one value.", \
            context)
        return False
    '''
    if( ExternalTextureSourceManager::getSingleton().getCurrentPlugIn() != 0 )
        ////First is command, next could be a string with one or more values
        ExternalTextureSourceManager::getSingleton().getCurrentPlugIn()->setParameter( vecparams[0], vecparams[1] );
    '''
    return False

#//-----------------------------------------------------------------------
def parseReceiveShadows(params,  context):
    params = params.lower()
    if (params == "on"):
        pass #context.material.setReceiveShadows(True)
    elif (params == "off"):
        pass #context.material.setReceiveShadows(False)
    else:
        logParseError( \
        "Bad receive_shadows attribute, valid parameters are 'on' or 'off'.", \
        context)

    return False

#//-----------------------------------------------------------------------
def parseDefaultParams(params,  context):
    context.section = MSS_DEFAULT_PARAMETERS
    #// Should be a brace next
    return True
#//-----------------------------------------------------------------------
def parseTransparencyCastsShadows(params, context):
    params = params.lower()
    if (params == "on"):
        context.material.setTransparencyCastsShadows(True)
    elif (params == "off"):
        context.material.setTransparencyCastsShadows(False)
    else:
        logParseError( \
        "Bad transparency_casts_shadows attribute, valid parameters are 'on' or 'off'.", \
        context)

    return False

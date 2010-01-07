
#    /** Enum to identify material sections. */
MSS_NONE, MSS_MATERIAL, MSS_TECHNIQUE, MSS_PASS, \
 MSS_TEXTUREUNIT,MSS_PROGRAM_REF, MSS_PROGRAM, \
 MSS_DEFAULT_PARAMETERS, MSS_TEXTURESOURCE = range(9)

##/// 1D texture, used in combination with 1D texture coordinates
##/// 2D texture, used in combination with 2D texture coordinates (default)
##/// 3D volume texture, used in combination with 3D texture coordinates
##/// 3D cube map, used in combination with 3D texture coordinates

TEX_TYPE_1D ,TEX_TYPE_2D , TEX_TYPE_3D , TEX_TYPE_CUBE_MAP  = range(1,5)

#enum SceneBlendType
##/// Make the object transparent based on the final alpha values in the texture
##/// Make the object transparent based on the colour             ET_ROTATE,
#values in the texture (brighter = more opaque)
##/// Add the texture values to the existing scene content
##/// Multiply the 2 colours together
#// TODO : more
SBT_TRANSPARENT_ALPHA,SBT_TRANSPARENT_COLOUR, SBT_ADD, SBT_MODULATE = range(4)




#/** Enumerates the types of programs which can run on the GPU. */
GPT_VERTEX_PROGRAM = 0
GPT_FRAGMENT_PROGRAM = 1
#   /** Struct for holding a program definition which is in progress. */

# enum TextureEffectType
##/// Generate all texture coords based on angle between camera and vertex
##/// Generate texture coords based on a frustum
##/// Constant u/v scrolling effect
##/// Constant rotation
##/// More complex transform
ET_ENVIRONMENT_MAP, ET_PROJECTIVE_TEXTURE, ET_SCROLL, ET_ROTATE, ET_TRANSFORM = range(5)

#/** Enumeration to specify type of envmap.
#@note
#Note that these have no effect when using the programmable pipeline, since their
#effect is overridden by the vertex / fragment programs.

#enum EnvMapType
##/// Envmap based on vector from camera to vertex position, good for planar geometry
##/// Envmap based on dot of vector from camera to vertex and vertex normal, good for curves
##/// Envmap entended to supply reflection vectors for cube mapping
##/// Envmap entended to supply normal vectors for cube mapping
ENV_PLANAR, ENV_CURVED, ENV_REFLECTION, ENV_NORMAL = range(4)

#/** Useful enumeration when dealing with procedural transforms.
#@note
#Note that these have no effect when using the programmable pipeline, since their
#effect is overridden by the vertex / fragment programs.

#enum TextureTransformType
TT_TRANSLATE_U, TT_TRANSLATE_V, TT_SCALE_U, TT_SCALE_V, TT_ROTATE = range(5)

#/** Texture addressing modes - default is TAM_WRAP.
#@note
#These settings are relevant in both the fixed-function and the
#programmable pipeline.
#enum TextureAddressingMode
##/// Texture wraps at values over 1.0
##/// Texture mirrors (flips) at joins over 1.0
##/// Texture clamps at 1.0
TAM_WRAP, TAM_MIRROR, TAM_CLAMP = range(3)
 
#/** Enum identifying the frame indexes for faces of a cube map (not the composite 3D type.
#enum TextureCubeFace
CUBE_FRONT , CUBE_BACK , CUBE_LEFT , CUBE_RIGHT , CUBE_UP ,CUBE_DOWN  = range(6)

#    /** Comparison functions used for the depth/stencil buffer operations and 
#       others. */
#    enum CompareFunction
CMPF_ALWAYS_FAIL, CMPF_ALWAYS_PASS, CMPF_LESS, CMPF_LESS_EQUAL, CMPF_EQUAL, CMPF_NOT_EQUAL, CMPF_GREATER_EQUAL, CMPF_GREATER = range(8)
 
#    /** High-level filtering options providing shortcuts to settings the
#        minification, magnification and mip filters. */
#    enum TextureFilterOptions
#        #/// Equal to: min=FO_POINT, mag=FO_POINT, mip=FO_NONE
#       #/// Equal to: min=FO_LINEAR, mag=FO_LINEAR, mip=FO_POINT
#        #/// Equal to: min=FO_LINEAR, mag=FO_LINEAR, mip=FO_LINEAR
#        #/// Equal to: min=FO_ANISOTROPIC, max=FO_ANISOTROPIC, mip=FO_LINEAR
TFO_NONE, TFO_BILINEAR, TFO_TRILINEAR, TFO_ANISOTROPIC = range(4)
 
#    enum FilterType
#        #/// The filter used when shrinking a texture
#        #/// The filter used when magnifiying a texture
#        #/// The filter used when determining the mipmap
FT_MIN, FT_MAG, FT_MIP = range(3)

#    /** Filtering options for textures / mipmaps. */
#    enum FilterOptions
#        #/// No filtering, used for FILT_MIP to turn off mipmapping
#        #/// Use the closest pixel
#        #/// Average of a 2x2 pixel area, denotes bilinear for MIN and MAG, trilinear for MIP
#        #/// Similar to FO_LINEAR, but compensates for the angle of the texture plane
FO_NONE, FO_POINT, FO_LINEAR, FO_ANISOTROPIC = range(4)

#    /** Light shading modes. */
#    enum ShadeOptions
SO_FLAT, SO_GOURAUD, SO_PHONG = range(3)

#/** Fog modes. */
#    enum FogMode
#        #/// No fog. Duh.
#        #/// Fog density increases  exponentially from the camera (fog = 1/e^(distance * density))
#        #/// Fog density increases at the square of FOG_EXP, i.e. even quicker (fog = 1/e^(distance * density)^2)
#        #/// Fog density increases linearly between the start and end distances
FOG_NONE, FOG_EXP, FOG_EXP2, FOG_LINEAR = range(4)

#    /** Hardware culling modes based on vertex winding.
#        This setting applies to how the hardware API culls triangles it is sent. */
#    enum CullingMode
#        #/// Hardware never culls triangles and renders everything it receives.
#        #/// Hardware culls triangles whose vertices are listed clockwise in the view (default).
#        #/// Hardware culls triangles whose vertices are listed anticlockwise in the view.
CULL_NONE , CULL_CLOCKWISE , CULL_ANTICLOCKWISE = range(1,4)

#    /** Manual culling modes based on vertex normals.
#        This setting applies to how the software culls triangles before sending them to the 
#       hardware API. This culling mode is used by scene managers which choose to implement it -
#       normally those which deal with large amounts of fixed world geometry which is often 
#       planar (software culling movable variable geometry is expensive). */
#    enum ManualCullingMode
#        #/// No culling so everything is sent to the hardware.
#        #/// Cull triangles whose normal is pointing away from the camera (default).
#        #/// Cull triangles whose normal is pointing towards the camera.
MANUAL_CULL_NONE , MANUAL_CULL_BACK , MANUAL_CULL_FRONT = range(1,4)

 #   /** Enumerates the wave types usable with the Ogre engine. */
 #   enum WaveformType
 #       #/// Standard sine wave which smoothly changes from low to high and back again.
 #       #/// An angular wave with a constant increase / decrease speed with pointed peaks.
 #       #/// Half of the time is spent at the min, half at the max with instant transition between.
 #       #/// Gradual steady increase from min to max over the period with an instant return to min at the end.
 #       #/// Gradual steady decrease from max to min over the period, with an instant return to max at the end.
 
WFT_SINE, WFT_TRIANGLE, WFT_SQUARE, WFT_SAWTOOTH, WFT_INVERSE_SAWTOOTH = range(5)

#   /** The broad type of detail for rendering. */
#   enum SceneDetailLevel
#       #/// Only points are rendered.
#       #/// Wireframe models are rendered.
#       #/// Solid polygons are rendered.
SDL_POINTS ,SDL_WIREFRAME ,SDL_SOLID = range(1,4)

#    /** An enumeration of broad shadow techniques */
#    enum ShadowTechnique
#        /** No shadows */

#        /** Stencil shadow technique which renders all shadow volumes as
#            a modulation after all the non-transparent areas have been 
#            rendered. This technique is considerably less fillrate intensive 
#            than the additive stencil shadow approach when there are multiple
#            lights, but is not an accurate model. 

#        /** Stencil shadow technique which renders each light as a separate
#            additive pass to the scene. This technique can be very fillrate
#            intensive because it requires at least 2 passes of the entire
#            scene, more if there are multiple lights. However, it is a more
#            accurate model than the modulative stencil approach and this is
#            especially apparant when using coloured lights or bump mapping.

#        /** Texture-based shadow technique which involves a monochrome render-to-texture
#            of the shadow caster and a projection of that texture onto the 
#            shadow receivers as a modulative pass. 

SHADOWTYPE_NONE, SHADOWTYPE_STENCIL_MODULATIVE, SHADOWTYPE_STENCIL_ADDITIVE, \
        SHADOWTYPE_TEXTURE_MODULATIVE = range(4)

#    /** An enumeration describing which material properties should track the vertex colours */
#    enum TrackVertexColourEnum {

TVC_NONE        = 0x0
TVC_AMBIENT     = 0x1        
TVC_DIFFUSE     = 0x2
TVC_SPECULAR    = 0x4
TVC_EMISSIVE    = 0x8

#   /** Types of blending that you can specify between an object and the existing contents of the scene.
#        @remarks
#            As opposed to the LayerBlendType, which classifies blends between texture layers, these blending
#            types blend between the output of the texture units and the pixels already in the viewport,
#            allowing for object transparency, glows, etc.
#        @par
#            These types are provided to give quick and easy access to common effects. You can also use
#            the more manual method of supplying source and destination blending factors.
#            See Material::setSceneBlending for more details.
#        @see
#            Material::setSceneBlending
#    enum SceneBlendType
#        #/// Make the object transparent based on the final alpha values in the texture
#        #/// Make the object transparent based on the colour values in the texture (brighter = more opaque)
#        #/// Add the texture values to the existing scene content
#       #/// Multiply the 2 colours together
#        // TODO : more

SBT_TRANSPARENT_ALPHA, SBT_TRANSPARENT_COLOUR, SBT_ADD, SBT_MODULATE = range(4)

#    /** Blending factors for manually blending objects with the scene. If there isn't a predefined
#        SceneBlendType that you like, then you can specify the blending factors directly to affect the
#        combination of object and the existing scene. See Material::setSceneBlending for more details.

#    enum SceneBlendFactor
 
SBF_ONE, SBF_ZERO, SBF_DEST_COLOUR, SBF_SOURCE_COLOUR, SBF_ONE_MINUS_DEST_COLOUR, \
  SBF_ONE_MINUS_SOURCE_COLOUR, SBF_DEST_ALPHA, SBF_SOURCE_ALPHA, SBF_ONE_MINUS_DEST_ALPHA, \
  SBF_ONE_MINUS_SOURCE_ALPHA = range(10)

#    /** Type of texture blend mode.

#    enum LayerBlendType
LBT_COLOUR, \
LBT_ALPHA = range(2)

'''    /** List of valid texture blending operations, for use with TextureUnitState::setColourOperation.
        @remarks
            This list is a more limited list than LayerBlendOperationEx because it only
            includes operations that are supportable in both multipass and multitexture
            rendering and thus provides automatic fallback if multitexture hardware
            is lacking or insufficient.
'''
#    enum LayerBlendOperation {
#        #/// Replace all colour with texture with no adjustment
#        #/// Add colour components together. \
#        #/// Multiply colour components together. \
#        #/// Blend based on texture alpha \
LBO_REPLACE, \
LBO_ADD, \
LBO_MODULATE, \
LBO_ALPHA_BLEND = range(4)


'''   /** Expert list of valid texture blending operations, for use with TextureUnitState::setColourOperationEx
and TextureUnitState::setAlphaOperation, and internally in the LayerBlendModeEx class. Its worth
noting that these operations are for blending <i>between texture layers</i> and not between rendered objects
and the existing scene. Because all of these modes are only supported in multitexture hardware it may be
required to set up a fallback operation where this hardware is not available.
'''
#    enum LayerBlendOperationEx {
#/// use source1 without modification
#/// use source2 without modification
#/// multiply source1 and source2 together
#/// as LBX_MODULATE but brighten afterwards (x2)
#/// as LBX_MODULATE but brighten more afterwards (x4)
#/// add source1 and source2 together
#/// as LBX_ADD, but subtract 0.5 from the result
#/// as LBX_ADD, but subtract product from the sum
#/// subtract source2 from source1
#/// use interpolated alpha value from vertices to scale source1, then add source2 scaled by (1-alpha)
#/// as LBX_BLEND_DIFFUSE_ALPHA, but use alpha from texture
#/// as LBX_BLEND_DIFFUSE_ALPHA, but use current alpha from previous stages
#/// as LBX_BLEND_DIFFUSE_ALPHA but use a constant manual blend value (0.0-1.0)
#/// dotproduct of color1 and color2 
LBX_SOURCE1, \
LBX_SOURCE2,\
LBX_MODULATE, \
LBX_MODULATE_X2, \
LBX_MODULATE_X4, \
LBX_ADD, \
LBX_ADD_SIGNED, \
LBX_ADD_SMOOTH, \
LBX_SUBTRACT, \
LBX_BLEND_DIFFUSE_ALPHA, \
LBX_BLEND_TEXTURE_ALPHA, \
LBX_BLEND_CURRENT_ALPHA, \
LBX_BLEND_MANUAL, \
LBX_DOTPRODUCT = range(14)


'''    /** List of valid sources of values for blending operations used
in TextureUnitState::setColourOperation and TextureUnitState::setAlphaOperation,
and internally in the LayerBlendModeEx class.
'''
#    enum LayerBlendSource
#/// the colour as built up from previous stages
#/// the colour derived from the texture assigned to this layer
#/// the interpolated diffuse colour from the vertices
#/// the interpolated specular colour from the vertices
#/// a colour supplied manually as a separate argument
LBS_CURRENT, \
LBS_TEXTURE, \
LBS_DIFFUSE, \
LBS_SPECULAR, \
LBS_MANUAL = range(5)

'''    /** Types of blending that you can specify between an object and the existing contents of the scene.
@remarks
    As opposed to the LayerBlendType, which classifies blends between texture layers, these blending
    types blend between the output of the texture units and the pixels already in the viewport,
    allowing for object transparency, glows, etc.
@par
    These types are provided to give quick and easy access to common effects. You can also use
    the more manual method of supplying source and destination blending factors.
    See Material::setSceneBlending for more details.
@see
    Material::setSceneBlending
'''
#    enum SceneBlendType

#/// Make the object transparent based on the final alpha values in the texture
#/// Make the object transparent based on the colour values in the texture (brighter = more opaque)
#/// Add the texture values to the existing scene content
#/// Multiply the 2 colours together
#// TODO : more
SBT_TRANSPARENT_ALPHA, \
SBT_TRANSPARENT_COLOUR, \
SBT_ADD, \
SBT_MODULATE = range(4)

'''    /** Blending factors for manually blending objects with the scene. If there isnt a predefined
SceneBlendType that you like, then you can specify the blending factors directly to affect the
combination of object and the existing scene. See Material::setSceneBlending for more details.
'''    
#    enum SceneBlendFactor

SBF_ONE, \
SBF_ZERO, \
SBF_DEST_COLOUR, \
SBF_SOURCE_COLOUR, \
SBF_ONE_MINUS_DEST_COLOUR, \
SBF_ONE_MINUS_SOURCE_COLOUR, \
SBF_DEST_ALPHA, \
SBF_SOURCE_ALPHA, \
SBF_ONE_MINUS_DEST_ALPHA, \
SBF_ONE_MINUS_SOURCE_ALPHA = range(10)

@insertpiece( SetCrossPlatformSettings )

// START UNIFORM STRUCT DECLARATION
@property( !hlms_shadowcaster )
@insertpiece( MaterialStructDecl )
@end
@insertpiece( custom_ps_uniformStructDeclaration )
// END UNIFORM STRUCT DECLARATION
struct PS_INPUT
{
@insertpiece( VStoPS_block )
};

@insertpiece( PsOutputDecl )

@property( !hlms_shadowcaster )

@padd( numSamplerStates, num_array_textures, num_textures )
@pset( samplerStateBind, 2 )

@property( diffuse )@piece( MultiplyDiffuseConst )* material.diffuse@end @end

fragment @insertpiece( output_type ) main_metal
(
	PS_INPUT inPs [[stage_in]]
	// START UNIFORM DECLARATION
	@property( !hlms_shadowcaster )
		@insertpiece( MaterialDecl )
	@end
	@insertpiece( custom_ps_uniformDeclaration )
	// END UNIFORM DECLARATION
	@property( hlms_vpos ), float4 gl_FragCoord : [[position]]@end

	@foreach( num_array_textures, n )
		, texture2d_array<float> textureMapsArray@n [[texture(@value(array_texture_bind@n))]]@end
	@foreach( num_textures, n )
		, texture2d<float> textureMaps@n [[texture(@value(texture_bind@n))]]@end
	@foreach( numSamplerStates, n )
		, sampler samplerState@n [[sampler(@counter(samplerStateBind))]]@end
)
{
	PS_OUTPUT outPs;

	@insertpiece( custom_ps_preExecution )

	@property( diffuse_map || alpha_test || diffuse )Material material;@end

@property( diffuse_map || alpha_test || diffuse )
	material = materialArray[inPs.materialId];
@end

	@insertpiece( custom_ps_posMaterialLoad )

@property( !diffuse_map && !diffuse_map0 )
@property( hlms_colour && !diffuse_map )	outPs.colour0 = inPs.colour @insertpiece( MultiplyDiffuseConst );@end
@property( !hlms_colour && !diffuse )	outPs.colour0 = float4(1, 1, 1, 1);@end
@property( !hlms_colour && diffuse )	outPs.colour0 = material.diffuse;@end
@end

@property( diffuse_map )@property( diffuse_map0 )
	//Load base image
	outPs.colour0 = @insertpiece( TextureOrigin0 ).sample( @insertpiece( SamplerOrigin0 ), @insertpiece( SamplerUV0 ) ).@insertpiece(diffuse_map0_tex_swizzle);@end

@foreach( diffuse_map, n, 1 )@property( diffuse_map@n )
	float4 topImage@n = @insertpiece( TextureOrigin@n ).sample( @insertpiece( SamplerOrigin@n ), @insertpiece( SamplerUV@n ) ).@insertpiece(diffuse_map@n_tex_swizzle);@end @end

@foreach( diffuse_map, n, 1 )@property( diffuse_map@n )
	@insertpiece( blend_mode_idx@n )@end @end

	@property( hlms_colour )outPs.colour0 *= inPs.colour @insertpiece( MultiplyDiffuseConst );@end
	@property( !hlms_colour && diffuse )outPs.colour0 *= material.diffuse;@end
@end

	@insertpiece( custom_ps_preLights )

@property( alpha_test )
	if( material.alpha_test_threshold.x @insertpiece( alpha_test_cmp_func ) outPs.colour0.a )
		discard;@end

	@insertpiece( custom_ps_posExecution )

@property( !hlms_render_depth_only )
	return outPs;
@end
}
@end @property( hlms_shadowcaster )
	@property( hlms_render_depth_only )
		@set( hlms_disable_stage, 1 )
	@end
fragment @insertpiece( output_type ) main_metal( PS_INPUT inPs
@property( hlms_vpos ), float4 gl_FragCoord : SV_Position@end ) : SV_Target0
{
	@insertpiece( custom_ps_preExecution )

	PS_OUTPUT outPs;
	outPs.colour0.x = inPs.depth.x;

	@insertpiece( custom_ps_posExecution )

	return outPs;
}
@end

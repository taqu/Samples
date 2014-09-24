SET COMPILER=..\..\..\..\ShaderCompiler.exe
SET SHADERDIR=.\src\render\shader\

pushd %SHADERDIR%
%COMPILER% -vs -p5 DefaultPN.vs DefaultPNVS.byte
%COMPILER% -vs -p5 DefaultPNU.vs DefaultPNUVS.byte
%COMPILER% -ps -p5 Default.ps DefaultPS.byte
%COMPILER% -ps -p5 DefaultTexture.ps DefaultTexturePS.byte

%COMPILER% -vs -p5 FullQuad.vs FullQuadVS.byte

%COMPILER% -vs -p5 Depth.vs DepthVS.byte
%COMPILER% -gs -p5 Depth.gs DepthGS.byte
%COMPILER% -ps -p5 Depth.ps DepthPS.byte

%COMPILER% -vs -p5 Rectangle.vs RectangleVS.byte
%COMPILER% -ps -p5 Rectangle.ps RectanglePS.byte

%COMPILER% -vs -p5 DebugVC.vs DebugVCVS.byte
%COMPILER% -ps -p5 DebugVC.ps DebugVCPS.byte

%COMPILER% -vs -p5 FXAA.vs FXAAVS.byte
%COMPILER% -ps -p5 FXAA.ps FXAAPS.byte

%COMPILER% -ps -p5 TonemappingStatic.ps TonemappingStaticPS.byte

%COMPILER% -ps -p5 Glare.ps GlarePS.byte

%COMPILER% -ps -p5 Gauss.ps GaussPS.byte

%COMPILER% -vs -p5 SkyBox.vs SkyBoxVS.byte
%COMPILER% -ps -p5 SkyBox.ps SkyBoxPS.byte

%COMPILER% -vs -p5 Debug.vs DebugVS.byte
%COMPILER% -ps -p5 Debug.ps DebugPS.byte


%COMPILER% -vs -p5 Voxelize.vs VoxelizeVS.byte
%COMPILER% -gs -p5 Voxelize.gs VoxelizeGS.byte
%COMPILER% -ps -p5 Voxelize.ps VoxelizePS.byte

%COMPILER% -vs -p5 VoxelRender.vs VoxelRenderVS.byte
%COMPILER% -gs -p5 VoxelRender.gs VoxelRenderGS.byte
%COMPILER% -ps -p5 VoxelRender.ps VoxelRenderPS.byte

%COMPILER% -vs -p5 ReflectiveShadowMap.vs ReflectiveShadowMapVS.byte
%COMPILER% -ps -p5 ReflectiveShadowMap.ps ReflectiveShadowMapPS.byte
%COMPILER% -vs -p5 ReflectiveShadowMapTexture.vs ReflectiveShadowMapTextureVS.byte
%COMPILER% -ps -p5 ReflectiveShadowMapTexture.ps ReflectiveShadowMapTexturePS.byte

%COMPILER% -vs -p5 VoxelInjection.vs VoxelInjectionVS.byte
%COMPILER% -ps -p5 VoxelInjection.ps VoxelInjectionPS.byte

%COMPILER% -vs -p5 VoxelMipmap.vs VoxelMipmapVS.byte
%COMPILER% -ps -p5 VoxelMipmap.ps VoxelMipmapPS.byte

popd

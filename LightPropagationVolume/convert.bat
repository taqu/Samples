SET COMPILER=..\..\..\..\ShaderCompiler.exe
SET SHADERDIR=.\src\render\shader\

pushd %SHADERDIR%
%COMPILER% -vs -p4 DefaultPN.vs DefaultPNVS.byte
%COMPILER% -vs -p4 DefaultPNU.vs DefaultPNUVS.byte
%COMPILER% -ps -p4 Default.ps DefaultPS.byte
%COMPILER% -ps -p4 DefaultTexture.ps DefaultTexturePS.byte

%COMPILER% -vs -p4 FullQuad.vs FullQuadVS.byte

%COMPILER% -vs -p4 Depth.vs DepthVS.byte
%COMPILER% -gs -p4 Depth.gs DepthGS.byte
%COMPILER% -ps -p4 Depth.ps DepthPS.byte

%COMPILER% -vs -p4 Rectangle.vs RectangleVS.byte
%COMPILER% -ps -p4 Rectangle.ps RectanglePS.byte

%COMPILER% -vs -p4 DebugVC.vs DebugVCVS.byte
%COMPILER% -ps -p4 DebugVC.ps DebugVCPS.byte

%COMPILER% -vs -p4 FXAA.vs FXAAVS.byte
%COMPILER% -ps -p4 FXAA.ps FXAAPS.byte

%COMPILER% -ps -p4 TonemappingStatic.ps TonemappingStaticPS.byte

%COMPILER% -ps -p4 Glare.ps GlarePS.byte

%COMPILER% -ps -p4 Gauss.ps GaussPS.byte

%COMPILER% -ps -p4 HDAO.ps HDAOPS.byte

%COMPILER% -vs -p4 ReflectiveShadowMap.vs ReflectiveShadowMapVS.byte
%COMPILER% -ps -p4 ReflectiveShadowMap.ps ReflectiveShadowMapPS.byte

%COMPILER% -vs -p4 LPVInjection.vs LPVInjectionVS.byte
%COMPILER% -gs -p4 LPVInjection.gs LPVInjectionGS.byte
%COMPILER% -ps -p4 LPVInjection.ps LPVInjectionPS.byte

%COMPILER% -vs -p4 LPVOcclusionInjection.vs LPVOcclusionInjectionVS.byte
%COMPILER% -ps -p4 LPVOcclusionInjection.ps LPVOcclusionInjectionPS.byte

%COMPILER% -vs -p4 LPVPropagate.vs LPVPropagateVS.byte
%COMPILER% -gs -p4 LPVPropagate.gs LPVPropagateGS.byte
%COMPILER% -ps -p4 LPVPropagate.ps LPVPropagatePS.byte
%COMPILER% -ps -p4 LPVPropagateSeparate.ps LPVPropagateSeparatePS.byte

%COMPILER% -ps -p4 LPVOcclusionPropagate.ps LPVOcclusionPropagatePS.byte
%COMPILER% -ps -p4 LPVAccumulate.ps LPVAccumulatePS.byte

%COMPILER% -vs -p4 LPVRender.vs LPVRenderVS.byte
%COMPILER% -vs -p4 LPVRenderSkinning.vs LPVRenderSkinningVS.byte
%COMPILER% -ps -p4 LPVRender.ps LPVRenderPS.byte
popd

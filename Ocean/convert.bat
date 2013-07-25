SET COMPILER=..\..\..\..\ShaderCompiler.exe
SET SHADERDIR=.\src\render\shader\

pushd %SHADERDIR%
%COMPILER% -vs -p4 Default.vs DefaultVS.byte
%COMPILER% -ps -p4 Default.ps DefaultPS.byte
%COMPILER% -ps -p4 DefaultTexture.ps DefaultTexturePS.byte

%COMPILER% -vs -p4 FullQuad.vs FullQuadVS.byte

%COMPILER% -vs -p4 Depth.vs DepthVS.byte
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

%COMPILER% -vs -p4 Ocean.vs OceanVS.byte
%COMPILER% -ps -p4 Ocean.ps OceanPS.byte
%COMPILER% -ps -p4 OceanWire.ps OceanWirePS.byte

%COMPILER% -vs -p4 SkyBox.vs SkyBoxVS.byte
%COMPILER% -ps -p4 SkyBox.ps SkyBoxPS.byte

%COMPILER% -ps -p4 HDAO.ps HDAOPS.byte
popd

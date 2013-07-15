SET COMPILER=..\..\..\..\ShaderCompiler.exe
SET SHADERDIR=.\src\render\shader\

pushd %SHADERDIR%
%COMPILER% -vs -p5 Default.vs DefaultVS.byte
%COMPILER% -ps -p5 Default.ps DefaultPS.byte
%COMPILER% -ps -p5 DefaultTexture.ps DefaultTexturePS.byte

%COMPILER% -vs -p5 Depth.vs DepthVS.byte
%COMPILER% -ps -p5 Depth.ps DepthPS.byte

%COMPILER% -vs -p5 Rectangle.vs RectangleVS.byte
%COMPILER% -ps -p5 Rectangle.ps RectanglePS.byte

%COMPILER% -vs -p5 DebugVC.vs DebugVCVS.byte
%COMPILER% -ps -p5 DebugVC.ps DebugVCPS.byte

%COMPILER% -ps -p5 TonemappingStatic.ps TonemappingStaticPS.byte

%COMPILER% -vs -p5 Voxelize.vs VoxelizeVS.byte
%COMPILER% -gs -p5 Voxelize.gs VoxelizeGS.byte
%COMPILER% -ps -p5 Voxelize.ps VoxelizePS.byte
%COMPILER% -ps -p5 VoxelizeTexture.ps VoxelizeTexturePS.byte

%COMPILER% -vs -p5 VoxelRender.vs VoxelRenderVS.byte
%COMPILER% -ps -p5 VoxelRender.ps VoxelRenderPS.byte

%COMPILER% -vs -p5 OctreeTag.vs OctreeTagVS.byte
%COMPILER% -ps -p5 OctreeTag.ps OctreeTagPS.byte

%COMPILER% -ps -p5 OctreeCreateNode.ps OctreeCreateNodePS.byte

%COMPILER% -ps -p5 OctreeInitNode.ps OctreeInitNodePS.byte

%COMPILER% -ps -p5 OctreeWriteValue.ps OctreeWriteValuePS.byte

%COMPILER% -ps -p5 OctreePropagate.ps OctreePropagatePS.byte

%COMPILER% -ps -p5 OctreeMipmap.ps OctreeMipmapPS.byte

%COMPILER% -ps -p5 DefaultAO.ps DefaultAOPS.byte
%COMPILER% -ps -p5 DefaultAOTexture.ps DefaultAOTexturePS.byte
popd

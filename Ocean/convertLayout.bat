SET COMPILER=..\..\..\..\ShaderCompiler.exe
SET SHADERDIR=.\src\render\shader\

pushd %SHADERDIR%
%COMPILER% -vs Layout_P.vs Layout_P.byte
%COMPILER% -vs Layout_PN.vs Layout_PN.byte
%COMPILER% -vs Layout_PU.vs Layout_PU.byte
%COMPILER% -vs Layout_PC.vs Layout_PC.byte
%COMPILER% -vs Layout_PNTB.vs Layout_PNTB.byte
%COMPILER% -vs Layout_PNU.vs Layout_PNU.byte
%COMPILER% -vs Layout_PNCU.vs Layout_PNCU.byte
%COMPILER% -vs Layout_PNTBU.vs Layout_PNTBU.byte
%COMPILER% -vs Layout_PNUBone.vs Layout_PNUBone.byte
%COMPILER% -vs Layout_Ocean.vs Layout_Ocean.byte
popd

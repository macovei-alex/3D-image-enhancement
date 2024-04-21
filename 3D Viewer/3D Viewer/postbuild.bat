@echo off
set "dirname=..\x64\Debug\Models"

if not exist "%dirname%" (
    mkdir "%dirname%"
    echo Directory "%dirname%" created successfully.
) else (
    echo Directory "%dirname%" already exists.
)

set "dirname=..\x64\Debug\Shaders"

if not exist "%dirname%" (
    mkdir "%dirname%"
    echo Directory "%dirname%" created successfully.
) else (
    echo Directory "%dirname%" already exists.
)

copy Shaders\lightingFS.glsl "..\x64\Debug\Shaders\lightingFS.glsl"
copy Shaders\lightingVS.glsl "..\x64\Debug\Shaders\lightingVS.glsl"
copy Shaders\modelFS.glsl "..\x64\Debug\Shaders\modelFS.glsl"
copy Shaders\modelVS.glsl "..\x64\Debug\Shaders\modelVS.glsl"
copy Shaders\noTransformVS.glsl "..\x64\Debug\Shaders\noTransformVS.glsl"
copy Shaders\noTransformFS.glsl "..\x64\Debug\Shaders\noTransformFS.glsl"
copy Models\lightModel.txt "..\x64\Debug\Models\lightModel.txt"
copy Models\model.txt "..\x64\Debug\Models\model.txt"
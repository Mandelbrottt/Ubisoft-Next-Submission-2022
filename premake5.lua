-- Config
user_cfg = {
    -- Application settings
    app_name = "Planetar",
    exe_dir  = "Executable/",
    
    -- Project settings
    do_release_symbols = "on",
    do_release_console = true,
}

build_cfg = {
    source_dir = "%{wks.location}/Source",
    test_dir   = "%{wks.location}/Tests",
    
    -- Build locations
    output_dir = "%{wks.location}/Build/Bin/%{cfg.architecture}/%{cfg.buildcfg}",
    obj_dir    = "%{wks.location}/Build/Obj/%{cfg.architecture}/%{cfg.buildcfg}",

    resource_dir = "./Resources/",
}

build_cfg.exe_output_dir = build_cfg.output_dir .. "/" .. user_cfg.exe_dir

include "util.lua"

if _ACTION == 'clean' then
    os.rmdir('Build')
    os.remove('**.sln')
    os.remove('**.vcxproj')
    os.remove('**.vcxproj.*')
 end

workspace "NextSubmission"
    architecture "x64"

    configurations {
        "Debug", 
        "Development",
        "Release"
    }

    startproject "NextBootstrap"

    group "Vendor"

        include "Source/NextAPI"
    
    group "Engine"

        include "Source/NextCore"

        include "Source/NextBootstrap"

        group "Engine/PreProcessors"

            include "Source/NextPreProcessors/ReflectionHelper"

    group "User Modules"

        include "Source/NextGame"

    group "Unit Tests"
    
       include "Tests/NextCoreTests"

    group ""
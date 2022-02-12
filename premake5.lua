output_dir = "%{wks.location}/Build/Bin/%{cfg.architecture}/%{cfg.buildcfg}"
obj_dir    = "%{wks.location}/Build/Obj/%{cfg.architecture}/%{cfg.buildcfg}"

workspace "NextSubmission"
    architecture "x64"

    configurations {
        "Debug", 
        "Release"
    }

    startproject "NextGame"

    include "NextAPI"
    
    include "NextCore"

    include "NextGame"
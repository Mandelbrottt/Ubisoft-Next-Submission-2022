-- Config
user_cfg = {
    -- Application settings
    app_name = "NextGame",
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

        -- filter { "toolset:msc*", "configurations:Release*" }

    group "User Modules"

        include "Source/NextGame"

    group "Unit Tests"
    
       include "Tests/NextCoreTests"

    group ""


-- function m.optimizeReferences(cfg)
	-- if config.isOptimizedBuild(cfg) then
		-- m.element("EnableCOMDATFolding", nil, "true")
		-- m.element("OptimizeReferences", nil, "true")
	-- end
-- end

require('vstudio')

premake.override(premake.vstudio.vc2010, "optimizeReferences", function(base, cfg)
    local config = premake.config
    local m      = premake.vstudio.vc2010
    
    if config.isOptimizedBuild(cfg) then
        m.element("EnableCOMDATFolding", nil, "true")
        m.element("OptimizeReferences", nil, "false")
    end
end)
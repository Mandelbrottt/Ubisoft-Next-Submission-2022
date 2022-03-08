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
}

build_cfg.exe_output_dir = build_cfg.output_dir .. "/" .. user_cfg.exe_dir

local function mysplit (inputstr, sep)
    if sep == nil then
       sep = "%s"
    end
    local t={}
    for str in string.gmatch(inputstr, "([^"..sep.."]+)") do
       table.insert(t, str)
    end
    return t
end

-- https://stackoverflow.com/a/23535333
function script_dir()
    local str = debug.getinfo(2, "S").source:sub(2)
    local path = str:match("(.*[/\\])") or "."
    local split = mysplit(path, "/\\")
    local last_dir
    for _, v in ipairs(split) do
        last_dir = v
    end
    return last_dir
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

    group "User Modules"

        include "Source/NextGame"

    group "Unit Tests"
    
       include "Tests/NextCoreTests"

    group ""
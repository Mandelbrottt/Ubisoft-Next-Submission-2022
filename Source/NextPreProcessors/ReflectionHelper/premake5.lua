local project_name = script_dir()

project(project_name)
    kind          "ConsoleApp"
    language      "C++"
    cppdialect    "C++17"
    staticruntime "off"
    
    flags {
        "FatalWarnings",
        "MultiProcessorCompile"
    }
    
    -- Output to the executable directory instead of a project directory
    targetdir(build_cfg.output_dir .. "/PreProcessors/")
    objdir   (build_cfg.obj_dir    .. "/%{prj.name}/")

    files {
        "%{prj.location}/%{prj.name}/**.cpp",
        "%{prj.location}/%{prj.name}/**.h",
    }

    defines {
        "WORKSPACE_DIR=\"../../\""
    }
    
    filter "system:windows"
        systemversion "latest"

    filter "configurations:Debug"
        runtime  "debug"
        optimize "off"
        symbols  "on"
    
    filter "configurations:Development"
        runtime  "release"
        optimize "debug"
        symbols  "on"

    filter "configurations:Release"
        runtime  "release"
        optimize "speed"
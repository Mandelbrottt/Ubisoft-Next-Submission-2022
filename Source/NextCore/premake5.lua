local project_name = script_dir()

project(project_name)
    location     (build_cfg.source_dir .. "/%{prj.name}")
    kind          "StaticLib"
    language      "C++"
    cppdialect    "C++17"
    staticruntime "Off"
    floatingpoint "Fast"
    rtti          "On"
    
    flags {
        "FatalWarnings",
        "MultiProcessorCompile"
    }
    
    targetdir(build_cfg.output_dir .. "/%{prj.name}/")
    objdir   (build_cfg.obj_dir    .. "/%{prj.name}/")

    pchheader "pch.h"
    pchsource "pch/pch.cpp"

    files {
        "%{prj.location}/%{prj.name}/**.cpp",
        "%{prj.location}/%{prj.name}/**.h",
        "%{prj.location}/pch/*",
    }

    includedirs {
        "%{prj.location}/",
        "%{prj.location}/pch/",
        "%{prj.location}/NextCore/",
        build_cfg.source_dir .. "/NextAPI/",
    }

    links {
        "NextAPI",
    }

    libdirs {
        build_cfg.output_dir .. "/NextAPI/",
    }

    defines {
        "NEXT_CORE",
        "MMNOSOUND",
        "NEXT_RESOURCE_DIR=\"" .. build_cfg.resource_dir .. "\""
    }
    
    filter { "options:nocustomcode" }
        defines { "NEXT_API_NO_CUSTOM_CODE" }
    filter {}

    filter "system:windows"
        systemversion "latest"
    
    filter "configurations:Debug"
        defines { "NEXT_DEBUG", }
        runtime "Debug"
        optimize "Off"
        symbols "On"
    
    filter "configurations:Development"
        defines { "NEXT_DEVELOPMENT" }
        runtime "Release"
        optimize "Debug"
        symbols "On"
    
    filter "configurations:Release"
        defines { 
            "NEXT_RELEASE",
        }
        runtime "Release"
        optimize "Speed"
        symbols(user_cfg.do_release_symbols)
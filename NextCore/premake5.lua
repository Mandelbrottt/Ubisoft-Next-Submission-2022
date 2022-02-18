local project_name = script_dir()

project(project_name)
    location "%{wks.location}/%{prj.name}"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "off"
    floatingpoint "fast"
    rtti "on"
    
    flags {
        "FatalWarnings",
        "MultiProcessorCompile"
    }
    
    targetdir(output_dir .. "/%{prj.name}/")
    objdir   (obj_dir    .. "/%{prj.name}/")

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
        "%{wks.location}/NextAPI/",
    }

    links {
        "NextAPI",
    }

    libdirs {
        output_dir .. "/NextAPI/",
    }

    defines {
        "NEXT_CORE",
        "MMNOSOUND",
    }
    
    
    filter "system:windows"
    systemversion "latest"
    
    filter "configurations:Debug"
        defines { 
            "NEXT_DEBUG",
            -- "NEXT_RESOURCE_DIR=\"%{wks.location}/Resources/\""
            "NEXT_RESOURCE_DIR=\"./\""
        }
        runtime "debug"
        optimize "off"
        symbols "on"
    
    -- filter "configurations:Development"
    --     defines { "NEXT_DEVELOPMENT" }
    --     runtime "release"
    --     optimize "debug"
    --     symbols "on"
    
    filter "configurations:Release"
        defines { 
            "NEXT_RELEASE",
            "NEXT_RESOURCE_DIR=\"./Resources/\""
        }
        runtime "release"
        optimize "speed"
        symbols(do_release_symbols)
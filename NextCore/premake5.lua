project "NextCore"
    location "%{wks.location}/NextCore"
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
    pchsource "pch.cpp"

    files {
        "%{prj.location}/%{prj.name}/**.cpp",
        "%{prj.location}/%{prj.name}/**.h",
        "%{prj.location}/pch.*",
    }

    includedirs {
        "%{prj.location}/",
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
        "NEXT_CORE"
    }

    
    filter "system:windows"
        systemversion "latest"

    filter "configurations:Debug"
        defines { "NEXT_DEBUG" }
        runtime "debug"
        optimize "off"
        symbols "on"
    
    -- filter "configurations:Development"
    --     defines { "NEXT_DEVELOPMENT" }
    --     runtime "release"
    --     optimize "debug"
    --     symbols "on"
    
    filter "configurations:Release"
        defines { "NEXT_RELEASE" }
        runtime "release"
        optimize "speed"
        symbols "off"
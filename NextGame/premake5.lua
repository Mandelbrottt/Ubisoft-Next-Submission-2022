project "NextGame"
    location "%{wks.location}/NextGame/"
    kind "ConsoleApp"
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
        "%{prj.location}/NextGame/",
        "%{wks.location}/NextAPI/",
        "%{wks.location}/NextCore/",
        "%{wks.location}/NextCore/NextCore/",
    }

    links {
        "NextAPI", -- Eventually remove as dependency
        "NextCore",
        "dxguid",
        "dsound"
    }

    libdirs {
        output_dir .. "/NextAPI/",
        output_dir .. "/NextCore/",
    }

    defines {
        "NEXT_GAME",
        "MMNOSOUND",
    }

    filter "architecture:Win32"
        libdirs {
            "%{wks.location}/NextAPI/glut/lib/",
        }
        
        postbuildcommands {
            "{COPY} \"%{wks.location}NextAPI/glut/bin/*.dll\" \"" .. output_dir .. "/%{prj.name}/\"",
        }

    filter "architecture:x64"
        libdirs {
            "%{wks.location}/NextAPI/glut/lib/x64/",
        }
        
        postbuildcommands {
            "{COPY} \"%{wks.location}NextAPI/glut/bin/x64/*.dll\" \"" .. output_dir .. "/%{prj.name}/\"",
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
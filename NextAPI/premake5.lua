local project_name = script_dir()

project(project_name)
    location      "%{wks.location}/%{prj.name}"
    kind          "StaticLib"
    language      "C++"
    cppdialect    "C++14"
    staticruntime "off"
    floatingpoint "fast"

    -- Disable warnings because this isn't our code
    warnings "off" 
    
    flags {
        "MultiProcessorCompile"
    }
    
    targetdir(user_config.output_dir .. "/%{prj.name}/")
    objdir   (user_config.obj_dir    .. "/%{prj.name}/")

    pchheader "stdafx.h"
    pchsource "stdafx.cpp"

    files {
        "%{prj.location}/%{prj.name}/**.cpp",
        "%{prj.location}/%{prj.name}/**.h",
        "%{prj.location}/*.cpp",
        "%{prj.location}/*.h",
    }

    removefiles { 
        "*GameTest.cpp", 
        "**/DirectSound.*", 
        "**/mydirectsound.h" 
    }

    includedirs {
        "%{prj.location}/",
        "%{wks.location}/NextAPI/",
    }

    links {
        "freeglut"
    }

    filter "architecture:Win32"
        libdirs {
            "%{prj.location}/glut/lib/",
        }

    filter "architecture:x64"
        libdirs {
            "%{prj.location}/glut/lib/x64/",
        }

    filter {}

    defines {
        "NEXT_API",
        "MMNOSOUND",
    }

    filter "system:windows"
        systemversion "latest"

    filter "configurations:Debug"
        defines { "NEXT_DEBUG" }
        runtime "debug"
        optimize "off"
        symbols "on"
    
    filter "configurations:Development"
        defines { "NEXT_DEVELOPMENT" }
        runtime "release"
        optimize "debug"
        symbols "on"
    
    filter "configurations:Release"
        defines { "NEXT_RELEASE" }
        runtime "release"
        optimize "speed"
        symbols(user_config.do_release_symbols)
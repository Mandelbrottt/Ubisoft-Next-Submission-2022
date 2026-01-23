local project_name = script_dir()

project(project_name)
    location     (build_cfg.source_dir .. "/%{prj.name}")
    kind          "StaticLib"
    language      "C++"
    cppdialect    "C++14"
    staticruntime "Off"
    floatingpoint "Fast"

    -- Disable warnings because this isn't our code
    warnings "Off" 
    
    flags {
        "MultiProcessorCompile"
    }
    
    targetdir(build_cfg.output_dir .. "/%{prj.name}/")
    objdir   (build_cfg.obj_dir    .. "/%{prj.name}/")

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
        build_cfg.source_dir .. "/NextAPI/",
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

    filter { "options:nocustomcode" }
        defines { "NEXT_API_NO_CUSTOM_CODE" }
    filter {}

    filter "system:windows"
        systemversion "latest"

    filter "configurations:Debug"
        defines { "NEXT_DEBUG" }
        runtime "Debug"
        optimize "Off"
        symbols "On"
    
    filter "configurations:Development"
        defines { "NEXT_DEVELOPMENT" }
        runtime "Release"
        optimize "Debug"
        symbols "On"
    
    filter "configurations:Release"
        defines { "NEXT_RELEASE" }
        runtime "Release"
        optimize "Speed"
        symbols(user_cfg.do_release_symbols)
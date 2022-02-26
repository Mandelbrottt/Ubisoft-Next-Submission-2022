local project_name = script_dir()

project(project_name)
    location      "%{wks.location}/Tests/%{prj.name}"
    kind          "ConsoleApp"
    language      "C++"
    cppdialect    "C++17"
    staticruntime "off"
    floatingpoint "fast"
    rtti          "on"

    nuget { "Microsoft.googletest.v140.windesktop.msvcstl.static.rt-dyn:1.8.1.4" }
    
    flags {
        "FatalWarnings",
        "MultiProcessorCompile"
    }
    
    targetdir(user_config.output_dir .. "/Tests/%{prj.name}/")
    objdir   (user_config.obj_dir    .. "/Tests/%{prj.name}/")

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
        "%{prj.location}/%{prj.name}/",
        "%{wks.location}/NextCore/",
        "%{wks.location}/NextCore/NextCore/",
        "%{wks.location}/NextGUI/",
    }

    links {
        "NextCore",
        "NextGUI",
    }

    libdirs {
        user_config.output_dir .. "/NextCore/",
        user_config.output_dir .. "/NextGUI/",
    }

    defines {
        "NEXT_TESTS",
        "MMNOSOUND",
        "NEXT_RESOURCE_DIR=\"./Resources/\""
    }

    postbuildcommands {
        "\"%{cfg.buildtarget.abspath}\""
    }

    filter "system:windows"
        systemversion "latest"
    
    filter "configurations:Debug"
        defines { "NEXT_DEBUG", }
        runtime "debug"
        optimize "off"
        symbols "on"
    
    filter "configurations:Development"
        defines { "NEXT_DEVELOPMENT" }
        runtime "release"
        optimize "debug"
        symbols "on"
    
    filter "configurations:Release"
        defines { 
            "NEXT_RELEASE",
        }
        runtime "release"
        optimize "speed"
        symbols(user_config.do_release_symbols)
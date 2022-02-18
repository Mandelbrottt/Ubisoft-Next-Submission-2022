local project_name = script_dir()

project(project_name)
    location      "%{wks.location}/%{prj.name}/"
    kind          "StaticLib"
    language      "C++"
    cppdialect    "C++17"
    staticruntime "off"
    floatingpoint "fast"
    rtti          "on"
    
    flags {
        "FatalWarnings",
        "MultiProcessorCompile"
    }
    
    targetdir(user_config.output_dir .. "/%{prj.name}/")
    objdir   (user_config.obj_dir    .. "/%{prj.name}/")

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
        "%{wks.location}/NextCore/",
    }

    links {
        "NextCore",
    }

    libdirs {
        user_config.output_dir .. "/NextCore/",
    }

    defines {
        "NEXT_GUI",
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
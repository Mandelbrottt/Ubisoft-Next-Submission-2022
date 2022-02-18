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

    files {
        "%{prj.location}/%{prj.name}/**.cpp",
        "%{prj.location}/%{prj.name}/**.h",
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
    }

    libdirs {
        user_config.output_dir .. "/NextAPI/",
        user_config.output_dir .. "/NextCore/",
    }

    defines {
        "NEXT_GAME",
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

local project_name = script_dir()

project(project_name)
    location     (build_cfg.source_dir .. "/%{prj.name}/")
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

    targetdir(build_cfg.output_dir .. "/%{prj.name}/")
    objdir   (build_cfg.obj_dir    .. "/%{prj.name}/")

    files {
        "%{prj.location}/%{prj.name}/**.cpp",
        "%{prj.location}/%{prj.name}/**.h",
    }

    includedirs {
        "%{prj.location}/NextGame/",
        build_cfg.source_dir .. "/NextAPI/",
        build_cfg.source_dir .. "/NextCore/",
        build_cfg.source_dir .. "/NextCore/NextCore/",
    }

    links {
        "NextAPI", -- Eventually remove as dependency
        "NextCore",
    }

    libdirs {
        build_cfg.output_dir .. "/NextAPI/",
        build_cfg.output_dir .. "/NextCore/",
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
        symbols(user_cfg.do_release_symbols)

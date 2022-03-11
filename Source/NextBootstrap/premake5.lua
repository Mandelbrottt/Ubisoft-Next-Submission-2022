local project_name = script_dir()

project(project_name)
    location     (build_cfg.source_dir .. "/%{prj.name}/")
    kind          "WindowedApp"
    language      "C++"
    cppdialect    "C++17"
    staticruntime "off"
    floatingpoint "fast"
    rtti          "on"
    
    flags {
        "FatalWarnings",
        "MultiProcessorCompile"
    }
    
    -- Output to the executable directory instead of a project directory
    targetdir(build_cfg.exe_output_dir)
    objdir   (build_cfg.obj_dir    .. "/%{prj.name}/")

    targetname(user_cfg.app_name)

    files {
        "%{prj.location}/%{prj.name}/**.cpp",
        "%{prj.location}/%{prj.name}/**.h",
        "%{prj.location}/pch/*",
    }

    includedirs {
        "%{prj.location}/",
        "%{prj.location}/pch/",
        build_cfg.source_dir .. "/NextCore/",
    }

    links {
        "NextAPI", -- Eventually remove as dependency
        "NextCore",
        "NextCoreTests",
        "ReflectionHelper",
        -- TODO: Make function to generate links to projects dynamically to
        --       allow users to have multi-module projects
        "NextGame",
        "dxguid",
        "dsound"
    }

    libdirs {
        build_cfg.output_dir .. "/NextAPI/",
        build_cfg.output_dir .. "/NextCore/",
        build_cfg.output_dir .. "/%{prj.name}/",
    }

    defines {
        "NEXT_GUI",
        "MMNOSOUND",
        "NEXT_RESOURCE_DIR=\"" .. build_cfg.resource_dir .. "\""
    }

    prebuildcommands {
        "\"" .. build_cfg.output_dir .. "/PreProcessors/ReflectionHelper.exe\"",
    }

    postbuildcommands {
        -- TODO: Find a way to generate list of test executables
        "\"" .. build_cfg.output_dir .. "/Tests/NextCoreTests/NextCoreTests.exe\"",
    }
    
    filter "architecture:Win32"
        libdirs {
            build_cfg.source_dir .. "/NextAPI/glut/lib/",
        }
        
        postbuildcommands {
            "{COPY} \"" .. build_cfg.source_dir .. "/NextAPI/glut/bin/*.dll\" \"" .. build_cfg.exe_output_dir .. "\"",
        }

    filter "architecture:x64"
        libdirs {
            build_cfg.source_dir .. "/NextAPI/glut/lib/x64/",
        }
        
        postbuildcommands {
            "{COPY} \"" .. build_cfg.source_dir .. "/NextAPI/glut/bin/x64/*.dll\" \"" .. build_cfg.exe_output_dir .. "\"",
        }

    filter "system:windows"
        systemversion "latest"

    filter "configurations:Debug"
        defines  { "NEXT_DEBUG" }
        runtime  "debug"
        optimize "off"
        symbols  "on"
    
    filter "configurations:Development"
        defines  { "NEXT_DEVELOPMENT" }
        runtime  "release"
        optimize "debug"
        symbols  "on"

    filter "configurations:not Release"
        debugdir "%{wks.location}/"
    
    filter "configurations:Release"
        defines  { "NEXT_RELEASE" }
        runtime  "release"
        optimize "speed"
        symbols   (user_cfg.do_release_symbols)
        debugdir  (build_cfg.exe_output_dir)
        
        debugargs {
            user_cfg.do_release_console and "--console" or ""
        }
    
        -- Only copy game resources in Release because we use the resources in-place otherwise
        prebuildcommands {
            "{COPY} \"%{wks.location}Resources/\" \"" .. build_cfg.exe_output_dir .. "/Resources/\"",
        }
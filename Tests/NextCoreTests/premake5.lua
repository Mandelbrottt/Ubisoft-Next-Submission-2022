local project_name = script_dir()

project(project_name)
    location     (build_cfg.test_dir .. "/%{prj.name}")
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
    
    targetdir(build_cfg.output_dir .. "/Tests/%{prj.name}/")
    objdir   (build_cfg.obj_dir    .. "/Tests/%{prj.name}/")

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
        build_cfg.source_dir .. "/NextCore/",
        build_cfg.source_dir .. "/NextCore/NextCore/",
        build_cfg.source_dir .. "/NextGUI/",
    }

    links {
        "NextCore",
        "NextGUI",
        "NextAPI",
        "dxguid",
        "dsound"
    }

    libdirs {
        build_cfg.output_dir .. "/NextAPI/",
        build_cfg.output_dir .. "/NextCore/",
        build_cfg.output_dir .. "/NextGUI/",
    }

    defines {
        "NEXT_TESTS",
        "MMNOSOUND",
        "NEXT_RESOURCE_DIR=\"./Resources/\""
    }

    filter "architecture:Win32"
        libdirs {
            build_cfg.source_dir .. "/NextAPI/glut/lib/",
        }
        
        postbuildcommands {
            "{COPY} \"" .. build_cfg.source_dir .. "NextAPI/glut/bin/*.dll\" \"" .. build_cfg.exe_output_dir .. "\"",
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
        symbols(user_cfg.do_release_symbols)
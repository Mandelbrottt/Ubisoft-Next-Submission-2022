local project_name = script_dir()

project(project_name)
    location "%{wks.location}/%{prj.name}/"
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
    
    targetdir(exe_output_dir)
    objdir   (obj_dir    .. "/%{prj.name}/")

    targetname(app_name)

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
        "NextAPI", -- Eventually remove as dependency
        "NextCore",
        -- TODO: Make function to generate links to projects dynamically to
        --       allow users to have multi-module projects
        "NextGame",
        "dxguid",
        "dsound"
    }

    libdirs {
        output_dir .. "/NextAPI/",
        output_dir .. "/NextCore/",
        output_dir .. "/%{prj.name}/",
    }

    defines {
        "NEXT_GUI",
        "MMNOSOUND",
    }

    filter "architecture:Win32"
        libdirs {
            "%{wks.location}/NextAPI/glut/lib/",
        }
        
        postbuildcommands {
            "{COPY} \"%{wks.location}NextAPI/glut/bin/*.dll\" \"" .. exe_output_dir .. "\"",
        }

    filter "architecture:x64"
        libdirs {
            "%{wks.location}/NextAPI/glut/lib/x64/",
        }
        
        postbuildcommands {
            "{COPY} \"%{wks.location}NextAPI/glut/bin/x64/*.dll\" \"" .. exe_output_dir .. "\"",
        }

    filter "system:windows"
        systemversion "latest"

    filter "configurations:Debug"
        defines { "NEXT_DEBUG" }
        runtime "debug"
        optimize "off"
        symbols "on"
        debugdir "%{wks.location}/Resources/" -- Why doesn't this work?
    
    -- filter "configurations:Development"
    --     defines { "NEXT_DEVELOPMENT" }
    --     runtime "release"
    --     optimize "debug"
    --     symbols "on"
    
    filter "configurations:Release"
        defines { "NEXT_RELEASE" }
        runtime "release"
        optimize "speed"
        symbols(do_release_symbols)
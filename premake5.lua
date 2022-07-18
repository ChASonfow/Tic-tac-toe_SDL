workspace "Tic-tac-toe_SDL"
	architecture "x64"
	configurations 	{ "Debug", "Release" }

project "Tic-tac-toe"
	location "Tic-tac-toe"
	language "C++"

	local outputdir = "%{prj.name}/%{cfg.buildcfg}-x64"
	targetdir ("bin/" .. outputdir)
	objdir ("bin-obj/" .. outputdir)

	files { "%{prj.name}/src/**.h", "%{prj.name}/src/**.hpp", "%{prj.name}/src/**.cpp" }

	includedirs { "%{prj.name}/vendor/SDL/include", "%{prj.name}/vendor/SDL_ttf" }

	libdirs { 
			"%{prj.name}/vendor/SDL/VisualC/x64/%{cfg.buildcfg}"
		, 	"%{prj.name}/vendor/SDL_ttf/VisualC/x64/%{cfg.buildcfg}" 
	}

	links {
			"SDL2"
		,	"SDL2main"
		, 	"SDL2_ttf" 
	}

	filter "system:windows"
		cppdialect "C++20"
		staticruntime "On"
		systemversion "latest"

		postbuildcommands { 
				'copy /Y "$(SolutionDir)$(ProjectName)\\vendor\\SDL\\VisualC\\$(Platform)\\$(Configuration)\\SDL2.dll" "$(TargetDir)"'
			,	'copy /Y "$(SolutionDir)$(ProjectName)\\vendor\\SDL_ttf\\VisualC\\$(Platform)\\$(Configuration)\\SDL2_ttf.dll" "$(TargetDir)"'
			,	'xcopy "$(SolutionDir)$(ProjectName)\\res\\" "$(TargetDir)\\res\\" /Y /S'
		}

	filter "configurations:Debug"
		kind "ConsoleApp"
		defines { "DEBUG" }
		symbols "On"

	filter "configurations:Release"
		kind "WindowedApp"
		defines { "NDEBUG" }
		optimize "On"
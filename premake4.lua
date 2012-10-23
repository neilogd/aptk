
local action = _ACTION or ""

-- Setup the main solution.
solution "aptk"
	location ( "build/" .. action )
	configurations { "Debug", "Release", "Production" }

	configuration "vs*"
		defines { "STATICLIB", "_CRT_SECURE_NO_WARNINGS", "_STATIC_CPPLIB", "_HAS_EXCEPTIONS=0" }	

	configuration "Debug"
		targetdir ( "build/" .. action .. "/bin/Debug" )
		flags { "StaticRuntime", "EnableSSE", "EnableSSE2", "FloatFast", "NativeWChar", "NoPCH", "NoRTTI", "NoExceptions", "Symbols" }

	configuration "Release"
		targetdir ( "build/" .. action .. "/bin/Production" )
		flags { "StaticRuntime", "EnableSSE", "EnableSSE2", "FloatFast", "NativeWChar", "NoPCH", "NoRTTI", "NoExceptions", "NoFramePointer", "Optimize" }

	project "aptk_lib"
		kind "StaticLib"
		language "C"
		files { "./inc/aptk/**.h", "./src/aptk/**.c" }
		includedirs { "./inc" }

	project "aptk_test"
		kind "ConsoleApp"
		language "C"
		files { "./inc/test/**.h", "./src/test/**.c" }
		includedirs { "./inc" }
		configuration "windows"
			links {
				"aptk_lib"
			}

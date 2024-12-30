#!/usr/bin/env python

# --- INCLUDES
import os
import platform
import subprocess
import shutil
import sys

# --- REQUIREMENTS
from Setup_VulkanSDK import VulkanRequirements

# --- GLOBALS
FILE_EXT = [
	'.sln',
	'.vcxproj',
	'.filters',
	'.user',
]

FOLDERS = [
	'bin',
	'bin-int',
	'Solution'
]

# ---
'''
	clear_paths method
	@note : Delete output folders and solution files to regenerated clean solution.
'''
def clear_paths( ) :
	print( '\n> Clearing output folders :' )

	for path in os.listdir( '.' ) :
		if path in FOLDERS :
			print( f'> Deleted Folder : {path}' )

			shutil.rmtree( path )
		elif os.path.splitext( path )[ -1 ].lower( ) in FILE_EXT :
			print( f'> Deleted File : {path}' )

			os.remove( path )

'''
	call_premake method
	@note : Call premake for solution file generation.
'''
def call_premake( ) :
	print( '\n> Calling Premake5 :')

	premake_call = [ '--file=Build\\Build.lua', f'--vk_version={VulkanRequirements.SDK_Version}' ]

	args = sys.argv
	args.pop( 0 )

	premake_call.extend( args )

	if platform.system( ) == 'Windows' :
		premake_call.insert( 0, 'Build\\Premake\\Windows\\premake5.exe' )
	elif platform.system( ) == 'Linux' :
		premake_call.insert( 0, 'Build\\Premake\\Linux\\premake5' )

	subprocess.call( premake_call )

# --- MAIN LOGIC
if __name__ == "__main__" :
	print( '=== Setup Micro Vulkan ===' )

	VulkanRequirements.TryValidate( )

	clear_paths( )
	call_premake( )

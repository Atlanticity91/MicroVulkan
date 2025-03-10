#!/usr/bin/env python

import platform
import os

from packaging.version import Version
from Utils import Sha256_Check, Download_File

class VulkanRequirements :

	# GLOBAL PARAMETERS
	SDK_Version = '1.4.304.1'
	SDK_BaseURL = f'https://sdk.lunarg.com/sdk/download/{SDK_Version}/'
	SDK_Installer = [
		# Win32 - x64
		( 
			f'{SDK_BaseURL}windows/VulkanSDK-{SDK_Version}-Installer.exe',
			f'VulkanSDK-{SDK_Version}-Installer.exe',
			'acb4ae0786fd3e558f8b3c36cc3eba91638984217ba8a6795ec64d2f9ffd8c4b' 
		),
		# Win32 - arm64
		( 
			f'{SDK_BaseURL}warm/InstallVulkanARM64-{SDK_Version}.exe',
			f'InstallVulkanARM64-{SDK_Version}.exe',
			'457f6f42d1be886fd2131fa6b212167730399e316e7ee33a9a8e9543f7f3ccc2' 
		),
		# Linux
		( 
			f'{SDK_BaseURL}linux/vulkansdk-linux-x86_64-{SDK_Version}.tar.xz',
			f'vulkansdk-linux-x86_64-{SDK_Version}.tar.xz',
			'79b0a1593dadc46180526250836f3e53688a9a5fb42a0e5859eb72316dc4d53e' 
		),
		# Macos
		( 
			f'{SDK_BaseURL}mac/vulkansdk-macos-{SDK_Version}.zip',
			f'vulkansdk-macos-{SDK_Version}.zip',
			'393fd11f65a4001f12fd34fdd009c38045220ca3f735bc686d97822152b0f33c' 
		)
	]

	# FUNCTION DEFINITIONS
	'''
		TryDownloadSDK method
		@note : Try download minimum required Vulkan SDK version for current platform.
		@param platform_index : Current platform index for SDK_Installer list.
	'''
	@classmethod
	def TryDownloadSDK( clss, platform_index ) :
		user_input = str( input( f'> Would you like to install VulkanSDK {clss.SDK_Version:s}? [Y/N]: ' ) )
		user_reply = user_input.lower( ).strip( )[ :1 ]
		sdk_installer = clss.SDK_Installer[ platform_index ]
		sdk_path = sdk_installer[ 1 ]

		if user_reply == 'y' :
			if not Download_File( sdk_installer[ 0 ], sdk_path ) or not Sha256_Check( sdk_path, sdk_installer[ 2 ] ) :
				print( f'> Fail to download SDK {clss.SDK_Version}' )
			else :
				if platform_index < 2 :
					os.startfile( clss.SDK_Installer[ platform_index ][ 1 ] )

					print( f'> Restart setup script after SDK installation with debug libraries' )
				else :
					print( f'> Please launch SDK installer {sdk_path} from repository root directory, make sure SDK installation is under path/to/SDK/version/ and restart setup script' )

		quit( )

	'''
		TryLocalSDK function
		@note : Try local SDK version against minimum required version
		@param local_sdk_path : Current local SDK path.
		@return True : when local SDK version is inferior to minimum required SDK version.
	'''
	@classmethod
	def TryLocalSDK( clss, local_sdk_path ) :
		if os.path.exists( local_sdk_path ) and local_sdk_path is not None :
			local_sdk_version = local_sdk_path.split( '\\' )[ -1 ]
			
			if Version( clss.SDK_Version ) <= Version( local_sdk_version ) :
				clss.SDK_Version = local_sdk_version
				
				return True

		return False

	'''
		TryValidate method
		@note : Process Vulkan Minimum SDK requirements check.
	'''
	@classmethod
	def TryValidate( clss ):
		architecture = platform.architecture( )[ 0 ]

		if architecture == '32bit' :
			print( '> Error this repository is made and tested for 64bits platforms' )

			quit( )

		local_sdk_path = os.environ.get( 'VULKAN_SDK' )

		if not clss.TryLocalSDK( local_sdk_path ) :
			print( '> Vulkan SDK is not installed or version is not supported' )

			platform_index = 0
			platform_name = platform.system( )
			
			if platform_name == 'arm64' :
				platform_index = 1
			elif platform_name == 'linux' or platform_name == 'linux2' :
				platform_index = 2 
			elif platform_name == 'darwin' :
				platform_index = 3

			clss.TryDownloadSDK( platform_index )
		else :
			print( f'> Vulkan SDK {clss.SDK_Version} match at least minimum requirement' ) 

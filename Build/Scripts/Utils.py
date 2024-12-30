#!/usr/bin/env python

import hashlib
import os
import requests
import sys
import time
import urllib

'''
	Sha256_Check function
	@note : Check file Sha256 against reference value.
	@param file_path : Query file path.
	@param reference_sha : Query reference sha256 value.
	@return True : when reference sha256 and actual calculated file are equal.
'''
def Sha256_Check( file_path, reference_sha ) :
	sha256_hash = hashlib.sha256( )

	if os.path.isfile( file_path ) :
		with open( file_path, "rb" ) as file :
			for byte_block in iter( lambda : file.read( 4096 ), b"" ) :
				sha256_hash.update( byte_block )

			file.close( )

			return sha256_hash.hexdigest( ) == reference_sha

	return False

'''
	Download_File function
	@param url : Query file download url.
	@param file_path : Query file download path.
	@return True : when download succeded.
'''
def Download_File( url, file_path ) :
    print( f'> Downloading {url:s}' )

    if url is not None and type( url ) is str :
        filepath = os.path.abspath( file_path )
        dir_name = os.path.dirname( file_path )
        
        if dir_name != '' :
            os.makedirs( dir_name, exist_ok=True )

        try :
            with open( filepath, 'wb' ) as file :
                headers = { 'User-Agent' : 'Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_4) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/83.0.4103.97 Safari/537.36' }
                response = requests.get( url, headers=headers, stream=True )
                total = response.headers.get( 'content-length' )

                if total is None :
                    file.write( response.content )
                else :
                    downloaded = 0
                    total = int( total )
                    startTime = time.time( )

                    for data in response.iter_content( chunk_size=max( int( total / 1000 ), 1024*1024 ) ) :
                        downloaded += len( data )
                        file.write( data )
                        
                        try :
                            done = int( 50 * downloaded / total ) if downloaded < total else 50
                            percentage = ( downloaded / total ) * 100 if downloaded < total else 100
                        except ZeroDivisionError :
                            done = 50
                            percentage = 100

                        elapsedTime = time.time( ) - startTime
                        
                        try :
                            avgKBPerSecond = ( downloaded / 1024 ) / elapsedTime
                        except ZeroDivisionError :
                            avgKBPerSecond = 0.0

                        avgSpeedString = '{:.2f} KB/s'.format( avgKBPerSecond )

                        if ( avgKBPerSecond > 1024 ) :
                            avgMBPerSecond = avgKBPerSecond / 1024
                            avgSpeedString = '{:.2f} MB/s'.format( avgMBPerSecond )

                        sys.stdout.write( '\r> [{}{}] {:.2f}% ({})     '.format( '█' * done, '.' * ( 50 - done ), percentage, avgSpeedString ) )
                        sys.stdout.flush( )
                        
                sys.stdout.write('\n')

            return True
        except urllib.error.URLError as e :
            print( f'URL Error encountered : {e.reason}' )
        except urllib.error.HTTPError as e :
            print( f'HTTP Error encountered : {e.code}' )
        except :
            print( f'> Unknow error' )

        os.remove( filepath )

    return False

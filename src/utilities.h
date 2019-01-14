// #pragma once

// #include <iostream>
// string Replace( string String1, string String2, string String3 );
// bool doesIncludeExtensions(string _filename, vector<string>_extensions);
// using namespace std;

// string Replace( string String1, string String2, string String3 )
// {
//     string::size_type  Pos( String1.find( String2 ) );

//     while( Pos != string::npos )
//     {
//         String1.replace( Pos, String2.length(), String3 );
//         Pos = String1.find( String2, Pos + String3.length() );
//     }

//     return String1;
// }

// bool doesIncludeExtensions(string _filename, std::vector<string> _extensions){
//     string extname;

//     int ext_i = _filename.find_last_of(".");
//     if( ext_i > 0 ){
//         extname = _filename.substr(ext_i,_filename.size()-ext_i);
//     }
//     else{
//         return false;
//     }

//     for( int i = 0; i < _extensions.size(); i++ ){
//         if( _extensions[i] == extname ){
//             return true;
//         }
//     }
//     return false;
// }

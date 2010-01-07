/**
  * @file common/win32/filesystem_changes_publisher.cpp
  * Publishes filesystem changes events
  */
 
 /* Copyright (C) 2005 ireon.org developers council
  * $Id: filesystem_changes_publisher.cpp 693 2006-08-31 08:02:35Z zak $
 
  *  See LICENSE for details
  */

// !!
// compile with: /clr
// !!

#ifdef _WIN32 
#ifdef _MSC_VER

/*
#include "common/common_stdafx.h"
#include "common/win32/filesystem_changes_publisher.h"

#using <system.dll>

using namespace System;
using namespace System::IO;

ref class FSEventHandler
{
public:
    void OnChanged (Object^ source, FileSystemEventArgs^ e)
    {
        Console::WriteLine("File: {0} {1}", 
               e->FullPath, e->ChangeType);
    }
    void OnRenamed(Object^ source, RenamedEventArgs^ e)
    {
        Console::WriteLine("File: {0} renamed to {1}", 
                e->OldFullPath, e->FullPath);
    }
};


int main()
{
   array<String^>^ args = Environment::GetCommandLineArgs();

   if(args->Length < 2)
   {
      Console::WriteLine("Usage: Watcher.exe <directory>");
      return -1;
   }

   FileSystemWatcher^ fsWatcher = gcnew FileSystemWatcher( );
   fsWatcher->Path = args[1];
   fsWatcher->NotifyFilter = static_cast<NotifyFilters> 
              (NotifyFilters::FileName | 
               NotifyFilters::Attributes | 
               NotifyFilters::LastAccess | 
               NotifyFilters::LastWrite | 
               NotifyFilters::Security | 
               NotifyFilters::Size );

    FSEventHandler^ handler = gcnew FSEventHandler(); 
    fsWatcher->Changed += gcnew FileSystemEventHandler( 
            handler, &FSEventHandler::OnChanged);
    fsWatcher->Created += gcnew FileSystemEventHandler( 
            handler, &FSEventHandler::OnChanged);
    fsWatcher->Deleted += gcnew FileSystemEventHandler( 
            handler, &FSEventHandler::OnChanged);
    fsWatcher->Renamed += gcnew RenamedEventHandler( 
            handler, &FSEventHandler::OnRenamed);

    fsWatcher->EnableRaisingEvents = true;

    Console::WriteLine("Press Enter to quit the sample.");
    Console::ReadLine( );
}

*/

#endif
#endif
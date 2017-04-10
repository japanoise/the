/*
 * Check if .thegui exists in directory where this executable run from
 * If it exists, exec() theg.exe end exit
 * Check if .theconsole exists in directory where this executable run from
 * If it exists, exec() thec.exe end exit
 * If neither exists execute thec.exe
 * Allow option: --gui-default to create .thegui and delete .theconsole
 * Allow option: --console-default to create .theconsole and delete .thegui
 */
#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

//#define _WIN32_WINNT 0x0501
#if defined(_WIN32) || defined(_WIN64)
# include <windows.h>
# include <io.h>
# include <userenv.h>
#else
# include <unistd.h>
# include <limits.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <ctype.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

#ifdef __APPLE__
# include <mach-o/dyld.h>
#endif
#include "mygetopt.h"

#ifndef F_OK
# define F_OK 0
#endif

#if defined(_WIN32) || defined(_WIN64)
# define THE_CONSOLE_EXE "thec.exe"
# define THE_GUI_EXE     "theg.exe"
# define THE_PATH_SEP '\\'
#else
# ifdef USE_UTF8
#  define THE_CONSOLE_EXE "nwthe"
#  define THE_GUI_EXE     "xwthe"
# else
#  define THE_CONSOLE_EXE "nthe"
#  define THE_GUI_EXE     "xthe"
# endif
# define THE_PATH_SEP '/'
#endif

#define THE_DEFAULT 0
#define THE_GUI     1
#define THE_CONSOLE 2

char *the_version = THE_VERSION;
char *the_release = THE_VERSION_DATE;
char *the_copyright = "Copyright 1991-2011 Mark Hessling";

#ifdef PATH_MAX
/* if you don't use PATH_MAX (by #including <limits.h>
 * you will get a crash in realpath!
 */
char prog[PATH_MAX+1];
#else
char prog[1024];
#endif
char **my_argv;
int my_argc;

int newConsole = 0;

#if defined(_WIN32) || defined(_WIN64)
void StartupConsole( void )
{
   /*
    * the.exe is a GUI process that can be called from a parent
    * process that has a Console (such as running the.exe from an
    * open command prompt), or from Explorer or Run... in which
    * case the parent does not have a Console.
    * Attempt to attach to the Console of the parent process. If that
    * fails, we already have a Console, so use it. Otherwise, create
    * a Console to use.
    * MHES - 22/2/2013 - we don't attempt this anymore. Firstly it doesn't
    * appear to work, and secondly it precludes THE from running on Win2000
   if ( AttachConsole( ATTACH_PARENT_PROCESS ) == 0 )
    */
   {
      /*
       * We failed to attach to the parent's console, so we need
       * to create our own.
       */
      AllocConsole();
      newConsole = 1;
      freopen( "conin$", "r", stdin );
      freopen( "conout$", "w", stdout );
      freopen( "conout$", "w", stderr );
   }
}

void ClosedownConsole( void )
{
   if ( newConsole )
   {
      fprintf( stderr, "\n==> Press ENTER key to close this window. <==" );
      getchar();
      fclose(stdin);
      fclose(stdout);
      fclose(stderr);
      FreeConsole();
   }
}

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{
   int rc;
   rc = main( __argc, __argv );
   return rc;
}
#endif

void usage()
{
#ifdef __APPLE__
   char *buf,*app;
#endif
#ifdef __APPLE__
   char copyright[1024];
   char *msg =
               "THE is distributed under the terms of the GNU General Public \n"
               "License and comes with NO WARRANTY.\nSee the file COPYING for details.\n"
               "\nUsage:\n\nthe [[-h] [-C] [-G] [--gui-default] [--console-default] [THE arguments] [file [...]]]\n"
               "\nwhere:\n\n"
               "-h,-?                  show this message\n"
               "-C                     override the default mode and run the console version of THE\n"
               "-G                     override the default mode and run the GUI version of THE\n"
               "--console-default      run the console version of THE whenever this executable is executed\n"
               "--gui-default          run the GUI version of THE whenever this executable is executed\n";
   sprintf( copyright, "\nTHE %s %s %s. All rights reserved.\n", the_version, the_release, the_copyright );
   if ( getenv( "THE_RUNNING_AS_APP" ) )
      app = "The Hessling Editor";
   else
      app = "Terminal";
   buf = alloca( strlen(msg) + strlen(app) + strlen(copyright) + 200 );
   sprintf( buf, "osascript -e 'tell app \"%s\" to display alert \"THE Usage\" message \"%s%s\" as warning buttons {\"Ok\"}' > /dev/null 2>&1", app, copyright, msg );
   system( buf );
#else
# if defined(_WIN32) || defined(_WIN64)
   StartupConsole();
# endif
   fprintf(stdout,"\nTHE %s %s %s. All rights reserved.\n",the_version,the_release,the_copyright);
   fprintf(stdout,"THE is distributed under the terms of the GNU General Public License \n");
   fprintf(stdout,"and comes with NO WARRANTY. See the file COPYING for details.\n");
   fprintf(stdout,"\nUsage:\n\nthe [[-h] [-C] [-G] [--gui-default] [--console-default] [THE arguments] [file [...]]]\n");
   fprintf(stdout,"\nwhere:\n\n");
   fprintf(stdout,"-h,-?                  show this message\n");
   fprintf(stdout,"-C                     override the default mode and run the console version of THE\n");
   fprintf(stdout,"-G                     override the default mode and run the GUI version of THE\n");
   fprintf(stdout,"--console-default      run the console version of THE whenever this executable is executed\n");
   fprintf(stdout,"--gui-default          run the GUI version of THE whenever this executable is executed\n");
# if defined(_WIN32) || defined(_WIN64)
   ClosedownConsole();
# endif
#endif
}

/*
 * GetArgv0 tries to find the fully qualified filename of the current program.
 * It uses some ugly and system specific tricks and it may return NULL if
 * it can't find any useful value.
 * The argument should be argv[0] of main() and it may be returned.
 * This function must not be called from another as the sole one when starting
 * up.
 */
static char *GetArgv0(char *argv0)
{
#if defined(_WIN32)
   char buf[1024];

   if (GetModuleFileName(NULL, buf, sizeof(buf)) != 0)
      return(strdup(buf)); /* never freed up */
#elif defined(__QNX__) && defined(__WATCOMC__)
   char buffer[PATH_MAX];
   char *buf;
   if ( (buf = _cmdname(buffer) ) != NULL )
      return(strdup(buf)); /* never freed up */
#elif defined(OS2)
   char buf[512];
   PPIB ppib;

# ifdef __EMX__
   if (_osmode == OS2_MODE)
   {
# endif
      if (DosGetInfoBlocks(NULL, &ppib) == 0)
         if (DosQueryModuleName(ppib->pib_hmte, sizeof(buf), buf) == 0)
            return(strdup(buf));
# ifdef __EMX__
   }
# endif
#endif

#ifdef __APPLE__
   {
      /*
       * will work on MacOS X
       */
      char buf[1024];
      uint32_t result=sizeof(buf);
      if ( _NSGetExecutablePath( buf, &result ) == 0 )
      {
         return strdup( buf );
      }
   }
#elif defined(HAVE_READLINK)
   {
      /*
       * will work on Linux 2.1+
       */
      char buf[1024];
      int result;
      result = readlink("/proc/self/exe", buf, sizeof( buf ) );
      if ( ( result > 0 ) && ( result < (int) sizeof( buf ) ) && ( buf[0] != '[' ) )
      {
         buf[result] = '\0';
         return strdup( buf );
      }
   }
#endif
   /* No specific code has found the right file name. Maybe, it's coded
    * in argv0. Check it, if it is an absolute path. Be absolutely sure
    * to detect it safely!
    */
   if (argv0 == NULL)
      return(NULL);

   if (argv0[0] == '/') /* unix systems and some others */
      return(argv0);

   if ((argv0[0] == '\\') && (argv0[1] == '\\')) /* MS and OS/2 UNC names */
      return(argv0);

   if (isalpha(argv0[0]) && (argv0[1] == ':') && (argv0[2] == '\\'))
      return(argv0); /* MS and OS/2 drive letter with path */

   return(NULL); /* not a proven argv0 argument */
}

/*
 * Determines where the special marker files .thegui and .theconsole are read/written
 */
char *GetHomeDirectory()
{
   static char user_dir[1024] = "";
#if defined(_WIN32) || defined(_WIN64)
   HANDLE hToken = 0;

   /* Use the Windows API to get the user's profile directory */
   if ( OpenProcessToken( GetCurrentProcess(), TOKEN_QUERY, &hToken ) )
   {
      DWORD BufSize = 1024;

      GetUserProfileDirectory(hToken, user_dir, &BufSize);
      CloseHandle(hToken);
      strcat( user_dir, "\\" );
   }
   /* If it fails set it to the root directory */
   if(!user_dir[0])
   {
      strcpy(user_dir, "C:\\");
   }
#else
   strcpy( user_dir, getenv( "HOME" ) );
   strcat( user_dir, "/" );
#endif
   return user_dir;
}

int CreateMarkerFile( char *homedir, char *filename )
{
   char buf[1024];
   int fd;

   strcpy( buf, homedir );
   strcat( buf, filename );
   if ( access( buf, F_OK ) != 0 )
   {
#if defined(_WIN32) || defined(_WIN64)
      fd = open( buf, O_CREAT, _S_IWRITE);
#else
      fd = open( buf, O_CREAT|O_NOCTTY, S_IRWXU);
#endif
      close( fd );
   }
   return 0;
}

int CheckBinary( char *prog, int terminate, char *msg )
{
   int rc = 0;
#ifdef __APPLE__
   char buf1[1024];
   char buf[1024];
   char *app;
#endif
#ifdef X_OK
   if ( access( prog, X_OK ) == 0 )
#else
   if ( access( prog, F_OK ) == 0 )
#endif
      rc = 1;
   if ( terminate && rc == 0 )
   {
#if defined(_WIN32) || defined(_WIN64)
      StartupConsole();
#endif
#ifdef __APPLE__
      sprintf( buf1, msg, prog );
      if ( getenv( "THE_RUNNING_AS_APP" ) )
         app = "The Hessling Editor";
      else
         app = "Terminal";
      sprintf( buf, "osascript -e 'tell app \"%s\" to display alert \"THE binary not found\" message \"%s\" as warning buttons {\"Ok\"}' > /dev/null 2>&1", app, buf1 );
      system( buf );
      exit(1);
#else
   fprintf( stderr, msg, prog );
# if defined(_WIN32) || defined(_WIN64)
   ClosedownConsole();
# endif
   exit(1);
#endif
   }
   return rc;
}

int CheckMarkerFile( char *homedir, char *filename )
{
   char buf[1024];

   strcpy( buf, homedir );
   strcat( buf, filename );
   if ( access( buf, F_OK ) == 0 )
      return 1;
   return 0;
}

int DeleteMarkerFile( char *homedir, char *filename )
{
   char buf[1024];

   strcpy( buf, homedir );
   strcat( buf, filename );
   unlink( buf );
   return 0;
}

#ifdef __APPLE__
int CheckX11Installed( int terminate, char *msg )
{
   char buf[1024];
   char *app;

   if ( access( "/Applications/Utilities/X11.app", F_OK ) == 0 )
      return 1;
   if ( terminate )
   {
      if ( getenv( "THE_RUNNING_AS_APP" ) )
         app = "The Hessling Editor";
      else
         app = "Terminal";
      sprintf( buf, "osascript -e 'tell app \"%s\" to display alert \"X11 is not installed\" message \"%s\" as warning buttons {\"Ok\"}' > /dev/null 2>&1", app, msg );
      system( buf );
      exit( 1 );
   }
   return 0;
}

int SetMacroPath( char *runtimedir )
{
   char buf[1024];
   int len = strlen( runtimedir );

   if ( getenv( "THE_RUNNING_AS_APP" ) )
   {
      strncpy( buf, runtimedir, len-6 );
      buf[len-6] = '\0';
      strcat( buf, "Extras" );
      setenv( "THE_MACRO_PATH", buf, 1 );
   }
   return 0;
}
#endif

int main( int argc, char *argv[] )
{
   char *argv0 = NULL;
   char *home = NULL;
   int i;
   int len;
   int rc=0;
   int path_len = 0;
   int override_default=THE_DEFAULT;
   int add_arg;
#ifdef __APPLE__
   char buf[1024];
   char *app;
#endif
#ifdef HAVE_FORK
   int pid;
#endif


   /* get our home directory */
   home = GetHomeDirectory();

   if ( argc == 2 && strcmp( argv[1], "--gui-default" ) == 0 )
   {
      CreateMarkerFile( home, ".thegui" );
      DeleteMarkerFile( home, ".theconsole" );
   }
   else if ( argc == 2 && strcmp( argv[1], "--console-default" ) == 0 )
   {
      CreateMarkerFile( home, ".theconsole" );
      DeleteMarkerFile( home, ".thegui" );
   }
   else
   {
      /* get our current runtime directory */
      argv0 = GetArgv0( argv[0] );
      if ( argv0 == NULL )
         exit(1);
#ifdef HAVE_REALPATH
      realpath( argv0, prog );
#else
      strcpy( prog, argv0 );
#endif
      /* remove the.exe from the full filename */
      len = strlen( prog );
      for ( i = len; i > -1; i--)
      {
         if ( prog[i] == THE_PATH_SEP )
            break;
      }
      path_len = i + 1;
      prog[path_len] = '\0';
      /*
       * If running from a Mac .app, set THE_MACRO_PATH...
       */
#ifdef __APPLE__
      SetMacroPath( prog );
#endif
      my_argv = (char **)malloc( sizeof(char *) * (argc+1) );
      my_argc = 0;
      for ( i = 0; i < argc; i++ )
      {
         add_arg = 1;
         /* check if we are overriding the default executable; -C for console; -G for GUI */
         if ( strlen( argv[i] ) == 2 && strcmp( argv[i], "-G" ) == 0 )
         {
#ifdef __APPLE__
            CheckX11Installed( 1, "Cannot run the GUI version of THE (with -G switch) as X11 is not installed on this machine." );
#endif
            prog[path_len] = '\0';
            strcat( prog, THE_GUI_EXE );
            if ( CheckBinary( prog, 1, "Cannot find the executable file: %s required with -G switch\n" ) )
               override_default = THE_GUI;
            add_arg = 0;
         }
         else if ( strlen( argv[i] ) == 2 && strcmp( argv[i], "-C" ) == 0 )
         {
            prog[path_len] = '\0';
            strcat( prog, THE_CONSOLE_EXE );
            if ( CheckBinary( prog, 1, "Cannot find the executable file: %s required with -C switch\n" ) )
               override_default = THE_CONSOLE;
            add_arg = 0;
         }
         else if ( strlen( argv[i] ) == 2 && strcmp( argv[i], "-h" ) == 0 )
         {
            usage();
            exit(0);
         }
#ifdef __1APPLE__
         /* hack to get rid of -psn_??? first arg when run from Finder */
         else if (strlen( argv[i] ) > 5 && strncmp( argv[i], "-psn_", 5 ) == 0 )
         {
            add_arg = 0;
         }
#endif
         if ( add_arg )
         {
            /*
             * We have an arg to pass to the spawned process. If the arg has a space in it, the
             * user MUST have quoted it originally.  Windows (of course) doesn't work like Unix
             * and execv() actually concats all the args into one string to pass to CreateProcess().
             * This results is multiple args for a single arg that has spaces.
             * We have to work around this by appending quotes to args that have spaces for Windows.
             */
#if defined(_WIN32) || defined(_WIN64)
            char *tmp = argv[i];
            if ( strchr( tmp, ' ' ) == NULL )
               my_argv[my_argc++] = argv[i];
            else
            {
               /* known memory leak! */
               tmp = malloc( 2 + strlen( argv[i] ) );
               if ( tmp )
               {
                  sprintf( tmp, "\"%s\"", argv[i] );
                  my_argv[my_argc++] = tmp;
               }
               else
                  /* multiple args better than nothing */
                  my_argv[my_argc++] = argv[i];
            }
#else
            my_argv[my_argc++] = argv[i];
#endif
         }
      }
      /* ensure the argv array is NULL terminated */
      my_argv[my_argc] = NULL;
      if ( override_default == THE_DEFAULT )
      {
         /* check whether .thegui or .theconsole exist */
         if ( CheckMarkerFile( home, ".thegui") )
         {
#ifdef __APPLE__
            CheckX11Installed( 1, "Cannot run the GUI version of THE (indicated by presence of $HOME/.thegui) as X11 is not installed on this machine." );
#endif
            prog[path_len] = '\0';
            strcat( prog, THE_GUI_EXE );
            if ( CheckBinary( prog, 1, "Cannot find the executable file: %s required with last use of --gui-default switch\n" ) )
               override_default = THE_GUI;
         }
         else if ( CheckMarkerFile( home, ".theconsole") )
         {
            prog[path_len] = '\0';
            strcat( prog, THE_CONSOLE_EXE );
            if ( CheckBinary( prog, 1, "Cannot find the executable file: %s required with last use of --console-default switch\n" ) )
               override_default = THE_CONSOLE;
         }
         else
         {
           /*
            * We don't have any specification to run a particular binary, so
            * lets look for the GUI version first, and then the console version.
            * If none found, abort!
            */
#ifdef __APPLE__
            if ( CheckX11Installed( 0, NULL ) )
#endif
            {
               prog[path_len] = '\0';
               strcat( prog, THE_GUI_EXE );
               if ( CheckBinary( prog, 0, NULL ) )
                  override_default = THE_GUI;
            }
            if ( override_default == THE_DEFAULT )
            {
               prog[path_len] = '\0';
               strcat( prog, THE_CONSOLE_EXE );
               if ( CheckBinary( prog, 0, NULL ) )
                  override_default = THE_CONSOLE;
            }
         }
      }
      /* now run our actual executable if we have found one */
      if ( override_default == THE_GUI )
      {
#ifdef HAVE_FORK
         pid = fork();
         if (pid < 0)
         {
            /* Could not fork */
            fprintf( stderr, "Unable to create child process\n" );
            exit(1);
         }
         else if (pid > 0)
         {
            /* Child created ok, so exit parent process */
            exit(0);
         }
         /* child continues */
#endif
         prog[path_len] = '\0';
         strcat( prog, THE_GUI_EXE );
         my_argv[0] = prog;
         rc = execv( prog, my_argv );
      }
      else if ( override_default == THE_CONSOLE )
      {
#ifdef __APPLE__
         char prog1[1024];
#endif

         prog[path_len] = '\0';
         strcat( prog, THE_CONSOLE_EXE );
#if defined(_WIN32) || defined(_WIN64)
         StartupConsole();
#endif
#ifdef __APPLE__
         /* if running as a bundled app, we need to startup nthe via Terminal */
         if ( getenv( "THE_RUNNING_AS_APP" ) )
         {
            strcpy( prog1, "open -n -W -b com.apple.Terminal \"" );
            strcat( prog1, prog );
            strcat( prog1, "\" &" );
            rc = system( prog1 );
         }
         else
         {
            my_argv[0] = prog;
            rc = execv( prog, my_argv );
         }
#else
         my_argv[0] = prog;
         rc = execv( prog, my_argv );
#endif
      }
      else
      {
         /* no binary found, abort */
         char *msg = "Cannot find any THE binary to run. Aborting!\n";
#if defined(_WIN32) || defined(_WIN64)
         StartupConsole();
#endif
#ifdef __APPLE__
         if ( getenv( "THE_RUNNING_AS_APP" ) )
            app = "The Hessling Editor";
         else
            app = "Terminal";
         sprintf( buf, "osascript -e 'tell app \"%s\" to display alert \"No THE binary found\" message \"%s\" as warning buttons {\"Ok\"}' > /dev/null 2>&1", app, msg );
         system( buf );
         exit(1);
#else
         fprintf( stderr, "%s\n", msg );
# if defined(_WIN32) || defined(_WIN64)
         ClosedownConsole();
# endif
         exit(1);
#endif
      }
   }
   exit(rc);
}

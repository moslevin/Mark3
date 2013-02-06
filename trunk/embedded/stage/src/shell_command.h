
#include "kerneltypes.h"
#include "ll.h"

//---------------------------------------------------------------------------
#define MAX_ARGV            (10)

//---------------------------------------------------------------------------
typedef int (ShellHandler*)(int argc_, void * argv_);

//---------------------------------------------------------------------------
class ShellCommand : public LinkListNode
{
public:  
    ShellCommand( const K_CHAR *pcCommand_, ShellHandler *pfHandler_ )
        { m_pcCommand = pcCommand_; m_pfHandler = pfHandler_; }
        
    void SetCommand( const K_CHAR *pcCommand_ ) { m_pcCommand = pcCommand_; }
    
    void SetHandler( ShellHandler* pfHandler_ ) { m_pfHandler = pfHandler_; }    

    const K_CHAR *GetCommand() { return m_pcCommand; }
        
    int  RunHandler( int argc_, void *argv_ ) { return m_pfHandler( argc_, argv_); }
    
private:
    const K_CHAR *m_pcCommand;
    ShellHandler m_pfHandler;    
};

//---------------------------------------------------------------------------
class Shell : public DoubleLinkList
{
public:
    /*!
        Add a new command to the list of commands supported in the shell
    */
    void AddCommand( ShellCommand *pclCommand_ ){ Add( pclCommand_ ); }
    
    /*!
        Parse the command string and run the associated command
    */
    void RunCommand( const K_CHAR *pcCommand_ );

private:
    int m_argc;             // Current argument count
    void *m_argv[MAX_ARGV]; // Current argument vector
};

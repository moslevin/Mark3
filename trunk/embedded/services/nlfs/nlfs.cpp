/*===========================================================================
     _____        _____        _____        _____
 ___|    _|__  __|_    |__  __|__   |__  __| __  |__  ______
|    \  /  | ||    \      ||     |     ||  |/ /     ||___   |
|     \/   | ||     \     ||     \     ||     \     ||___   |
|__/\__/|__|_||__|\__\  __||__|\__\  __||__|\__\  __||______|
    |_____|      |_____|      |_____|      |_____|

--[Mark3 Realtime Platform]--------------------------------------------------

Copyright (c) 2013 Funkenstein Software Consulting, all rights reserved.
See license.txt for more information
===========================================================================*/
/*!
    \file   nlfs.cpp
    \brief  Nice Little Filesystem (NLFS) implementation for Mark3
*/

#include "kerneltypes.h"
#include "nlfs.h"
#include "nlfs_file.h"
#include "memutil.h"
#include "nlfs_config.h"

//---------------------------------------------------------------------------
char NLFS::Find_Last_Slash( const char *szPath_ )
{
    uint8_t u8LastSlash = 0;
    uint8_t i = 0;
    while (szPath_[i])
    {
        if (szPath_[i] == '/')
        {
            u8LastSlash = i;
        }
        i++;
    }
    return u8LastSlash;
}

//---------------------------------------------------------------------------
bool NLFS::File_Names_Match( const char *szPath_, NLFS_Node_t *pstNode_)
{
    uint8_t u8LastSlash = Find_Last_Slash( szPath_ );
    uint8_t i;

    u8LastSlash++;
    for (i = 0; i < FILE_NAME_LENGTH; i++)
    {
        if (!szPath_[u8LastSlash+i] || !pstNode_->stFileNode.acFileName[i])
        {
            break;
        }
        if (szPath_[u8LastSlash+i] != pstNode_->stFileNode.acFileName[i])
        {
            return false;
        }
    }

    if (szPath_[u8LastSlash+i] != pstNode_->stFileNode.acFileName[i])
    {
        return false;
    }
    return true;
}

//---------------------------------------------------------------------------
void NLFS::Print_File_Details( uint16_t u16Node_ )
{
    NLFS_Node_t stFileNode;
    Read_Node(u16Node_, &stFileNode);

    DEBUG_PRINT(" Name       : %16s\n" , stFileNode.stFileNode.acFileName);
    DEBUG_PRINT(" Next Peer  : %d\n"   , stFileNode.stFileNode.u16NextPeer);
    DEBUG_PRINT(" Prev Peer  : %d\n"   , stFileNode.stFileNode.u16PrevPeer);
    DEBUG_PRINT(" user|Group : %d|%d\n", stFileNode.stFileNode.u8User,
                                         stFileNode.stFileNode.u8Group);

    DEBUG_PRINT(" Permissions: %04X\n" , stFileNode.stFileNode.u16Perms);
    DEBUG_PRINT(" Parent     : %d\n"   , stFileNode.stFileNode.u16Parent);
    DEBUG_PRINT(" First Child: %d\n"   , stFileNode.stFileNode.u16Child);
    DEBUG_PRINT(" Alloc Size : %d\n"   , stFileNode.stFileNode.u32AllocSize);
    DEBUG_PRINT(" File  Size : %d\n"   , stFileNode.stFileNode.u32FileSize);

    DEBUG_PRINT(" First Block: %d\n"   , stFileNode.stFileNode.u32FirstBlock);
    DEBUG_PRINT(" Last Block : %d\n"   , stFileNode.stFileNode.u32LastBlock);
}

//---------------------------------------------------------------------------
void NLFS::Print_Dir_Details( uint16_t u16Node_ )
{
    NLFS_Node_t stFileNode;
    Read_Node(u16Node_, &stFileNode);

    DEBUG_PRINT(" Name       : %16s\n" , stFileNode.stFileNode.acFileName);
    DEBUG_PRINT(" Next Peer  : %d\n"   , stFileNode.stFileNode.u16NextPeer);
    DEBUG_PRINT(" Prev Peer  : %d\n"   , stFileNode.stFileNode.u16PrevPeer);
    DEBUG_PRINT(" user|Group : %d|%d\n", stFileNode.stFileNode.u8User,
                                   stFileNode.stFileNode.u8Group);
    DEBUG_PRINT(" Permissions: %04X\n"  , stFileNode.stFileNode.u16Perms);
    DEBUG_PRINT(" Parent     : %d\n"   , stFileNode.stFileNode.u16Parent);
    DEBUG_PRINT(" First Child: %d\n"   , stFileNode.stFileNode.u16Child);
}

//---------------------------------------------------------------------------
void NLFS::Print_Free_Details( uint16_t u16Node_ )
{
    NLFS_Node_t stFileNode;
    Read_Node(u16Node_, &stFileNode);

    DEBUG_PRINT(" Next Free  : %d\n"    , stFileNode.stFileNode.u16NextPeer );
}

//---------------------------------------------------------------------------
void NLFS::Print_Node_Details( uint16_t u16Node_ )
{
    NLFS_Node_t stTempNode;
    Read_Node(u16Node_, &stTempNode);

    DEBUG_PRINT("\nNode: %d\n"
           " Node Type: ", u16Node_);
    switch (stTempNode.eBlockType)
    {
        case NLFS_NODE_FREE:
            DEBUG_PRINT( "Free\n" );
            Print_Free_Details(u16Node_);
            break;
        case NLFS_NODE_ROOT:
            DEBUG_PRINT( "Root Block\n" );
            break;
        case NLFS_NODE_FILE:
            DEBUG_PRINT( "File\n" );
            Print_File_Details(u16Node_);
            break;
        case NLFS_NODE_DIR:
            DEBUG_PRINT( "Directory\n" );
            Print_Dir_Details(u16Node_);
            break;
        default:
            break;
    }
}

//---------------------------------------------------------------------------
uint16_t NLFS::Pop_Free_Node(void)
{
    uint16_t u16RetVal = m_stLocalRoot.u16NextFreeNode;
    NLFS_Node_t stFileNode;

    if (INVALID_NODE == u16RetVal)
    {
        return 0;
    }

    // Update Claimed node
    Read_Node(u16RetVal, &stFileNode);
    m_stLocalRoot.u16NextFreeNode = stFileNode.stFileNode.u16NextPeer;
    stFileNode.stFileNode.u16NextPeer = INVALID_NODE;
    DEBUG_PRINT("Node %d allocated, next free %d\n", u16RetVal, m_stLocalRoot.u16NextFreeNode);
    Write_Node(u16RetVal, &stFileNode);

    //Update root node
    Read_Node(FS_CONFIG_BLOCK , &stFileNode);
    stFileNode.stRootNode.u16NextFreeNode = m_stLocalRoot.u16NextFreeNode;
    stFileNode.stRootNode.u16NumFilesFree--;
    Write_Node(FS_CONFIG_BLOCK, &stFileNode);

    return u16RetVal;
}

//---------------------------------------------------------------------------
void NLFS::Push_Free_Node(uint16_t u16Node_)
{
    NLFS_Node_t stFileNode;

    Read_Node(u16Node_, &stFileNode);
    stFileNode.stFileNode.u16NextPeer = m_stLocalRoot.u16NextFreeNode;
    m_stLocalRoot.u16NextFreeNode = u16Node_;

    Write_Node(u16Node_, &stFileNode);

    DEBUG_PRINT("Node %d freed\n", u16Node_);

    //Update root node
    Read_Node(FS_CONFIG_BLOCK , &stFileNode);
    stFileNode.stRootNode.u16NextFreeNode = m_stLocalRoot.u16NextFreeNode;
    stFileNode.stRootNode.u16NumFilesFree++;
    Write_Node(FS_CONFIG_BLOCK , &stFileNode);
}

//---------------------------------------------------------------------------
uint32_t NLFS::Pop_Free_Block(void)
{
    uint32_t u32RetVal = m_stLocalRoot.u32NextFreeBlock;
    NLFS_Block_t stFileBlock;
    NLFS_Node_t  stFileNode;

    if ((INVALID_BLOCK == u32RetVal) || (0 == m_stLocalRoot.u32NumBlocksFree))
    {
        DEBUG_PRINT("Out of data blocks\n");
        return 0;
    }

    Read_Block_Header(u32RetVal, &stFileBlock);

    m_stLocalRoot.u32NextFreeBlock = stFileBlock.u32NextBlock;
    m_stLocalRoot.u32NumBlocksFree--;
    stFileBlock.u32NextBlock = INVALID_BLOCK;

    Write_Block_Header(u32RetVal, &stFileBlock);

    Read_Node(FS_CONFIG_BLOCK , &stFileNode);

    stFileNode.stRootNode.u32NextFreeBlock = m_stLocalRoot.u32NextFreeBlock;
    stFileNode.stRootNode.u32NumBlocksFree--;

    Write_Node(FS_CONFIG_BLOCK , &stFileNode);

    DEBUG_PRINT("Allocated block %d, next free %d\n", u32RetVal, m_stLocalRoot.u32NextFreeBlock);
    return u32RetVal;
}

//---------------------------------------------------------------------------
void NLFS::Push_Free_Block(uint32_t u32Block_ )
{    
    NLFS_Block_t stFileBlock;
    NLFS_Node_t  stFileNode;

    Read_Block_Header(u32Block_, &stFileBlock);

    stFileBlock.u32NextBlock = m_stLocalRoot.u32NextFreeBlock;
    m_stLocalRoot.u32NextFreeBlock = u32Block_;

    Write_Block_Header(u32Block_, &stFileBlock);

    Read_Node(FS_CONFIG_BLOCK , &stFileNode);
    stFileNode.stRootNode.u32NextFreeBlock = m_stLocalRoot.u32NextFreeBlock;
    stFileNode.stRootNode.u32NumBlocksFree++;
    Write_Node(FS_CONFIG_BLOCK , &stFileNode);

    DEBUG_PRINT("Block %d freed\n", u32Block_);
}

//---------------------------------------------------------------------------
uint32_t NLFS::Append_Block_To_Node(NLFS_Node_t *pstFile_ )
{
    uint32_t u32Block;
    NLFS_Block_t stFileBlock;

    // Allocate a new block
    u32Block = Pop_Free_Block();
    if (u32Block == INVALID_BLOCK)
    {
        return -1;
    }

    // Initialize the block
    DEBUG_PRINT("reading block header\n");
    Read_Block_Header(u32Block, &stFileBlock);
    stFileBlock.u32NextBlock = INVALID_BLOCK;
    stFileBlock.uAllocated  = 1;

    DEBUG_PRINT("writing block header\n");
    Write_Block_Header(u32Block, &stFileBlock);

    // Update the previous last-block links (if there is one)
    DEBUG_PRINT("updating previous block %d\n", pstFile_->stFileNode.u32LastBlock);
    if (pstFile_->stFileNode.u32LastBlock != INVALID_BLOCK)
    {
        Read_Block_Header(pstFile_->stFileNode.u32LastBlock, &stFileBlock);
        stFileBlock.u32NextBlock = u32Block;
        Write_Block_Header(pstFile_->stFileNode.u32LastBlock, &stFileBlock);
    }
    else
    {
        DEBUG_PRINT("  previous block is invalid, setting as first\n");
        pstFile_->stFileNode.u32FirstBlock = u32Block;
    }

    pstFile_->stFileNode.u32LastBlock = u32Block;
    pstFile_->stFileNode.u32AllocSize += m_stLocalRoot.u32BlockSize;

    RootSync();

    return u32Block;
}

//---------------------------------------------------------------------------
uint16_t NLFS::Find_Parent_Dir(const char *szPath_)
{
    int i, j;
    uint8_t u8LastSlash = 0;
    uint16_t u16RetVal;
    char szTempName[FILE_NAME_LENGTH];
    NLFS_Node_t stFileNode;
    uint16_t u16TempPeer;

    Read_Node(FS_ROOT_BLOCK, &stFileNode );

    u16RetVal = FS_ROOT_BLOCK;

    if (szPath_[0] != '/')
    {
        DEBUG_PRINT("Only fully-qualified paths are supported.  Bailing\n");
        return -1;
    }

    // Starting from the root fs_block (which is the mount point...)
    u8LastSlash = Find_Last_Slash(szPath_);

    // a) Search for each "/" if we've got more than one...
    if (0 == u8LastSlash)
    {
        return u16RetVal;
    }

    u16TempPeer = stFileNode.stFileNode.u16Child;
    Read_Node(u16TempPeer, &stFileNode );

    i = 1;
    while (szPath_[i] && i < u8LastSlash)
    {
        NLFS_Node_t stTempNode;
        bool bMatch = false;

        j = 0;
        MemUtil::SetMemory(szTempName, 0, FILE_NAME_LENGTH);

        while (szPath_[i] && (szPath_[i] != '/') && j < FILE_NAME_LENGTH)
        {
            szTempName[j] = szPath_[i];
            i++;
            j++;
        }
        DEBUG_PRINT("Checking %s\n", szTempName );
        if (j == FILE_NAME_LENGTH && szPath_[i] != '/')
        {
            DEBUG_PRINT("Directory name too long, invalid\n");
            return -1;
        }
        else if (szPath_[i] != '/')
        {
            i++;
            continue;
        }

        // Check to see if there's a valid peer with this name...
        while (INVALID_NODE != u16TempPeer)
        {
            Read_Node(u16TempPeer, &stTempNode);
            if (NLFS_NODE_DIR == stTempNode.eBlockType)
            {
                if (true == MemUtil::CompareStrings(stTempNode.stFileNode.acFileName, szTempName))
                {
                    bMatch = true;
                    break;
                }
            }
            u16TempPeer = stTempNode.stFileNode.u16NextPeer;
        }

        // Matched the folder name descend into the folder
        if (bMatch)
        {
            DEBUG_PRINT("Matched folder: %s, node %d\n", szTempName, u16TempPeer);

            u16RetVal = u16TempPeer;

            u16TempPeer = stTempNode.stFileNode.u16Child;
            if (INVALID_NODE != u16TempPeer)
            {
                DEBUG_PRINT("Entering subdirectory %d\n", u16TempPeer);
                Read_Node(u16TempPeer, &stFileNode);
            }
            else
            {
                break;
            }
        }
        // Failed to match the folder name, bail
        else
        {
            DEBUG_PRINT("Could not match folder name, bailing\n");
            u16RetVal = -1;
            break;
        }

        if (i >= u8LastSlash)
        {
            break;
        }
        i++;
    }

    if (i == u8LastSlash)
    {
        // No more folders to traverse - we're successful.
        DEBUG_PRINT("Found root path for %s\n with node %d\n", szPath_, u16RetVal);
        return u16RetVal;
    }
    return INVALID_NODE;
}

//---------------------------------------------------------------------------
uint16_t NLFS::Find_File(const char *szPath_)
{
    NLFS_Node_t stTempNode;
    NLFS_Node_t stTempDir;

    uint16_t u16TempNode;

    uint16_t u16ParentDir = Find_Parent_Dir(szPath_);

    if (INVALID_NODE == u16ParentDir)
    {
        DEBUG_PRINT("invalid root dir\n");
        return INVALID_NODE;
    }

    Read_Node(u16ParentDir, &stTempDir);

    if (INVALID_NODE == stTempDir.stFileNode.u16Child)
    {
        return INVALID_NODE;
    }

    u16TempNode = stTempDir.stFileNode.u16Child;

    // See if there are matching child nodes
    while (INVALID_NODE != u16TempNode)
    {
        Read_Node(u16TempNode, &stTempNode);

        if (true == File_Names_Match(szPath_,&stTempNode ))
        {
            DEBUG_PRINT("matched file: %16s, node %d\n",
                   stTempNode.stFileNode.acFileName, u16TempNode);
            return u16TempNode;
        }

        u16TempNode = stTempNode.stFileNode.u16NextPeer;
    }
    DEBUG_PRINT("couldn't match file: %s\n", szPath_);
    return INVALID_NODE;
}

//---------------------------------------------------------------------------
void NLFS::Print(void)
{
    uint16_t i;
    for (i = 0; i < m_stLocalRoot.u16NumFiles; i++)
    {
        Print_Node_Details(i);
    }
}

//---------------------------------------------------------------------------
void NLFS::Set_Node_Name( NLFS_Node_t *pstFileNode_, const char *szPath_ )
{
    uint8_t i,j;
    uint8_t u8LastSlash = 0;

    // Search for the last "/", that's where we stop looking.
    i = 0;
    while (szPath_[i])
    {
        if (szPath_[i] == '/')
        {
            u8LastSlash = i;
        }
        i++;
    }

    // Parse out filename
    i = u8LastSlash + 1;
    j = 0;
    while (szPath_[i] && j < FILE_NAME_LENGTH)
    {     
        pstFileNode_->stFileNode.acFileName[j] = szPath_[i];
        j++;
        i++;
    }
    if (!szPath_[i]) // if no extension, we're done.
    {
        return;
    }
}

//---------------------------------------------------------------------------
uint16_t NLFS::Create_File_i(const char *szPath_, NLFS_Type_t eType_ )
{
    uint16_t u16Node;
    uint16_t u16RootNodes;

    NLFS_Node_t stFileNode;
    NLFS_Node_t stParentNode;
    NLFS_Node_t stPeerNode;

    // Tricky part - directory traversal
    u16RootNodes = Find_Parent_Dir(szPath_);

    if (INVALID_NODE == u16RootNodes)
    {
        DEBUG_PRINT("Unable to find path - bailing\n");
        return INVALID_NODE;
    }

    u16Node = Pop_Free_Node();
    if (!u16Node)
    {
        DEBUG_PRINT("Unable to allocate node.  Failing\n");
        return INVALID_NODE;
    }
    DEBUG_PRINT("New file using node %d\n", u16Node);

    // File node allocated, do something with it...
    // Set the file's name and extension

    Read_Node(u16Node, &stFileNode);

    // Set the file path
    Set_Node_Name(&stFileNode, szPath_);

    // Set block as in-use as a file
    stFileNode.eBlockType = eType_;

    // Zero-out the file
    stFileNode.stFileNode.u32FileSize = 0;

    // Set the default user and group, as well as perms
    stFileNode.stFileNode.u8User   = 0;   //!! ToDo - set real user/group IDs
    stFileNode.stFileNode.u8Group  = 0;
    stFileNode.stFileNode.u16Perms  = PERM_U_ALL | PERM_G_ALL | PERM_O_ALL;

    stFileNode.stFileNode.u16Child  = INVALID_NODE;
    stFileNode.stFileNode.u16Parent = u16RootNodes;

    // Update the parent node.
    Read_Node(u16RootNodes, &stParentNode);

    DEBUG_PRINT( "Parent's root child: %d\n", stParentNode.stFileNode.u16Child );
    // Insert node at the beginning of the peer list
    if (INVALID_NODE != stParentNode.stFileNode.u16Child)
    {
        stFileNode.stFileNode.u16NextPeer = stParentNode.stFileNode.u16Child;
        stFileNode.stFileNode.u16PrevPeer = INVALID_NODE;

        // Update the peer node.
        Read_Node(stFileNode.stFileNode.u16NextPeer , &stPeerNode);

        stPeerNode.stFileNode.u16PrevPeer = u16Node;
        stParentNode.stFileNode.u16Child = u16Node;

        DEBUG_PRINT("updating peer's prev: %d\n", stPeerNode.stFileNode.u16PrevPeer);
        Write_Node(stFileNode.stFileNode.u16NextPeer, &stPeerNode);
    }
    else
    {
        stParentNode.stFileNode.u16Child = u16Node;
        stFileNode.stFileNode.u16NextPeer = INVALID_NODE;
        stFileNode.stFileNode.u16PrevPeer = INVALID_NODE;
    }

    Write_Node(u16Node, &stFileNode);
    Write_Node(u16RootNodes, &stParentNode);

    RootSync();

    return u16Node;
}

//---------------------------------------------------------------------------
uint16_t NLFS::Create_File( const char *szPath_ )
{

    if (INVALID_NODE != Find_File(szPath_))
    {
        DEBUG_PRINT("Create_File: File already exists\n");
        return INVALID_NODE;
    }

    return Create_File_i( szPath_, NLFS_NODE_FILE );
}

//---------------------------------------------------------------------------
uint16_t NLFS::Create_Dir( const char *szPath_ )
{
    if (INVALID_NODE != Find_File(szPath_))
    {
        DEBUG_PRINT("Create_Dir: Dir already exists!\n");
        return INVALID_NODE;
    }

    return Create_File_i(szPath_, NLFS_NODE_DIR );
}

//---------------------------------------------------------------------------
void NLFS::Cleanup_Node_Links(uint16_t u16Node_, NLFS_Node_t *pstNode_)
{
    DEBUG_PRINT("Cleanup_Node_Links: Entering\n");

    if (INVALID_NODE != pstNode_->stFileNode.u16Parent)
    {
        NLFS_Node_t stParent;
        DEBUG_PRINT("Cleanup_Node_Links: Parent Node: %d\n", pstNode_->stFileNode.u16Parent);
        Read_Node(pstNode_->stFileNode.u16Parent, &stParent);

        DEBUG_PRINT("0\n");
        if (stParent.stFileNode.u16Child == u16Node_)
        {
            DEBUG_PRINT("1\n");
            stParent.stFileNode.u16Child = pstNode_->stFileNode.u16NextPeer;
            Write_Node(pstNode_->stFileNode.u16Parent, &stParent);
            DEBUG_PRINT("2\n");
        }
    }

    DEBUG_PRINT("a\n");
    if ( (INVALID_NODE != pstNode_->stFileNode.u16NextPeer) ||
         (INVALID_NODE != pstNode_->stFileNode.u16PrevPeer) )
    {
        NLFS_Node_t stNextPeer;
        NLFS_Node_t stPrevPeer;

        DEBUG_PRINT("b\n");
        if (INVALID_NODE != pstNode_->stFileNode.u16NextPeer)
        {
            DEBUG_PRINT("c\n");
            Read_Node(pstNode_->stFileNode.u16NextPeer, &stNextPeer);
            DEBUG_PRINT("d\n");
        }

        if (INVALID_NODE != pstNode_->stFileNode.u16PrevPeer)
        {
            DEBUG_PRINT("e\n");
            Read_Node(pstNode_->stFileNode.u16PrevPeer, &stPrevPeer);
            DEBUG_PRINT("f\n");
        }

        if (INVALID_NODE != pstNode_->stFileNode.u16NextPeer)
        {
            DEBUG_PRINT("g\n");
            stNextPeer.stFileNode.u16PrevPeer = pstNode_->stFileNode.u16PrevPeer;
            Write_Node(pstNode_->stFileNode.u16NextPeer, &stNextPeer);
            DEBUG_PRINT("h\n");
        }

        if (INVALID_NODE != pstNode_->stFileNode.u16PrevPeer)
        {
            DEBUG_PRINT("i\n");
            stPrevPeer.stFileNode.u16NextPeer = pstNode_->stFileNode.u16NextPeer;
            Write_Node(pstNode_->stFileNode.u16PrevPeer, &stPrevPeer);
            DEBUG_PRINT("j\n");
        }
    }
    pstNode_->stFileNode.u16Parent = INVALID_NODE;
    pstNode_->stFileNode.u16PrevPeer = INVALID_NODE;
    pstNode_->stFileNode.u16NextPeer = INVALID_NODE;
}

//---------------------------------------------------------------------------
uint16_t NLFS::Delete_Folder(const char *szPath_)
{
    uint16_t u16Node = Find_File(szPath_);
    NLFS_Node_t stNode;

    if (INVALID_NODE == u16Node)
    {
        DEBUG_PRINT("Delete_Folder: File not found!\n");
        return INVALID_NODE;
    }
    if (FS_ROOT_BLOCK == u16Node || FS_CONFIG_BLOCK == u16Node)
    {
        DEBUG_PRINT("Delete_Folder: Cannot delete root!\n");
        return INVALID_NODE;
    }

    Read_Node(u16Node, &stNode);

    if (NLFS_NODE_FILE == stNode.eBlockType)
    {
        DEBUG_PRINT("Delete_Folder: Path is not a Folder (is it a file?)");
        return INVALID_NODE;
    }

    if (INVALID_NODE != stNode.stFileNode.u16Child)
    {
        DEBUG_PRINT("Delete_Folder: Folder is not empty!");
        return INVALID_NODE;
    }

    Cleanup_Node_Links(u16Node, &stNode);

    stNode.eBlockType = NLFS_NODE_FREE;

    Write_Node(u16Node, &stNode);
    Push_Free_Node(u16Node);

    RootSync();

    return u16Node;
}

//---------------------------------------------------------------------------
uint16_t NLFS::Delete_File( const char *szPath_)
{
    uint16_t u16Node = Find_File(szPath_);
    uint32_t u32Curr;
    uint32_t u32Prev;
    NLFS_Node_t stNode;
    NLFS_Block_t stBlock;

    if (INVALID_NODE == u16Node)
    {
        DEBUG_PRINT("Delete_File: File not found!\n");
        return INVALID_NODE;
    }
    if (FS_ROOT_BLOCK == u16Node || FS_CONFIG_BLOCK == u16Node)
    {
        DEBUG_PRINT("Delete_File: Cannot delete root!\n");
        return INVALID_NODE;
    }

    Read_Node(u16Node, &stNode);

    if (NLFS_NODE_DIR == stNode.eBlockType)
    {
        DEBUG_PRINT("Delete_File: Path is not a file (is it a directory?)");
        return INVALID_NODE;
    }

    Cleanup_Node_Links(u16Node, &stNode);
    u32Curr = stNode.stFileNode.u32FirstBlock;

    while (INVALID_BLOCK != u32Curr)
    {
        Read_Block_Header(u32Curr, &stBlock);

        u32Prev = u32Curr;
        u32Curr = stBlock.u32NextBlock;

        Push_Free_Block(u32Prev);
    }

    stNode.eBlockType = NLFS_NODE_FREE;

    Write_Node(u16Node, &stNode);
    Push_Free_Node(u16Node);

    RootSync();

    return u16Node;
}

//---------------------------------------------------------------------------
void NLFS::Format(NLFS_Host_t *puHost_, uint32_t u32TotalSize_, uint16_t u16NumFiles_, uint16_t u16DataBlockSize_)
{
    uint32_t i;
    uint32_t u32NumBlocks;

    NLFS_Node_t  stFileNode;
    NLFS_Block_t stFileBlock;

    // Compute number of data blocks (based on FS Size and the number of file blocks)
    u32TotalSize_ -= ((uint32_t)u16NumFiles_) * sizeof(stFileNode);
    u32NumBlocks = u32TotalSize_ / ((((uint32_t)u16DataBlockSize_) + (sizeof(stFileBlock) - 1) + 3 ) & ~3);

    DEBUG_PRINT("Number of blocks %d\n", u32NumBlocks);

    // Set up the local_pointer -> this is used for the low-level, platform-specific
    // bits, allowing the FS to be used on RAM buffers, EEPROM's, networks, etc.
    m_puHost = puHost_;

    // Set the local copies of the data block byte-offset, as well as the data-block size
    m_stLocalRoot.u16NumFiles        = u16NumFiles_;
    m_stLocalRoot.u16NumFilesFree    = m_stLocalRoot.u16NumFiles - 2;
    m_stLocalRoot.u16NextFreeNode    = 2;

    m_stLocalRoot.u32NumBlocks       = u32NumBlocks;
    m_stLocalRoot.u32NumBlocksFree   = u32NumBlocks;
    m_stLocalRoot.u32NextFreeBlock   = 0;

    m_stLocalRoot.u32BlockSize       = ((((uint32_t)u16DataBlockSize_) + 3 ) & ~3 );
    m_stLocalRoot.u32BlockOffset     = (((uint32_t)u16NumFiles_) * sizeof(NLFS_Node_t));
    m_stLocalRoot.u32DataOffset      = m_stLocalRoot.u32BlockOffset
                                        + (((uint32_t)u32NumBlocks) * sizeof(NLFS_Block_t));

    // Create root data block node
    MemUtil::CopyMemory(&(stFileNode.stRootNode), &m_stLocalRoot, sizeof(m_stLocalRoot));
    stFileNode.eBlockType = NLFS_NODE_ROOT;

    DEBUG_PRINT("Writing root node\n");
    Write_Node(0, &stFileNode);
    DEBUG_PRINT("Done\n");

    // Create root mount point (directory)
    MemUtil::SetMemory(&stFileNode, 0, sizeof(stFileNode));
    stFileNode.eBlockType = NLFS_NODE_DIR;

    stFileNode.stFileNode.acFileName[0] = '/';

    stFileNode.stFileNode.u16NextPeer   = INVALID_NODE;
    stFileNode.stFileNode.u16PrevPeer   = INVALID_NODE;
    stFileNode.stFileNode.u8Group      = 0;
    stFileNode.stFileNode.u8User       = 0;
    stFileNode.stFileNode.u16Perms      = PERM_U_ALL | PERM_G_ALL | PERM_O_ALL;

    stFileNode.stFileNode.u16Parent     = INVALID_NODE;
    stFileNode.stFileNode.u16Child      = INVALID_NODE;

    stFileNode.stFileNode.u32AllocSize  = 0;
    stFileNode.stFileNode.u32FileSize   = 0;

    stFileNode.stFileNode.u32FirstBlock = INVALID_BLOCK;
    stFileNode.stFileNode.u32LastBlock  = INVALID_BLOCK;

    DEBUG_PRINT("Writing mount point\n");
    Write_Node(1, &stFileNode);
    DEBUG_PRINT("Done\n");

    stFileNode.stFileNode.acFileName[0] = 0;
    // Format nodes
    for (i = 2; i < u16NumFiles_; i++)
    {
        stFileNode.eBlockType = NLFS_NODE_FREE;
        if (i != u16NumFiles_ - 1)
        {
            stFileNode.stFileNode.u16NextPeer = (uint16_t)(i + 1);            
        }
        else
        {
            stFileNode.stFileNode.u16NextPeer = INVALID_NODE;
        }

        Write_Node(i, &stFileNode);
    }
    DEBUG_PRINT("File nodes formatted\n");

    // Format file blocks
    MemUtil::SetMemory(&stFileBlock, 0, sizeof(stFileBlock));

    DEBUG_PRINT("Writing file blocks\n");
    for (i = 0; i < u32NumBlocks; i++)
    {
        if (i == u32NumBlocks - 1)
        {
            stFileBlock.u32NextBlock = INVALID_BLOCK;
        }
        else
        {
            stFileBlock.u32NextBlock = i + 1;
        }

        Write_Block_Header(i, &stFileBlock);
    }    
}

//---------------------------------------------------------------------------
void NLFS::Mount(NLFS_Host_t *puHost_)
{
    NLFS_Node_t stRootNode;

    //!! Must set the host pointer first.
    m_puHost = puHost_;
    DEBUG_PRINT("Remounting FS %X - reading config node\n", puHost_);

    // Reload the root block into the local cache
    Read_Node(FS_CONFIG_BLOCK, &stRootNode);

    DEBUG_PRINT("Copying config node\n");
    MemUtil::CopyMemory(&m_stLocalRoot, &(stRootNode.stRootNode), sizeof(m_stLocalRoot));

    DEBUG_PRINT("Block Size", m_stLocalRoot.u32BlockSize );
    DEBUG_PRINT("Data Offset", m_stLocalRoot.u32DataOffset );
    DEBUG_PRINT("Block Offset", m_stLocalRoot.u32BlockOffset );
}

//---------------------------------------------------------------------------
void NLFS::RootSync()
{
    NLFS_Node_t stRootNode;

    MemUtil::CopyMemory(&(stRootNode.stRootNode), &m_stLocalRoot, sizeof(m_stLocalRoot));
    stRootNode.eBlockType = NLFS_NODE_ROOT;
    Write_Node(FS_CONFIG_BLOCK, &stRootNode);
}


//---------------------------------------------------------------------------
uint16_t NLFS::GetFirstChild( uint16_t u16Node_ )
{
    NLFS_Node_t stTemp;
    if (!u16Node_ || INVALID_NODE == u16Node_)
    {
        return INVALID_NODE;
    }
    Read_Node(u16Node_, &stTemp);

    if (stTemp.eBlockType != NLFS_NODE_DIR)
    {
        return INVALID_NODE;
    }

    return stTemp.stFileNode.u16Child;
}

//---------------------------------------------------------------------------
uint16_t NLFS::GetNextPeer( uint16_t u16Node_ )
{
    NLFS_Node_t stTemp;
    if (!u16Node_ || INVALID_NODE == u16Node_)
    {
        return INVALID_NODE;
    }
    Read_Node(u16Node_, &stTemp);
    return stTemp.stFileNode.u16NextPeer;
}

//---------------------------------------------------------------------------
bool NLFS::GetStat( uint16_t u16Node_, NLFS_File_Stat_t *pstStat_)
{
    NLFS_Node_t stTemp;
    if (!u16Node_ || INVALID_NODE == u16Node_)
    {
        return false;
    }
    Read_Node(u16Node_, &stTemp);
    pstStat_->u32AllocSize = stTemp.stFileNode.u32AllocSize;
    pstStat_->u32FileSize = stTemp.stFileNode.u32FileSize;
    pstStat_->u8Group = stTemp.stFileNode.u8Group;
    pstStat_->u8User = stTemp.stFileNode.u8User;
    pstStat_->u16Perms = stTemp.stFileNode.u16Perms;
    MemUtil::CopyMemory(pstStat_->acFileName, stTemp.stFileNode.acFileName, 16);
    return true;
}

